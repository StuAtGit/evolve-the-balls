#ifndef LSystem_WORLD_OBJECT_CC_STU
#define LSystem_WORLD_OBJECT_CC_STU

#include "LSystemWorldObject.h"

unsigned int LSystemWorldObject::objectCounter = 0;

bool LSystemWorldObject::create( const string& name, double posx, double posy, double posz )
{
    this->logger.print( "Notify: entered LSystemWorldObject::create ", this->doNotify );
    this->graphics = true;
    this->resourceName = name;
    LSystemWorldObject::objectCounter++;
    this->objectId = LSystemWorldObject::objectCounter;
    this->lastUpdateTime = 0;
    this->lastPhysicsUpdate = 0;
    this->growthRate = 0.2;
    this->physicsUpdateRate = 1;

    //to avoid mem leak, and just to avoid overwriting when we don't want too...
    if( this->system == NULL )
    {
        this->system = new LSystemGeneric();
        if( !this->system->initialize( this->name + ".sys" ) )
        {
            this->logger.print( "Warning: in LSystemWorldObject::create, failed to initialize LSystemGeneric() with sys file: " + this->name + ".sys" );
            return false;
        }
        string query = "";
        for( unsigned int i = 0; i < 100; ++i )
        {
            this->system->run( string("grow"), query );
        }
    }

    if( this->interpreter == NULL )
    {
        this->interpreter = this->incInterpreter = new LSystemIncrementalInterpreter( this->system );
    }

    //set default attributes, just in case the config file does not have them
    this->interpreter->setAttribute( "Mesh", resourceName );
    this->interpreter->setAttribute( "JointType", "ball" );
    this->interpreter->loadAttributes( this->name + ".attr" );
    string doPhysics = this->interpreter->getAttributeValue( "Physics" );
    if( doPhysics == "off" || doPhysics == "false" )
    {
        this->physics = false;
    }
    string growthRateConfig = this->interpreter->getAttributeValue( "GrowthRate" );

    if( growthRateConfig != "" )
    {
        bool success = false;
        double rate = Utility::stringTo<double>( growthRateConfig, success );
        if( !success )
        {
            this->logger.print( "Warning: in LSystemWorldObject::create, GrowthRate not given as a number in the config file, defaulting to: " + Utility::toString( this->growthRate ) );
            this->logger.print( "Warning: entry was: " + growthRateConfig );
        }
        if( rate < 0.0 || rate > 1.0 )
        {
            this->logger.print( "Warning: in LSystemWorldObject::create, GrowthRate given is out of range, should be in [0,1], defaulting to: " + Utility::toString( this->growthRate ) );
            this->logger.print( "Warning: entry was: " + growthRateConfig );
        }
        else
        {
            this->growthRate = rate;
        }
    }
    string physicsUpdateRateConfig = this->interpreter->getAttributeValue( "PhysicsUpdateRate" );
    if( physicsUpdateRateConfig != "" )
    {
        bool success = false;
        double rate = Utility::stringTo<double>( physicsUpdateRateConfig, success );
        if( !success )
        {
            this->logger.print( "Warning: in LSystemWorldObject::create, PhysicsUpdateRate not given as a number in the config file, defaulting to: " + Utility::toString( this->growthRate ) );
            this->logger.print( "Warning: entry was: " + physicsUpdateRateConfig );
        }
        if( rate < 0.0 )
        {
            this->logger.print( "Warning: in LSystemWorldObject::create, PhysicsUpdateRate < 0, defaulting to: " + Utility::toString( this->growthRate ) );
        }
        else
        {
            this->physicsUpdateRate = rate;
        }
    }

    //this line would seem to be useless.
    //yet it would seem that we would not want to ignore either the start point in the config file or
    //the passed arguments. Wtf was I thinking here? Maybe need some flag to use config file or args.
    //vector<double> pos = Utility::split<double>( this->interpreter->getAttributeValue( "StartPoint" ) );

    string defaultMeshName = this->interpreter->getAttributeValue( "Mesh" );
    Entity* ent = NULL;
    double legx, legy, legz;
    string tmpMeshName = "tmpBoundsDefaultMeshObject.tmp";
    ostringstream tmpostr;

    if( this->sceneManager == NULL )
    {
        this->logger.print( "Warning: in LSystemWorldObject::create(), sceneManager is null" );
        return false;
    }
    ent = this->sceneManager->createEntity( tmpMeshName, defaultMeshName );
    //currently we use legy, because that is the local mesh axis 
    //that we align to our interpreter movement vector.
    Utility::getAABBLegs( ent, legx, legy, legz );
    this->sceneManager->destroyEntity( tmpMeshName );
    tmpostr.clear();
    tmpostr << legy;
    this->interpreter->setAttribute( "AtomicSize", tmpostr.str() );

    tmpostr.clear();
    tmpostr << posx << "," << posy << "," << posz;
    this->interpreter->setAttribute( "StartPoint", tmpostr.str() );
    string meshMap = this->interpreter->getAttributeValue( "MeshMap" );
    //need to add joint type to this at some point
    if( meshMap != "" )
    {
        char_separator<char> sep(",");
        tokenizer< char_separator<char> > tokens( meshMap, sep );
        tokenizer< char_separator<char> >::const_iterator i = tokens.begin();
        string symbol = "";
        if( i != tokens.end() )
        {
            symbol = *i; 
        }
        ++i;
        string mesh = "";
        if( i != tokens.end() )
        {
            mesh = *i;
        }
        ++i;
        string geometry = "";
        if( i != tokens.end() )
        {
            geometry = *i;
        }
        //here we populate our map, and set up the interpreter with
        //the correct step size for our mesh.
        if( geometry != "" && mesh != "" && symbol != "" )
        {
            this->meshMap[symbol] = std::pair<string,string>( mesh, geometry );

            string tmpname = mesh + "_tmp_bounds_check";
            Entity* ent = NULL;
            ent = this->sceneManager->createEntity( tmpname, mesh );
            //currently we use legy, because that is the local mesh axis 
            //that we align to our interpreter movement vector.
            Utility::getAABBLegs( ent, legx, legy, legz );
            this->sceneManager->destroyEntity( tmpname );
            this->incInterpreter->setElementSize( symbol, legy );
        }
        else
        {
            logger.print( "Warning: in LSystemWorldObject::create() bad mesh map: " + meshMap );
            return false;
        }
    }
    //we have to make sure this is set to a valid type
    if( !this->setJointType( this->interpreter->getAttributeValue( "JointType" ) ) )
    {
        this->jointType = "ball";
    }
    return true;
}

bool LSystemWorldObject::update()
{
    logger.print( "Notify: entered LSystemWorldObject::update()", this->doNotify );
    if( !this->physics || !this->graphics )
    {
        return true;
    }

    time_t physicsUpdateDelta = time( NULL ) - this->lastPhysicsUpdate;
    //if the physics updateRate is > 1 we simply go as fast as we can. (ignoring the delta check)
    if( this->physicsUpdateRate <= 1 && (double)(physicsUpdateDelta) * this->physicsUpdateRate < 1 )
    {
        return true;
    }

    vector<double> pos;
    bool status = false;        
    bool ret = true;
    for( map<string,SceneNode*>::const_iterator i = this->sceneNodes.begin(); i != this->sceneNodes.end(); ++i )
    {
        pos.clear();
        status = this->physicsEngine->getPosition( (*i).first, pos );
        logger.print( "Warning: LSystemWorldObject::update() failed to getPostion from physicsEngine for item: " + (*i).first, !status );
        if( status )
        {
            (*i).second->setPosition( pos[0], pos[1], pos[2] );
        }
        ret = ret && status;//if any of these fails, return false
    }
    this->lastPhysicsUpdate = time( NULL );
    return ret;
}

bool LSystemWorldObject::grow()
{
    logger.print( "Notify: entered LSystemWorldObject::grow()", this->doNotify );
    time_t updateDelta = time( NULL ) - this->lastUpdateTime;
    //e.g. 1 sec pass, 1 update per second, we should update
    //     2 sec pass, .5 updates per sec...
    if( (double)(updateDelta) * this->growthRate < 1 )
    {
        //we still need to update physics, etc.
        this->update();
        return true;
    }
    LSystemIncrementalInterpreter* tmp = dynamic_cast<LSystemIncrementalInterpreter*>(this->interpreter);
    if( tmp == NULL )
    {
        logger.print( "Warnning: bad interpreter in LSystemWorldObject" );
        this->update();
        return false;
    }

    vector<double> point;
    //note added stop condition to not bother when we have no physics...
    for( vector< std::pair< string, unsigned int > >::const_iterator i = this->objectNames.begin(); this->physics && i != this->objectNames.end(); ++i )
    {
        point.clear(); //getPosition simply appends, so we need to clear()
        if( !this->physicsEngine->getPosition( (*i).first, point ) )
        {
            logger.print( "Warning: in LSystemWorldObject, failed to retrieve physics item: " + (*i).first + " for interpreter update" );
        }
        else
        {
            if( !tmp->updatePoint( (*i).second, point ) )
            {
                logger.print( "Warning: in LSystemWorldObject, failed to update interpreter point: " + Utility::toString( (*i).second ) + " from physics item: " + (*i).first );
            }
        }
    }
    vector<double> newPoint;
    vector<double> prevPoint;
    unsigned int currentIndex = 0;
    unsigned int prevIndex = 0;
    bool success = true;
    //grow CAN fail.
    while( newPoint.size() < 3  && success ) // "["
    {
        prevPoint.clear();
        newPoint.clear();
        tmp->prev( prevPoint, prevIndex ); //keep grabbing the previous point, in case we pop out of scope. 
        success = tmp->grow( newPoint, currentIndex );
    }
    string query = "";

    //don't mutate while we try to figure out issues
    //ok enough issues gone, try again :)
    this->system->run( string("mutate"), query );
    
    if( !success )
    {
        //try growing the system a little, then...
        this->system->run( string("grow"), query );
        this->update();
        return false;
    }
    string elementName = "";

    //prevIndex should always be valid, since we start with one pre-existing element
    //in the interpreter by default, even before the first "grow" call.
    this->makeElement( prevPoint, prevIndex, newPoint, currentIndex, elementName );
    this->update();
    this->lastUpdateTime = time( NULL );
    return true;
}

bool LSystemWorldObject::noPhysics()
{
    logger.print( "Notify: entered LsystemWorldObject:noPhysics()", this->doNotify );
    bool ret = true;
    for( map<string,SceneNode*>::const_iterator i =  this->sceneNodes.begin(); i != this->sceneNodes.end(); ++i )
    {
        if( !this->physicsEngine->removeItem( (*i).first ) )
        {
            logger.print( "Warning: in LSystemWorldObject::noPhysics(), failed to remove item: " + (*i).first + " from lsystem " );
            ret = false;
        }
    }
    this->physics = false;
    return ret;
}

bool LSystemWorldObject::setPosition( const vector<double>& pos )
{
    logger.print( "Warning: in LSystemWorldObject::setPosition, method has a half-ass implementation that only sets the start point... (should update the whole point chain. *sigh*" );

    if( pos.size() < 3 )
    {
        logger.print( "Warning: in LSystemWordObject::setPosition, passed a position vector with less than three coordinates, not even bothering with the current half-assed implementation. Come on people" );
        return false;
    }
    ostringstream point;
    point << pos[0] << "," << pos[1] << "," << pos[2];
    this->interpreter->setAttribute( "StartPoint", point.str() );

    return true;
}

bool LSystemWorldObject::setJointType( const string& jointType )
{
    logger.print( "Notify: in LSystemWorldObject::setJointType", this->doNotify );

    if( jointType == "ball" || jointType == "slider" || jointType == "fixed" || jointType == "hinge" || jointType == "universal" )
    {
        this->jointType = jointType;
        return true;
    }

    logger.print( "Warning: in LSystemWorldObject::setJointType attempted to set joint type to: " + jointType + " which is not a supported type" );
    return false;
}

bool LSystemWorldObject::setGrowthRate( double rate )
{
    if( rate < 0.0 )
    {
        logger.print( "Warning: in LSystemWorldObject::setGrowthRate, attempted to set a negative growth rate, which doesn't make sense" );
        return false;
    }

    if( rate > 1.0 )
    {
        logger.print( "Warning: in LSystemWorldObject::setGRowthRate, attempted to set a rate > 1, which is too fast" );
        return false;
    }

    this->growthRate = rate;
    return true;
}

bool LSystemWorldObject::setPhysicsUpdateRate( double rate )
{
    if( rate < 0.0 )
    {
        logger.print( "Warning: in LSystemWorldObject::setGrowthRate, attempted to set a negative physics update rate, which doesn't make sense" );
        return false;
    }

    this->physicsUpdateRate = rate;
    return true;
}

bool LSystemWorldObject::makeElement( const vector<double>& prevPos, unsigned int prevIndex, const vector<double>& pos, unsigned int index, string& name )
{
    logger.print( "Notify: entered LSystemWorldObject::makeElement()", this->doNotify );
    if( pos.size() < 3 )
    {
        logger.print( "Warning: in LSystemWorldObject, bad position vector (to small)" );
        logger.print( Utility::vectorToString( pos ) );
        return false;
    }
    string prevName = this->name + ":" + Utility::toString( prevIndex ) + ":" + Utility::toString( this->objectId );
    name = this->name + ":" + Utility::toString( index ) + ":" + Utility::toString( this->objectId );     

    string lSystemElement;
    if( !this->incInterpreter->getElement( lSystemElement, index ) )
    {
        logger.print( "Warning: in LSystemWorldObject::makeElement, bad index: " + lexical_cast<string>(index) + " did not map to an element" );
        return false;
    }

    Entity* ent = NULL;
    SceneNode* sceneNode = NULL;
    if( this->meshMap.find( lSystemElement ) != this->meshMap.end() )
    {
        string meshName = this->meshMap[lSystemElement].first;
        ent = this->sceneManager->createEntity( name, meshName );
        sceneNode = this->sceneManager->getRootSceneNode()->createChildSceneNode( name + "Node" );
        sceneNode->attachObject( ent );
        vector<double> currentDirection;

    }
    else
    {
        ent = this->sceneManager->createEntity( name, this->interpreter->getAttributeValue( "Mesh" ) );
        sceneNode = this->sceneManager->getRootSceneNode()->createChildSceneNode( name + "Node" );
        sceneNode->attachObject( ent );
    }

    
    if( this->physics )
    {
        //Box doesn't work right (stuff falls through the floor... ). Argh.
        //Also, joints don't work either...
        /*
        double legx, legy, legz;
        Utility::getAABBLegs( sceneNode, name, legx, legy, legz );
        double max = Utility::max( legx, legy );
        max = Utility::max( max, legz );
        vector<double> physicsPos;
        physicsPos.resize(3);
        physicsPos[0] = legx / 2;
        physicsPos[1] = legy / 2;
        physicsPos[2] = legz / 2;
        this->physicsEngine->addItem( name, physicsPos, legx, legy, legz );
        */
        double radius = sceneNode->getAttachedObject( name )->getBoundingRadius(); 
        this->physicsEngine->addItem( name, pos, radius );
    }

    vector<double> currentDirection;
    if( !this->incInterpreter->getCurrentDirection( currentDirection ) )
    {
        logger.print( "Warning: in LSystemWorldObject::makeElement, failed to get the current direction of movement" );
        return false;
    }
    else
    {
        sceneNode->setDirection( currentDirection[0], currentDirection[1], currentDirection[2], SceneNode::TS_WORLD, Vector3::UNIT_Y );
    }

    sceneNode->setPosition( pos[0], pos[1], pos[2] );
    this->sceneNodes[name] = sceneNode;

    this->objectNames.push_back( std::pair<string,unsigned int>(name,index) );

    if( this->physics )
    {
        JointProperties jointProperties;
        jointProperties.axis.resize(3);
        if( pos.size() < 3 || prevPos.size() < 3 )
        {
            logger.print( "Warning: in LSystemWorldObject::makeElement, pos or prev pos is < 3, pos: " + Utility::toString( pos.size() ) + " prevPos: " + Utility::toString( prevPos.size() ) );
            return false;
        }

        for( unsigned int i = 0; i < 3; ++i ) { jointProperties.axis[i] = pos[i] - prevPos[i]; }
        Matrix::midpoint( jointProperties.anchor, pos, prevPos );
        /*
        double radius = sceneNode->getAttachedObject( name )->getBoundingRadius(); 
        jointProperties.anchor = pos;
        for( unsigned int i = 0; i < 3; ++i ){ jointProperties.anchor[i] += radius * 2; }
        */
        this->physicsEngine->connectItems( prevName, name, this->jointType, jointProperties );
    }

    return true;
}

WorldObject* LSystemWorldObject::breed( WorldObject* parent2, unsigned int swappiness  )
{
    logger.print( "Notify: entered LSystemWorldObject::breed", this->doNotify );
    if( parent2 == NULL )
    {
        logger.print( "Warning: in LSystemWorldObject::breed, parent2 is null" );
        return NULL;
    }
    if( this->system == NULL )
    {
        logger.print( "Warning: in LSystemWorldObject::breed, system is null" );
        return NULL;
    }

    if( typeid( *this ) != typeid( LSystemWorldObject ) )
    {
        logger.print( "Warning: in LSystemWorldObject::breed, called this method from a derived class with no breed implemenation" );
        logger.print( "Warning: tell the author to fix their code! Refusing to breed." );
        return NULL;
    }

    WorldObject* child = new LSystemWorldObject( this->name, this->sceneManager, this->physicsEngine );
    *child = *this;
    //positions should be randomized offset from parent...
    //Note this is currently reset to start point in create() call in makeChild()...
    //need to remove that when we shift to random model..
    double posx = 0;
    double posy = 0;
    double posz = 0;
    if( !child->makeChild( this->name, posx, posy, posz ) )
    {
        //mem leak until smart ptrs in parent obj.
        return false;
    }
    if( swappiness == 0 )
    {
        return child;
    }
    if( !this->cross( child, parent2, swappiness ) )
    {
        logger.print( "Warning: in LSystemWorldObject::breed(), cross failed" );
    }
    return child;
}

bool LSystemWorldObject::makeChild( const string& name, double posx, double posy, double posz )
{
    if( typeid( *this ) != typeid( LSystemWorldObject ))
    {
        logger.print( "In LSystemWorldObject::makeChild, called method on child type, this is no good, refusing to spawn..." );
        return false;
    }
    //unique name is different, same resource though.
    this->name = name;
    //give the child a *new* system that is a copy of the parents
    //(don't just point it at the parents)
    System* tmp = NULL; //yes, you need to do it this way!
    this->system->getCopy( tmp );
    this->system = tmp;
    this->interpreter = NULL; //points at parents interpreter, we don't want that, and we don't want to delete the parents either...
    //just use new interpreter made in create()
    this->create( this->resourceName, posx, posy, posz );
    return true;
}


#endif
