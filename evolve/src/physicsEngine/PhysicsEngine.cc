#ifndef STU_PHYSICS_ENGINE_CC
#define STU_PHYSICS_ENGINE_CC

#include "PhysicsEngine.h"

#define PI 3.14159

PhysicsEngine::PhysicsEngine()
{
    this->worldId = dWorldCreate();
    this->spaceId = dHashSpaceCreate( 0 );
    this->contactJointGroup = dJointGroupCreate( 0 );
    dWorldSetGravity( this->worldId, 0, 0, 0 );
    dWorldSetCFM(this->worldId, 0.5);
    dWorldSetContactMaxCorrectingVel(this->worldId, 50 );
    dWorldSetContactSurfaceLayer(this->worldId, 0.01);
    this->running = true;
    this->threaded = false;
}

bool PhysicsEngine::addItem( const string& name, const vector<double>& pos, double radius )
{
    logger.print( "Notify: entered Physics::addItem", this->doNotify );
    if( bodies.find( name ) != bodies.end() )
    {
        logger.print( "Warning: in PhysicsEngine::addItem attempted to add a body to the physics engine with a duplicate name of: " + name );
        return false;
    }

    if( pos.size() < 3 )
    {
        logger.print( "Warning: in PhysicsEngine::addItem, passed a position vector with a size < 3" );
        return false;
    }
    
    this->engineLock.writeLock();
    dGeomID geometry = dCreateSphere( this->spaceId, radius );
    dBodyID body = dBodyCreate(this->worldId);
    dGeomSetBody( geometry, body );
    dBodySetPosition( body, pos[0], pos[1], pos[2] );
    this->engineLock.unlock();
    bodies[name] = body;
    return true;
}

bool PhysicsEngine::addItem( const string& name, const vector<double>& pos, double edgex, double edgey, double edgez )
{
    logger.print( "Notify: entered Physics::addItem (box)", this->doNotify );
    if( bodies.find( name ) != bodies.end() )
    {
        logger.print( "Warning: in PhysicsEngine::addItem (box) attempted to add a body to the physics engine with a duplicate name of: " + name );
        return false;
    }

    if( pos.size() < 3 )
    {
        logger.print( "Warning: in PhysicsEngine::addItem (box), passed a position vector with a size < 3" );
        return false;
    }
    this->engineLock.writeLock();
    dGeomID geometry = dCreateBox( this->spaceId, edgex, edgey, edgez );
    dBodyID body = dBodyCreate( this->worldId );
    dGeomSetBody( geometry, body ); 
    dBodySetPosition( body, pos[0], pos[1], pos[2] );
    this->engineLock.unlock();
    this->bodies[name] = body;
    return true;
}


bool PhysicsEngine::addStaticItem( const string& name, const vector<double>& pos, double edgex, double edgey, double edgez )
{
    logger.print( "Notify: entered Physics::addStaticItem", this->doNotify );
    if( bodies.find( name ) != bodies.end() )
    {
        logger.print( "Warning: in PhysicsEngine::addStaticItem attempted to add a body to the physics engine with a duplicate name of: " + name );
        return false;
    }

    if( pos.size() < 3 )
    {
        logger.print( "Warning: in PhysicsEngine::addStaticItem, passed a position vector with a size < 3" );
        return false;
    }
    this->engineLock.writeLock();
    dGeomID geometry = dCreateBox( this->spaceId, edgex, edgey, edgez );
    dGeomSetBody( geometry, 0 ); //0 means static body (no rigid body associated with it)
    dGeomSetPosition( geometry, pos[0], pos[1], pos[2] );
    this->engineLock.unlock();
    this->geometries[name] = geometry;
    return true;
}

bool PhysicsEngine::addStaticItem( const string& name, const vector<double>& pos, double radius )
{
    logger.print( "Notify: entered Physics::addStaticItem", this->doNotify );
    if( bodies.find( name ) != bodies.end() )
    {
        logger.print( "Warning: in PhysicsEngine::addStaticItem attempted to add a body to the physics engine with a duplicate name of: " + name );
        return false;
    }

    if( pos.size() < 3 )
    {
        logger.print( "Warning: in PhysicsEngine::addStaticItem, passed a position vector with a size < 3" );
        return false;
    }
    this->engineLock.writeLock();
    dGeomID geometry = dCreateSphere( this->spaceId, radius );
    dGeomSetBody( geometry, 0 ); //0 means static body (no rigid body associated with it)
    dGeomSetPosition( geometry, pos[0], pos[1], pos[2] );
    this->engineLock.unlock();
    this->geometries[name] = geometry;
    return true;
}

bool PhysicsEngine::connectItems( const string& item1, const string& item2, const string& jointType, const JointProperties& jointProperties )
{
    if( this->bodies.find( item1 ) == this->bodies.end() || 
        this->bodies.find( item2 ) == this->bodies.end() )
    {
        if( this->bodies.find( item1 ) == this->bodies.end() )
        {
            logger.print( "Warning: in PhysicsEngine::connectItems, " + item1 + " not in engine" );
        }
        else
        {
            logger.print( "Warning: in PhysicsEngine::connectItems, " + item2 + " not in engine" );
        }
        return false;
    }
    dJointID jointId = 0;
    if( jointType == "ball" )
    {
        if( jointProperties.anchor.size() < 3 )
        {
            logger.print( "Warning: in PhysicsEngine::connectItems, ball joint selected, but joint properties.anchor does not have 3 dimensions.");
            return false;
        }
        this->engineLock.writeLock();
        jointId = dJointCreateBall( this->worldId, 0 );
        dJointSetBallAnchor( jointId, jointProperties.anchor[0], jointProperties.anchor[1], jointProperties.anchor[2] );
        this->engineLock.unlock();
    }
    else if( jointType == "slider" )
    {
        if( jointProperties.axis.size() < 3 )
        {
            logger.print( "Warning: in PhysicsEngine::connectItems, slider joint selected, but joint properties.axis does not have 3 dimensions.");
            return false;
        }
        //ok, no longer seg faults (when moving setAxis to after body creation)
        //but now the thing explodes :(
        this->engineLock.writeLock();
        jointId = dJointCreateSlider( this->worldId, 0 );
        this->engineLock.unlock();
    }
    else if( jointType == "fixed" )
    {
        this->engineLock.writeLock();
        jointId = dJointCreateFixed( this->worldId, 0 );
        this->engineLock.unlock();
    }
    else if( jointType == "hinge" )
    {
        this->engineLock.writeLock();
        jointId = dJointCreateHinge( this->worldId, 0 );
        dJointSetHingeAnchor( jointId, jointProperties.anchor[0], jointProperties.anchor[1], jointProperties.anchor[2] );
        dJointSetHingeParam( jointId, dParamLoStop, -(PI * 0.02) );
        dJointSetHingeParam( jointId, dParamHiStop, (PI * 0.02) );
        dJointSetHingeParam( jointId, dParamFudgeFactor, 1 );
        dJointSetHingeParam( jointId, dParamBounce, 0.4 );
        dJointSetHingeParam( jointId, dParamCFM, 0.001 );
        dJointSetHingeParam( jointId, dParamStopERP, 0.8 );
        dJointSetHingeParam( jointId, dParamStopCFM, 0.001 );

        this->engineLock.unlock();
    }
    else if( jointType == "universal" )
    {
        if( jointProperties.anchor.size() < 3 )
        {
            logger.print( "Warning: in PhysicsEngine::connectItems, ball joint selected, but joint properties.anchor does not have 3 dimensions.");
            return false;
        }

        this->engineLock.writeLock();
        jointId = dJointCreateUniversal( this->worldId, 0 );
        dJointSetUniversalAnchor( jointId, jointProperties.anchor[0], jointProperties.anchor[1], jointProperties.anchor[2] );
        dJointSetUniversalParam( jointId, dParamLoStop, -(PI * 0.02) );
        dJointSetUniversalParam( jointId, dParamHiStop, (PI * 0.02) );
        dJointSetUniversalParam( jointId, dParamFudgeFactor, 1 );
        dJointSetUniversalParam( jointId, dParamBounce, 0.4 );
        dJointSetUniversalParam( jointId, dParamCFM, 0.001 );
        dJointSetUniversalParam( jointId, dParamStopERP, 0.8 );
        dJointSetUniversalParam( jointId, dParamStopCFM, 0.001 );
        this->engineLock.unlock();
    }
    else if( jointType == "motor" )
    {
        this->engineLock.writeLock();
        this->engineLock.unlock();
    }
    else
    {
        logger.print( "Warning: in PhysicsEngine::connectItems, " + jointType + " not supported" );
        return false;
    }
    this->engineLock.writeLock();
    dJointAttach( jointId, this->bodies[item1], this->bodies[item2] );
    if( jointType == "fixed" )
    {
        dJointSetFixed( jointId );
    }
    else if( jointType == "slider" )
    {
        //note these have different meanings depending on whether it is
        //a rotational joint or not. I think!
        //survives a little longer when we tweak these settings (CFM up, ERP down, etc)
        //OK, with these settings we "survive", but it doesn't quite work like you'd expect-
        //a lot of random jostling
        dJointSetSliderParam( jointId, dParamLoStop, 0.1 );
        dJointSetSliderParam( jointId, dParamHiStop, 70 );
        dJointSetSliderParam( jointId, dParamFudgeFactor, 0.6 );
        dJointSetSliderParam( jointId, dParamBounce, 0.2 );
        dJointSetSliderParam( jointId, dParamCFM, 0.8 );
        dJointSetSliderParam( jointId, dParamStopERP, 0.2 );
        dJointSetSliderParam( jointId, dParamStopCFM, 0.8 );
        dJointSetSliderAxis( jointId, jointProperties.axis[0], jointProperties.axis[1], jointProperties.axis[2] );

    }
    this->engineLock.unlock();
    //only add the joint id to one of the joints, so we don't try to delete it twice later.
    //(by removing one body, then the other, which would trigger a delete of all joints associated
    //with each body). Yes, some joints will persist a little longer then the should. Better than
    //trying to find out what happens with ODE when you delete twice...
    this->joints[item1].push_back( jointId );
    return true;
}

bool PhysicsEngine::removeItem( const string& name )
{
    if( bodies.find( name ) == bodies.end() and geometries.find( name ) == geometries.end() )
    {
        logger.print( "Notify: in PhysicsEngine, attempt to remove an item " + name + " that does not exist", this->doNotify );
        return false;
    }

    if( bodies.find( name ) != bodies.end() )
    {
        this->engineLock.writeLock();
        dBodyDestroy( this->bodies[name] );
        this->engineLock.unlock();
        this->bodies.erase(name);
        //delete all joints associated with this body from the physics engine & (more importantly) from ODE.
        if( this->joints.find( name ) != this->joints.end() )
        {
            for( vector<dJointID>::const_iterator i = this->joints[name].begin(); i != this->joints[name].end(); ++i )
            {
                this->engineLock.writeLock();
                dJointDestroy( (*i) );
                this->engineLock.unlock();
            }
            this->joints[name].clear();
        }
    }
    if( geometries.find( name ) != geometries.end() )
    {
        this->engineLock.writeLock();
        dGeomDestroy( this->geometries[name] );
        this->engineLock.unlock();
        this->geometries.erase(name);
    }
    return true;
}

bool PhysicsEngine::setGravity( double x, double y, double z )
{
    this->engineLock.writeLock();
    dWorldSetGravity( this->worldId, x, y, z );
    this->engineLock.unlock();
    return true;
}

bool PhysicsEngine::setPosition( const string& name, const vector<double>& position )
{
    logger.print( "Notify: entered Physics::setPosition", this->doNotify );
    if( bodies.find( name ) == bodies.end() )
    {
        logger.print( "Notify: in PhysicsEngine::setPosition attempted to set the position of a body that does not exist, the name tried was: " + name, this->doNotify );
        return false;
    }

    if( position.size() < 3 )
    {
        logger.print( "Warning: Oop! we only support objects in a 3-manifold\n" +
                      string("Warning: (Your position vector in PhysicsEngine::updateItemPosition had a size < 3)") );
    }

    this->engineLock.writeLock();
    dBodySetPosition( bodies[name], position[0], position[1], position[2] );
    this->engineLock.unlock();
    return true;
}

bool PhysicsEngine::setPosition( const string& name, double x, double y, double z )
{
    logger.print( "Notify: entered Physics::setPosition(x,y,z)", this->doNotify );
    if( bodies.find( name ) == bodies.end() )
    {
        logger.print( "Notify: in PhysicsEngine::setPosition(x,y,z) attempted to set the position of a body that does not exist, the name tried was: " + name, this->doNotify );
        return false;
    }
    
    this->engineLock.writeLock();
    dBodySetPosition( bodies[name], x, y, z );
    this->engineLock.unlock();
    return true;
}

bool PhysicsEngine::setMass( const string& name, double mass )
{
    logger.print( "Notify: entered Physics::setMass", this->doNotify );
    if( bodies.find( name ) == bodies.end() )
    {
        logger.print( "Notify: in PhysicsEngine::setMass, attempted to set the mass of a body that does not exist, the name tried was: " + name, this->doNotify );
        return false;
    }
    
    this->engineLock.writeLock();
    dMass dmass;
    dMassSetZero( &dmass );
    dMassSetSphere( &dmass, mass / 3.14159, 1 );
    dBodySetMass( bodies[name], &dmass );
    this->engineLock.unlock();
    return true;

}

bool PhysicsEngine::setLinearVelocity( const string& name, const vector<double>& vel )
{
    logger.print( "Notify: entered Physics::setLinearVelocity", this->doNotify );
    if( bodies.find( name ) == bodies.end() )
    {
        logger.print( "Notify: in PhysicsEngine::setLinearVelocity attempted to set the linear velocity of a body that does not exist, the name tried was: " + name, this->doNotify );
        return false;
    }

    if( vel.size() < 3 )
    {
        logger.print( "Warning: in PhysicsEngine::setLinearVelocity, passed a velocity vector with a size < 3" );
        return false;
    }
    
    this->engineLock.writeLock();
    dBodySetLinearVel( bodies[name], vel[0], vel[1], vel[2] );
    this->engineLock.unlock();
    return true;

}

bool PhysicsEngine::getPosition( const string& name, vector<double>& pos )
{
    logger.print( "Notify: entered Physics::getPosition", this->doNotify );
    if( this->bodies.find( name ) == this->bodies.end() && 
        this->geometries.find( name ) == this->geometries.end()  )
    {
        //this happens often enough that we don't bother with warning
        logger.print( "Notify: in PhysicsEngine::getPosition(x,y,z) attempted to get the position of a body/geom that does not exist, the name tried was: " + name, this->doNotify );
        return false;
    }

    if( this->bodies.find( name ) != this->bodies.end() )
    {
        this->engineLock.readLock();
        const dReal* phy_pos = dBodyGetPosition( this->bodies[name] );
        this->engineLock.unlock();
        pos.push_back( phy_pos[0] );
        pos.push_back( phy_pos[1] );
        pos.push_back( phy_pos[2] );
    }
    else
    {
        this->engineLock.readLock();
        const dReal* phy_pos = dGeomGetPosition( this->geometries[name] );
        this->engineLock.unlock();
        pos.push_back( phy_pos[0] );
        pos.push_back( phy_pos[1] );
        pos.push_back( phy_pos[2] );
    }
    return true;
}


bool PhysicsEngine::getGravity( vector<double>& grav )
{
    dVector3 odeGrav;
    this->engineLock.readLock();
    dWorldGetGravity( this->worldId, odeGrav );
    this->engineLock.unlock();
    grav.push_back( odeGrav[0] );
    grav.push_back( odeGrav[1] );
    grav.push_back( odeGrav[2] );
    return true;
}

bool PhysicsEngine::doPhysics()
{
    if( this->threaded )
    {
        logger.print( "Warning: in PhysicsEngine::doPhysics(), called non-threaded method after thread was started via startPhysics(). Don't do that!" );
        return false;
    }
    dSpaceCollide( this->spaceId, this, &nearCollisionCallback );
    dWorldStep( this->worldId, 1 );
    dJointGroupEmpty( this->contactJointGroup );
    return true;
}

bool PhysicsEngine::startPhysics()
{
    if( !this->running )
    {
        logger.print( "Warning: in PhysicsEngine::startPhysics, attempted to start physics as the object was being deleted..." );
        return false;
    }
    if( this->threaded )
    {
        logger.print( "Warning: in PhysicsEngine::startPhysics, attempted to start more than one.. don't do that..." );
        return false;
    }
    //no longer simple happy land from here on out... 
    this->threaded = true;
    pthread_t* pthread = new pthread_t();
    pthread_attr_t* pthread_attr = new pthread_attr_t();
    pthread_attr_init( pthread_attr );
    int ret = 0;
    if( (ret = pthread_create( pthread, pthread_attr, &physicsThread, this )) != 0 )
    {
        logger.print( "Warning: in PhysicsEngine::startPhysics, thread failed: " + string( strerror( ret ) ) );
        this->threaded = false;
        return false;
    }
    pthread_attr_destroy( pthread_attr );
    delete pthread;
    return true;
}

void* evolve::physicsThread( void* pe )
{
    PhysicsEngine* _this = (PhysicsEngine*)pe;
    while( _this->running )
    {
        //sleeps are to yield the cpu (trying gamely to keep interactivety level up)
        _this->engineLock.writeLock();
        dSpaceCollide( _this->spaceId, _this, &nearCollisionCallback );
        _this->engineLock.unlock();
        usleep(1000);
        _this->engineLock.writeLock();
        dWorldQuickStep( _this->worldId, 1 );
        dJointGroupEmpty( _this->contactJointGroup );
        _this->engineLock.unlock();
        usleep(1000);
    }
    pthread_exit( NULL );
    return NULL;
}

/*
 * callback for ODE. friend of PhysicsEngine. _this is meant to be 
 * a this pointer at the PhysicsEngine object currently using the callback.
 */
void evolve::nearCollisionCallback( void* pe, dGeomID id1, dGeomID id2 )
{
    PhysicsEngine* _this = (PhysicsEngine*)pe;

    dBodyID b1 = dGeomGetBody( id1 );
    dBodyID b2 = dGeomGetBody( id2 );

    if( b1 && b2 && dAreConnectedExcluding( b1, b2, dJointTypeContact ) )
    {
        return;
    }

    //TODO: ...
    //er, technically the second argument to dCollide is a "flag" whose lower 
    //16 bits is an integer with the max number of contacts. And the upper 16 bits
    //are "Reserved for future use". Yup, that's right.
    //Heck, these guys should write an RFC
    short max_contacts = 4;
    int num_contacts = 0;
    dContactGeom* contactGeoms = new dContactGeom[max_contacts]; 
    num_contacts = dCollide( id1, id2, (unsigned int)max_contacts, contactGeoms, sizeof(dContactGeom) );
    if( num_contacts == 0 )
    {
        delete contactGeoms;
        return;
    }

    dContact* contacts = new dContact[num_contacts];
    for( int i = 0; i < num_contacts; ++i )
    {
        contacts[i].surface.mode = dContactBounce | dContactSoftCFM | dContactSoftCFM;
        contacts[i].surface.mu = dInfinity;
        contacts[i].surface.bounce = 0.3;
        contacts[i].surface.bounce_vel = 0.3f;
        contacts[i].surface.soft_cfm = 0.02;
        contacts[i].surface.soft_erp = 0.8;
        contacts[i].geom = contactGeoms[i];
    }

    for( int i = 0; i < num_contacts; ++i )
    {
        dJointID c = dJointCreateContact( _this->worldId, _this->contactJointGroup, &contacts[i] );
        dJointAttach( c, b1, b2 );
    }
    
    delete contactGeoms;
    delete contacts;
}

#endif
