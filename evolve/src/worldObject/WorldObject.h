#ifndef WORLD_OBJECT_STU_H
#define WORLD_OBJECT_STU_H

#include <string>
#include <vector>
#include <map>
#include <ostream>

#include "Ogre.h"
#include "OgreException.h"

#include "Utility.h"
#include "CommonAspect.h"
#include "PhysicsEngine.h"
#include "WorldObject.h"
#include "Interpreter.h"
#include "System.h"

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
/**
 * And object that glues together the physics of an object
 * with it's geometry. The link between ODE and OGRE. 
 * However, stepping the physical world should be done elsewhere.
 * Be sure to call update()!
 *
 * Hmm.. we may add some interface to this to determine the bounding box.
 * Could use inheritance and override create(), but that may be to much of a pita.
 *
 * Also, may want to add system member into this sooner rather than later.
 * This would argue for an LSystemRemote class though...
 */
class WorldObject : public CommonAspect
{
  public:
    /**
     * Initialize a world object at 0,0,0
     * Does not actually create the object in the world. 
     * @param name The name used to identify this object
     *             This is not the same as the name of the resource
     *             used to create thsi object
     *             Also, the scene node of this object will be named name + "Node".
     *            (unless this behavior is overidden)
     * @param root Ogre root object
     * @param physicsEngine 
     */
    WorldObject( const string& name, SceneManager* sceneManager, PhysicsEngine* physicsEngine );
    virtual ~WorldObject()
    { 
        if( this->interpreter != NULL ) delete this->interpreter; 
        if( this->system != NULL ) delete this->system; 
    }
    /**
     * This method is overriden by child objects to 
     * provide the ability for object to determine
     * it's own geometry (vs. by loading a resource, as below)
     * e.g. SphereObject : public WorldObject would use this to make 
     *                     a sphere.
     */
    virtual bool create() { logger.print("Error: WorldObject::create() invalid for this object. Use create( resourceName )" ); return false; }
    /**
     * Creates the geometry of the object from a given resource
     * Returns false if this object should use a create(), and 
     * this does not apply.
     * @param resourceName The name of the resource.
     *        This is given in terms of Ogre resources (e.g. "ninja.mesh")
     *        This is not the same as the name of this object, which 
     *        is maintained separately
     * @param posx The default x position of this object
     * @param posy Ibid.
     * @param posz Ibid.
     */
    virtual bool create( const string& resourceName, double posx = 0, double posy = 0, double posz = 0 ); 
    /**
     * Set the position of the object.
     * Use with care, as constantly resetting the position of the object
     * may mess with the physics.
     */
    virtual bool setPosition( const vector<double>& pos );
    virtual bool setLinearVelocity( const vector<double>& velocity );
    virtual bool setMass( double mass );
    /** saved for later.
     *  physcis engine interface doesn't handle this yet.
     */
    virtual bool addForce() { logger.print("Error: WorldObject::addForce not implemented"); return false; }
    /**
     * syncs the graphical object to any changes in the physical object.
     * (e.g., updates the position of the graphical object in response to
     * any physics that occurred).
     * call this OR grow every frame, but not both.
     * */
    virtual bool update();
    /**
     * Allows us to extend update functionality, while still factoring out generic update code
     * (or just ignore the generic behavior altogether)
     * call this every frame.
     */
    virtual bool grow() { return this->update(); }
    /**
     * Creates a new object, with a system that is a cross between this one and the argument.
     * The object returned is always of the same type as the object whose method is called.
     * Attempting to call a parents breed() method on a child object will fail!
     * @param parent2 the other object to use in the cross
     * @param swappiness how much swapping to do between this object and the argument.
     *                   Not necessarily a direct probability. 0 means just return a copy of this object.
     * @returns the new object or NULL on failure (note, the cross of systems failing does not necessarily result in breed failure)
     */
    virtual WorldObject* breed( WorldObject* parent2, unsigned int swappiness );
    /**
     * Removes the object from the physical world but does not destroy
     * any other state. Useful for "storing" or "ghosting" them. 
     * calling create() after this call will add the physics state back in.
     */
    virtual bool noPhysics();
    virtual bool restartPhysics();
    /**
     * Inverse of noPhyiscs. Removes the graphical state from the world,
     * but continues to track the physics.
     * calling create() after this call will add the physics state back in.
     */
    virtual bool noGraphics();
    virtual bool restartGraphics();
    /**
     * Returns the sceneNode for this object.
     * Note that the name of this node will be this->name + "Node"
     * (unless overidden by child types)
     */
    virtual SceneNode* getSceneNode();
    virtual bool getName( string& name );
    /**
     * Gets the position of the object
     * Note that this will reset the position of the graphical
     * object to that of the physical object.
     * @param pos the position of the object.
     */
    virtual bool getPosition( vector<double>& pos );
    /**
     * Sets the interpreter for this object.
     * Obviously, for interpreted system objects only (objects linked to an interpreted system that grows over time)
     */
    virtual bool setInterpreter( Interpreter* interpreter );
    /**
     * Sets the system for this object.
     * Obviously, for growth object only (objects linked to a system that grows over time)
     */
    virtual bool setSystem( System* system );

    /**
     * Transforms this object into a child of "itself". You would call this on
     * an object that was generated via the vanilla copy constructor.
     * (creates unique names, resets interpreters/systems, etc) 
     * Will leak memory/do bad things if called on *non-child* (i.e. no copied)
     * object. (e.g. simply losing the interpreter pointer, assuming parent still has a handle to it..)
     * @param name the name to use to identify the object in the graphics & physics engines
     * @param posx the x position
     * @param posy ibid
     * @param posz ibid
     */
    virtual bool makeChild( const string& name, double posx, double posx, double posz );

  protected:
    //cross code can be factored out (for most cases). Object creation code cannot!
    bool cross( WorldObject* obj1, WorldObject* obj2, unsigned int swappiness );
    PhysicsEngine* physicsEngine;
    SceneNode* sceneNode;
    SceneManager* sceneManager;
    //these should be smart pointers
    Interpreter* interpreter;
    System* system;
    string name; 
    string resourceName;
    bool graphics; 
    bool physics;
};
};
#endif
