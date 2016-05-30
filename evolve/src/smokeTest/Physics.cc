#ifndef STU_PHYSICS_CC
#define STU_PHYSICS_CC

//#define DO_QUAD_TREE 1

#include "Physics.h"

bool stu_physics_collided = false;

void nearCollisionCallback( void* data, dGeomID id1, dGeomID id2 );

Physics::Physics()
{
    this->doNotify = false;
    this->worldId = dWorldCreate();
#ifdef DO_QUAD_TREE
    dVector3 center;
    center[0] = 0;
    center[1] = 0;
    center[2] = 0;
    dVector3 extents;
    extents[0] = 500;
    extents[1] = 500;
    extents[2] = 500;
    int depth = 10;
    this->spaceId = dQuadTreeSpaceCreate( 0, center, extents, depth );
#endif
#ifndef DO_QUAD_TREE
    this->spaceId = dHashSpaceCreate( 0 );
#endif
        
    dWorldSetGravity( worldId, 0, 0, 0 );
}

unsigned int Physics::addSystem( vector< vector< double > > system, vector<double> radii, bool printDebug )
{
    if( printDebug && this->doNotify )
    {
        logger.print( "Notify: entered addSystem (with vector radii argument)" );
    }
 
    if( radii.size() != system.size() )
    {
        logger.print( "You gave me more units then radii", printDebug );
        return 0;    
    }
   
    for( unsigned int i = 0; i < system.size(); ++i )
    {
        dGeomID geom = dCreateSphere( spaceId, radii[i] );
        double x = system[i][0];
        double y = system[i][1];
        double z = system[i][2];
        dGeomSetPosition( geom, x, y, z );
        dBodyID body = dBodyCreate(worldId);
        dGeomSetBody( geom, body );
        dBodySetPosition( body, x, y, z );
        dSpaceCollide(this->spaceId, 0, &nearCollisionCallback );

        //we are assuming since no physics have been done, a collision will be set only
        //for the current addition, since any previous additions would have triggered a collision
        //when they were added.
        if( !stu_physics_collided )
        {
            geometries.push_back( geom );
            bodies.push_back( body );
        }
        else
        {
            dSpaceRemove( this->spaceId, geom );
            stu_physics_collided = false;
            return i;
        }
    }

    return system.size();
}

unsigned int Physics::addSystem( vector< vector< double > > system, double radius, bool printDebug )
{
    if( printDebug && this->doNotify )
    {
        logger.print( "Notify: entered addSystem (with vector radii argument)" );
    }
  
    for( unsigned int i = 0; i < system.size(); ++i )
    {
        dGeomID geom = dCreateSphere( spaceId, radius );
        double x = system[i][0];
        double y = system[i][1];
        double z = system[i][2];
        dGeomSetPosition( geom, x, y, z );
        dBodyID body = dBodyCreate(worldId);
        dGeomSetBody( geom, body );
        dBodySetPosition( body, x, y, z );
        dSpaceCollide(this->spaceId, 0, &nearCollisionCallback );

        //we are assuming since no physics have been done, a collision will be set only
        //for the current addition, since any previous additions would have triggered a collision
        //when they were added.
        if( !stu_physics_collided )
        {
            geometries.push_back( geom );
            bodies.push_back( body );
        }
        else
        {
            dSpaceRemove( this->spaceId, geom );
            stu_physics_collided = false;
            return i;
        }
    }

    return system.size();
}

/**
 * @param system a system of branches
 * @param radius the radius to use when testing for collisions for each unit
 * @return a map of where collisions occurred, indexed by level
 * */
map<int, unsigned int> Physics::addSystem( const map< int,vector< vector< double > > >& system, double radius, bool printDebug )
{
    Timer timer;

    timer.startTimer( 0 );
    if( printDebug && this->doNotify )
    {
        logger.print( "Notify: entered addSystem (with vector radii argument)" );
    }

    map<int, unsigned int> ret;
   
    stu_physics_collided = false;

    long totalOdeMicroseconds = 0;
    long totalCollisionMicroseconds = 0;
    /**
    *
    * OK, I guess this needs a little explain'
    * I loop over every system
    * For every system, I loop over every branch, as long as the parent of that 
    * branch exists (you don't want to grow branches from non-existent parents)
    * Once a collision occurs, record it in the return map.
    * */  
    for( map< int, vector< vector<double> > >::const_iterator iter = system.begin(); iter != system.end(); ++iter )
    {
        int collisionIndex = 0;
        for( unsigned int i = 0; i < (*iter).second.size(); ++i )
        {
            //if we are at the start of a branch, and find a two-tuple, it's a pointer
            if( i == 0 && (*iter).second[i].size() == 2 )
            {
                unsigned int levelPointer = (unsigned int)(*iter).second[i][0]; 
                unsigned int indexPointer = (unsigned int)(*iter).second[i][1];

                //if we can't find the parent branch in ret, this means it wasn't rendered, so we bail
                if( ret.find( levelPointer ) == ret.end() )
                {
                    if( printDebug )
                    {
                        ostringstream idStream;
                        ostringstream branchIdStream;
                        branchIdStream << (*iter).first;
                        idStream << levelPointer;
                        logger.print( "Warning: couldn't find parent branch with id: " + idStream.str() + " for child branch: " + branchIdStream.str() );
                    }
                    
                    stu_physics_collided = true;
                    break;
                }

                if( indexPointer >= ret[levelPointer] )
                {
                    //cout << "not rendering branch: " << (*iter).first << endl; 
                    //cout << "parent branch: " << levelPointer << " had collision at: " << ret[levelPointer] << endl;
                    stu_physics_collided = true;
                    break;
                }

                //cout << " branch: " << (*iter).first << " parent: " << levelPointer << " indexPointer: " << indexPointer << " parent level collision at: " << ret[levelPointer] << " parent size: " << system[levelPointer].size() << endl;
            }
            
            if( (*iter).second[i].size() < 3 )
            {
                //cout << "skipping pointer in branch: " << (*iter).first << endl;
                continue;
            }

            timer.startTimer( 1 );
            dGeomID geom = dCreateSphere( spaceId, radius );
            double x = (*iter).second[i][0];
            double y = (*iter).second[i][1];
            double z = (*iter).second[i][2];
            dGeomSetPosition( geom, x, y, z );
            dBodyID body = dBodyCreate(worldId);
            dGeomSetBody( geom, body );
            dBodySetPosition( body, x, y, z );
            timer.startTimer( 2 );
            dSpaceCollide(this->spaceId, 0, &nearCollisionCallback );
            timer.stopTimer( 2 );
            timer.stopTimer( 1 );
            totalOdeMicroseconds += timer.getMicroseconds( 1 );
            totalCollisionMicroseconds += timer.getMicroseconds( 2 );

            //we are assuming since no physics have been done, a collision will be set only
            //for the current addition, since any previous additions would have triggered a collision
            //when they were added.
            if( !stu_physics_collided )
            {
                geometries.push_back( geom );
                bodies.push_back( body );
                collisionIndex++;
            }
            else
            {
                //cout << "collision occurred for branch: " << (*iter).first << endl;
                //cout << "for point: " << x << "," << y << "," << z << endl;
                dSpaceRemove( this->spaceId, geom );
                break;
            }
        }

        //if we left the for loop over a collision
        if( stu_physics_collided )
        {
            stu_physics_collided = false;
            ret[(*iter).first] = collisionIndex;
        }
        else
        {
            ret[(*iter).first] = (*iter).second.size();
        }
    }

    timer.stopTimer( 0 );
    cout << "total physics func time: " << timer.getMicroseconds( 0 ) * 1e-6 << endl;
    cout << "total ode time (in func): " << totalOdeMicroseconds * 1e-6 << endl;
    cout << "total collision time (in func): " << totalCollisionMicroseconds * 1e-6 << endl;
    return ret;
}

void Physics::setLogfileName( string logfileName, bool printDebug )
{
    if( printDebug && this->doNotify )
    {
        logger.print( "Notify: entered setLogfileName in physics" );
    }
    logger.setLogFileName( logfileName, printDebug );
}

void Physics::setNotify( bool notify, bool printDebug )
{
    if( printDebug && this->doNotify )
    {
        logger.print( "Notify: entered doNotify" );
    }

    this->doNotify = notify;
}

void nearCollisionCallback( void* data, dGeomID id1, dGeomID id2 )
{
     
    dBodyID b1 = dGeomGetBody( id1 );
    dBodyID b2 = dGeomGetBody( id2 );

    if( b1 && b2 && dAreConnectedExcluding( b1, b2, dJointTypeContact ) )
        return;

    //we only care about the first contact currently
    int max_contacts = 1;
    dContact contact;
    contact.surface.mode = dContactBounce | dContactSoftCFM;
    contact.surface.mu = dInfinity;
    contact.surface.mu2 = 0;
    contact.surface.bounce = 0.2f;
    contact.surface.bounce_vel = 0.5f;
    contact.surface.soft_cfm = 0.3f;

    //doCollide returns the number of contacts/collisions (here it's limited to 1 anyway)
    if( dCollide( id1, id2, max_contacts, &contact.geom, sizeof(dContact) ) > 0 )
    {
        stu_physics_collided = true;

        //remove id1? id2???
        //store the id of currently added object in class member, and if 
        //one of the id's equal it, remove it?
        //does dSpaceRemove(dSpaceID, dGeomID) remove body entries
        //made in call to BodyCreate(worldId)?
    }
}

#endif
