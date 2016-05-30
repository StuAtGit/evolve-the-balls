#ifndef STU_PHYSICS_H
#define STU_PHYSICS_H

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <ode/ode.h>
#include "Logger.h"
#include "Timer.h"

#define DO_DEBUG true

using namespace std;

/**
Handles the physics for a set of systems.
When a system is added, it is only added up to
the point where one of the systems elements collide
with another element. (You are not allowed to add
elements that will collide with existing elements)\n
Unless otherwise noted the printDebug argument sets the debug flag (true = on, false = off)
for the method passed to in.\n
The setNotify and setLogfileName sets the notify flag, and logging file name. (see Logger.h)
*/
class Physics
{
public:
    Physics();
    /**
    Adds an unbranched system, with a separate radius for each object
    @param system the outer vector is the sytem, the inner represents a point
    @param radii a vector of radii, radii.size() must equal system.size()
    */
    unsigned int addSystem( vector< vector< double > > system, vector< double > radii, bool printDebug = DO_DEBUG );
    /**
    Adds an unbranched system, with one radius for all objects
    @param system the outer vector is the sytem, the inner represents a point
    @radius the radius to use for every element in the system (for collision detection)
    */
    unsigned int addSystem( vector< vector< double > > system, double radius, bool printDebug = DO_DEBUG );
    /**
    *@param system a system with branches. The map is a system of branches indexed according
    *       to their place in the hierarchy. The system is the vector, the innermost vector is the point.
    *@param radius the radius to use for collision detection for each element in the system.  
    * */
    map<int, unsigned int> addSystem( const map< int, vector< vector< double > > >& system, double radius, bool printDebug = DO_DEBUG );
    void setLogfileName( string logFileName, bool printDebug = DO_DEBUG );
    void setNotify( bool notify, bool printDebug = DO_DEBUG );

private: 
    //members
    dWorldID worldId;
    dSpaceID spaceId;
    dJointGroup theJointGroup;
    vector<dMass> masses;
    vector<dBodyID> bodies;
    vector<dGeomID> geometries;
    Logger logger;
    Timer timer;
    unsigned long totalCollisionTime;
    unsigned long totalCreateBodyTime;
    bool doNotify;
};

#endif
