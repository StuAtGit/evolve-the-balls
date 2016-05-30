#ifndef STATIC_WORLD_STU_H
#define STATIC_WORLD_STU_H

#include <string>
#include <vector>
#include <iostream>
#include <map>

#include "Ogre.h"
#include "OgreException.h"

#include "Utility.h"
#include "CommonAspect.h"
#include "PhysicsEngine.h"
#include "WorldObject.h"

using namespace std;
using namespace Ogre;
using namespace evolve;

#define setBackgroundColor setBackgroundColour
#define ColorValue ColourValue
#define initialize initialise
#define initializeAllResourceGroups initialiseAllResourceGroups
#define setDiffuseColor setDiffuseColour
#define setSpecularColor setSpecularColour

namespace evolve
{
    class StaticWorldObject : public WorldObject
    {
      public:
        StaticWorldObject( const string& name, SceneManager* sceneManager, PhysicsEngine* physicsEngine ) : WorldObject( name, sceneManager, physicsEngine ){}
        virtual ~StaticWorldObject() {};
        virtual bool create( const string& resourceName, double posx = 0, double posy = 0, double posx = 0 );
    };
};
#endif 
