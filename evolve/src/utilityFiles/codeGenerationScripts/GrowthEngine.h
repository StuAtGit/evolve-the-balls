#ifndef Stu_GrowthEngine_H
#define Stu_GrowthEngine_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "LSystem.h"
#include "Timer.h"
#include "Logger.h"

#define DO_DEBUG true

using namespace std;
using namespace boost;

namespace evolve
{
    /**A class that binds together graphical objects, interpreters, and systems*/
    class GrowthEngine
    {
    public:
        GrowthEngine();
        /**
        *
        */
        bool grow( unsigned int steps,bool printDebug = DO_DEBUG );
        /**
        *
        */
        bool addSystem( const string& id, const System& system,bool printDebug = DO_DEBUG );
        /**
        *
        */
        bool addInterpreter( const string& id, const Interpreter& interpreter,bool printDebug = DO_DEBUG );
        /**
        *
        */
        bool getSystem( const string& id, System& system,bool printDebug = DO_DEBUG );
        /**
        *
        */
        bool getInterpreter( const string& id, Interpreter& interpreter,bool printDebug = DO_DEBUG );
        /**
        *
        */
        bool addWorldObject( const string& id, const WorldObject& worldObject,bool printDebug = DO_DEBUG );
        /**
        *
        */
        bool getWorldObject( const string& id, WorldObject& worldObject,bool printDebug = DO_DEBUG );
    private:
        map<string, System&> systems;
        map<string, Interpreter&> interpreters;
        map<string, WorldObject&> worldObjects;
    };
};
#endif
