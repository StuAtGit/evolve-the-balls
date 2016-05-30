#ifndef LSYSTEM_WORLD_STU_H
#define LSYSTEM_WORLD_STU_H

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <time.h>

#include <boost/tokenizer.hpp>
#include "Ogre.h"
#include "OgreException.h"

#include "Utility.h"
#include "Matrix.h"
#include "CommonAspect.h"
#include "PhysicsEngine.h"
#include "WorldObject.h"
#include "LSystemGeneric.h"
#include "LSystemGenericInterpreter.h"
#include "LSystemIncrementalInterpreter.h"

using namespace std;
using namespace Ogre;
using namespace evolve;

#define setBackgroundColor setBackgroundColour
#define ColorValue ColourValue
#define initializeAllResourceGroups initialiseAllResourceGroups
#define setDiffuseColor setDiffuseColour
#define setSpecularColor setSpecularColour

namespace evolve
{
    class LSystemWorldObject : public WorldObject
    {
      public:
        LSystemWorldObject( const string& name, SceneManager* sceneManager, PhysicsEngine* physicsEngine ) : WorldObject( name, sceneManager, physicsEngine ){}
        virtual ~LSystemWorldObject() {};

        //overriden methods
        virtual bool create( const string& resourceName, double posx = 0, double posy = 0, double posx = 0 );
        virtual bool noPhysics();
        virtual bool update();
        virtual bool grow();
        virtual bool setPosition( const vector<double>& pos );
        virtual WorldObject* breed( WorldObject* parent2, unsigned int swappiness );
        //used to create unique names for ogre renderer
        //obviously, threading should be interesting...
        //(we would need to lock access to this at increment & 
        //assigning to local object id in constructor)
        static unsigned int objectCounter;
        //SEE NOTES is WOrldObject! (use with care)
        virtual bool makeChild( const string& name, double posx, double posy, double posz );
        virtual bool setJointType( const string& jointType );
        /**
         * Set the growth rate of the system, in steps per second.
         * Note that this will inherently be a maximum rate, since we can't ensure
         * a minimum in any reasonable fashion (with this system, at least).
         * @param rate the maximum rate of growth for the lsystem, in steps per second. Should be in the range [0,1].
         */
        virtual bool setGrowthRate( double rate );
        /**
         * Set how often physics will step & update the system.
         * In steps per second. If value is set > 1 it will be 
         * as fast as the system can manage.
         * @param rate the physics update rate. In steps per second.
         */
        virtual bool setPhysicsUpdateRate( double rate );

      protected:
        //utility pointer so we only have to cast once...
        LSystemIncrementalInterpreter* incInterpreter;
        virtual bool makeElement( const vector<double>& prevPos, unsigned int prevIndex, const vector<double>& pos, unsigned int index, string& name );
        map<string,SceneNode*> sceneNodes;
        //a list of the names we give components of this object in the physics engine
        //Used to coordinate physics with the interpreter
        //maps physics names -> interpreter indices
        vector< std::pair<string,unsigned int> > objectNames;
        //where we store our object id
        unsigned int objectId; 
        string jointType;
        //mapping of a system element type to a mesh and a bounding geometry type
        //Right now, just making it work for S->(column.mesh,box)
        map< string,std::pair<string,string> > meshMap;
        double growthRate;
        double physicsUpdateRate; 
        time_t lastUpdateTime;
        time_t lastPhysicsUpdate;
    };
};
#endif 
