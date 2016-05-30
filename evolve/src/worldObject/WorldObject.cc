#ifndef WORLD_OBJECT_STU_CC
#define WORLD_OBJECT_STU_CC

#include "WorldObject.h"

WorldObject::WorldObject( const string& name, SceneManager* sceneManager, PhysicsEngine* physicsEngine )
{
    this->name = name; 
    this->resourceName = name;
    this->physics = this->graphics = true;
    this->sceneNode = NULL;
    this->interpreter = NULL;
    this->system = NULL;

    if( sceneManager == NULL )
    {
        logger.print( "Error: in WorldObject::WorldObject, sceneManager NULL" );
        return;
    }
    if( physicsEngine == NULL )
    {
        logger.print( "Error: in WorldObject::WorldObject, physics engine NULL" );
        return;
    }

    this->physicsEngine = physicsEngine;
    this->sceneManager = sceneManager;
}

bool WorldObject::create( const string& resourceName, double posx, double posy, double posz ) 
{ 
    this->logger.print( "Notify: entered WorldObject::create ", this->doNotify );
    this->graphics = true;
    this->resourceName = resourceName;
    this->name = resourceName; 
    Entity* ent = this->sceneManager->createEntity( this->name, resourceName );
    this->sceneNode = this->sceneManager->getRootSceneNode()->createChildSceneNode( this->name + "Node" );
    this->sceneNode->attachObject( ent );
    double radius = this->sceneNode->getAttachedObject( this->name )->getBoundingRadius(); 
    this->sceneNode->setPosition( posx, posy, posz );
    this->physicsEngine->addItem( this->name, Utility::makeVector<double>( posx, posy, posz ), radius );
    return true;
}

//Developer Note: be sure to update both the physics & the graphical object.
bool WorldObject::setPosition( const vector<double>& pos )
{
    if( pos.size() < 3 )
    {
        this->logger.print( "Warning: WorldObject::setPostion, passed a position with less than 3 coordinates" );
        return false;
    }
    bool ret = this->physicsEngine->setPosition( this->name, pos );
    if( ret )
    {
        this->sceneNode->setPosition( pos[0], pos[1], pos[2] );
    }
    return ret;
}

bool WorldObject::setLinearVelocity( const vector<double>& velocity )
{
    logger.print( "Notify: entered WorldObject::setLinearVelocity()", this->doNotify );
    return this->physicsEngine->setLinearVelocity( this->name, velocity ); 
}

bool WorldObject::setMass( double mass )
{
    logger.print( "Notify: entered WorldObject::setMass()", this->doNotify );
    return this->physicsEngine->setMass( this->name, mass );
}

bool WorldObject::update()
{
    logger.print( "Notify: entered WorldObject::update()", this->doNotify );
    if( !this->physics || !this->graphics )
    {
        return true;
    }

    vector<double> pos;
    bool ret = this->physicsEngine->getPosition( this->name, pos );
    logger.print( "Warning: WorldObject::update() failed to getPostion from physicsEngine", !ret );
    if( ret )
    {
        this->sceneNode->setPosition( pos[0], pos[1], pos[2] );
    }
    return ret;

}

WorldObject* WorldObject::breed( WorldObject* parent2, unsigned int swappiness  )
{
    logger.print( "Notify: entered WorldObject::breed", this->doNotify );
    if( parent2 == NULL )
    {
        logger.print( "Warning: in WorldObject::breed, parent2 is null" );
        return NULL;
    }
    if( this->system == NULL )
    {
        logger.print( "Warning: in WorldObject::breed, system is null" );
        return NULL;
    }

    if( typeid( *this ) != typeid( WorldObject ) )
    {
        logger.print( "Warning: in WorldObject::breed, called this method from a derived class with no breed implemenation" );
        logger.print( "Warning: tell the author to fix their code! Refusing to breed." );
        return NULL;
    }

    WorldObject* child = new WorldObject( this->name, this->sceneManager, this->physicsEngine );
    *child = *this;
    if( !child->makeChild( this->name + "Child", 0.0, 0.0, 0.0 ) )
    {
        //delete child; err.. this deletes the members that point at the same thing the parent does?
        //or one of them leaks :(
        //I need to use smart ptrs here...
        //check above should prevent this anywhoo...
        return false;
    }
    if( swappiness == 0 )
    {
        return child;
    }
    if( !this->cross( child, parent2, swappiness ) )
    {
        logger.print( "Warning: in WorldObject::breed(), cross failed" );
    }
    return child;
}

bool WorldObject::makeChild( const string& name, double posx, double posy, double posz )
{
    if( typeid( *this ) != typeid( WorldObject ))
    {
        logger.print( "In WorldObject::makeChild, called method on child type, this is no good, refusing to spawn..." );
        return false;
    }
    //unique name is different, same resource though.
    this->name = name;
    this->create( this->resourceName, posx, posy, posz );
    //give the child a *new* system that is a copy of the parents
    //(don't just point it at the parents)
    System* systemTmp = NULL; //yes, you need to do it this way!
    this->system->getCopy( systemTmp );
    this->system = systemTmp;
    //this is obviously invalid in the incremental interpreter case. OVERRIDE THERE!
    Interpreter* interpreterTmp = NULL; //yes, you need to do it this way!
    this->interpreter->getCopy( interpreterTmp ); 
    this->interpreter = interpreterTmp;
    return true;
}

bool WorldObject::cross( WorldObject* child, WorldObject* parent2, unsigned int swappiness )
{
    //any bigger than this is bound to be pretty pointless
    unsigned int maxSwappiness =  this->system->getSystem().length() * 2;
    if( swappiness >= maxSwappiness )
    {
        swappiness = maxSwappiness;
    }
    map<string,string> sysCrossMap;
    sysCrossMap["random:random"] = 3;
    map<string,string> optCrossMap;
    map<string,string> mutCrossMap;
    map<string,string> grthCrossMap;

    //only swap the system this many times, because the number of entries of the attributes are generally much smaller
    bool success = true;
    for( unsigned int i = 0; i < swappiness; ++i )
    {
        success = child->system->cross( parent2->system, sysCrossMap, optCrossMap, mutCrossMap, grthCrossMap );
        if( !success )
        {
            logger.print( "Warning: in WorldObject::cross, system cross: " + Utility::toString(i) + " failed " );
            return false;
        }
    }

    //do these much less
    optCrossMap["random:random"] = 3;
    mutCrossMap["ranom:random"] = 3;
    grthCrossMap["random:random"] = 3;
    sysCrossMap.clear();
    success = true;
    for( unsigned int i = 0; i < (swappiness / 100) + 1; ++i )
    {
        success = child->system->cross( parent2->system, sysCrossMap, optCrossMap, mutCrossMap, grthCrossMap );
        if( !success )
        {
            logger.print( "Warning: in WorldObject::cross, growth cross: " + Utility::toString(i) + " failed " );
            return false;
        }
    }
    
    return true;
}

bool WorldObject::noPhysics()
{
    if( !this->physicsEngine->removeItem( this->name ) )
    {
        this->logger.print( "Warning: failed to turn off physics for item: " + this->name );
        return false;
    }
    this->physics = false;
    return true;
}

bool WorldObject::restartPhysics()
{
    return (this->physics = true);
}

bool WorldObject::noGraphics()
{
    logger.print( "Notify: entered WorldObject::noGraphics()", this->doNotify );
    this->graphics = false;
    this->sceneManager->getRootSceneNode()->removeAndDestroyChild( this->name + "Node" );
    return true;
}

bool WorldObject::restartGraphics()
{
    logger.print( "Notify: entered WorldObject::restartGraphics()", this->doNotify );
    if( this->graphics )
    {
        return true;
    }
    else
    {
        return (this->graphics = this->create( this->resourceName ) );
    }
}

SceneNode* WorldObject::getSceneNode()
{
    return this->sceneNode;
}

bool WorldObject::getName( string& name )
{
    name = this->name;
    return true;
}

bool WorldObject::getPosition( vector<double>& pos )
{
    this->logger.print( "Notify: entered WorldObject::getPosition", this->doNotify );
    bool ret = this->physicsEngine->getPosition( this->name, pos );
    if( ret )
    {
        this->sceneNode->setPosition( pos[0], pos[1], pos[2] );
    }
    return ret;
}

bool WorldObject::setInterpreter( Interpreter* interpreter )
{
    this->interpreter = interpreter;
    return true;
}

bool WorldObject::setSystem( System* system )
{
    this->system = system;
    return true;
}

#endif
