#ifndef STATIC_SPHERE_OBJECT_CC_STU
#define STATIC_SPHERE_OBJECT_CC_STU

#include "StaticSphereObject.h"

bool StaticSphereObject::create( const string& resourceName, double posx, double posy, double posz )
{
    this->logger.print( "Notify: entered StaticSphereObject::create ", this->doNotify );
    this->graphics = true;
    this->resourceName = resourceName;
    Entity* ent = this->sceneManager->createEntity( this->name, resourceName );
    this->sceneNode = this->sceneManager->getRootSceneNode()->createChildSceneNode( this->name + "Node" );
    this->sceneNode->attachObject( ent );
    this->sceneNode->setPosition( posx, posy, posz);
    double radius = this->sceneNode->getAttachedObject( this->name )->getBoundingRadius();
    this->physicsEngine->addStaticItem( this->name, Utility::makeVector( posx, posy, posz ), radius  );
    return true;
}

#endif
