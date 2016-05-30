#ifndef STU_PHYSICS_ENGINE_H
#define STU_PHYSICS_ENGINE_H

#include <map>
#include <string>
#include <vector>

#include <ode/ode.h>

#include "CommonAspect.h"
#include "Utility.h"
#include "ReadWriteLock.h"
#include "Timer.h"

using namespace std;
using namespace evolve;

namespace evolve
{

struct JointProperties
{
    vector<double> anchor;
    vector<double> axis;
};

/*
 * callback for ODE. friend of PhysicsEngine. _this is meant to be 
 * a this pointer at the PhysicsEngine object currently using the callback.
 */
void nearCollisionCallback( void* _this, dGeomID id1, dGeomID id2 );
/**
 * main physics thread. Do not call this directly! Call PhysicsEngine::startPhysics()
 */
void* physicsThread( void* _this );

/**
 * This class provides an interface to our physics engine.
 * Currently, it just encapsulates ODE, and therefore the 
 * user is highly recommended to read over the ODE docs & examples in 
 * order to understand stuff like joint types and joint properties.
 *
 * It also provides a threaded interface to the physics library via the 
 * startPhysics() call. doPhysics is provided for debugging purposes only..
 *
 * NOTE! READ! LOOK HERE BUDDY!
 * ODE does NOT like too many overlapping bodies (or really any). Tends to just
 * seg fault under such circumstances. It's highly recommended that you set up
 * you're objects, and move them _away_ from each other (just enough so they don't
 * overlap) before you call startPhyics(). Hey, I warned you buddy!
 */
class PhysicsEngine : public CommonAspect
{
public:
    PhysicsEngine();
    virtual ~PhysicsEngine() 
    { 
        this->running = false; 
        this->engineLock.writeLock();
        dWorldDestroy( this->worldId ); 
        dCloseODE(); 
        this->engineLock.unlock();
    }
    /**
     * Add an dynamic item to the engine. A dymanic item 
     * will move, and have it's position updated.
     * @param name the id of the item in the engine
     * @param pos the center of the objects bounding sphere (vector of size 3)
     * @param radius the radius of the objects bounding sphere
     */
    bool addItem( const string& name, const vector<double>& pos, double radius );
    bool addItem( const string& name, const vector<double>& pos, double edgex, double edgey, double edgez );

    /**
     * Add a static item to the engine. A static item
     * is an immovable part of the environment (like the ground).
     * @param name the id of the item in the engine
     * @param pos the position of the item (the center of the box, not the corner)
     * @param edgex the edge defining the bounding box of the static item 
     * @param edgey ibid
     * @param edgez ibid
     */
    bool addStaticItem( const string& name, const vector<double>& pos, double edgex, double edgey, double edgez );
    bool addStaticItem( const string& name, const vector<double>& pos, double radius );

    /**
     * Connect two items together, with the give joint type & properties.
     * Both items must already exist in the engine
     */
    bool connectItems( const string& item1, const string& item2, const string& jointType, const JointProperties& jointProperties );

    bool removeItem( const string& name );

    bool setGravity( double x, double y, double z );
    bool setPosition( const string& name, const vector<double>& position );
    bool setPosition( const string& name, double x, double y, double z );
    bool setMass( const string& name, double mass );
    bool setLinearVelocity( const string& name, const vector<double>& velocity );
    /*
     * Returns the position of an object (can be dynamic or static)
     * @param name the name of the object
     * @param pos the position of the object. 
     *        The position is appended to the end of the vector.
     */
    bool getPosition( const string& name, vector<double>& pos );
    bool getGravity( vector<double>& grav );

    /**
     * Statefully steps the physics engine.
     * Deprecated/Debugging. Physics should be done asynchronously now via startPhysics()
     */
    bool doPhysics();
    
    /**
     * Starts the physics thread. This will do safety check & then launch the physics thread.
     * Repeatedly calling this does nothing extra, except to generate warnings.
     */
    bool startPhysics();

    /*
     * callback for ODE. friend of PhysicsEngine. _this is assumed to be 
     * a this pointer at the PhysicsEngine object currently using the callback.
     * Hmm. you can have some pointer overflow fun with this.
     */
    friend void nearCollisionCallback( void* _this, dGeomID id1, dGeomID id2 );
    friend void* physicsThread( void* _this );

protected:
    //the basic idea is that a lot of what we do is read-only operations, where
    //we don't care greatly about reading behind or ahead
    //So we only lock when we do write operations (i.e. call into ODE with some state change).
    //should be ok.
    ReadWriteLock engineLock;
    bool running;
    bool threaded;
    //used to track amount of time last spent in collision
    //and when we are going slow (and need to start returning cached
    //positions, etc)
    long long lastCollisionTime;
    bool goingSlow;
    map<string,dBodyID> bodies;
    //this maps a body id to the magnitude of the last update
    //and the time of the last update. Used to provide a quick
    //(and slightly wrong) return from getPosition() that does
    //not require the readLock()
    //Need to track whether we actually use this 
    map< string,pair<double,Timer> > deltas;
    //this caches the last position of the body
    //(see above)
    map< string,vector<double> > lastPos;
    //how often we actually use the cache
    int numCacheReturns;
    //associate the string id of each body with a list 
    //of joints attached to it.
    map<string,vector<dJointID> > joints;
    //Pretty much these are stored just so we can remove them later if we want
    map<string,dGeomID> geometries; 
    dWorldID worldId;
    dSpaceID spaceId;
    dJointGroupID contactJointGroup;
};
};
#endif
