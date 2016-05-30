#ifndef STATIC_WORLD_OBJECT_CC_STU
#define STATIC_WORLD_OBJECT_CC_STU

#include "StaticWorldObject.h"

bool StaticWorldObject::create( const string& resourceName, double posx, double posy, double posz )
{
    this->logger.print( "Notify: entered StaticWorldObject::create ", this->doNotify );
    this->graphics = true;
    this->resourceName = resourceName;
    Entity* ent = this->sceneManager->createEntity( this->name, resourceName );
    this->sceneNode = this->sceneManager->getRootSceneNode()->createChildSceneNode( this->name + "Node" );
    this->sceneNode->attachObject( ent );
    this->sceneNode->setPosition( posx, posy, posz );
    AxisAlignedBox box = this->sceneNode->getAttachedObject( this->name )->getBoundingBox();
    Vector3 flb = box.getCorner( AxisAlignedBox::FAR_LEFT_BOTTOM );
    Vector3 frb = box.getCorner( AxisAlignedBox::FAR_RIGHT_BOTTOM );
    Vector3 nlb = box.getCorner( AxisAlignedBox::NEAR_LEFT_BOTTOM );
    Vector3 nlt = box.getCorner( AxisAlignedBox::NEAR_LEFT_TOP );
    double legx = ( flb - frb ).length();
    double legy = ( nlt - nlb ).length();
    double legz = ( nlb - flb ).length();
    this->physicsEngine->addStaticItem( this->name, Utility::makeVector<double>( posx, posy, posz ), legx, legy, legz );
    return true;

}

#endif
