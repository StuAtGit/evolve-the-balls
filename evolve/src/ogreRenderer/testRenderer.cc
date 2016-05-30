#include <iostream>
#include <string>
#include <map>

#include "Ogre.h"
#include "OgreException.h"
#include "OgreRenderer.h"

#include "WorldObject.h"
#include "StaticWorldObject.h"
#include "LSystemWorldObject.h"
#include "Utility.h"

using namespace std;
using namespace Ogre;

int main( int argc, char** argv )
{
    unsigned int numNinjas = 1;
    double grav = -9.8;
    if( argc > 1 )
    {
        bool success = false;
        numNinjas = Utility::stringTo<unsigned int>( string( argv[1] ), success );
        if( !success )
        {
            cout << "argument: " << argv[1] << " was not an unsigned int, please try again.." << endl;
            return -1;
        }

        if( numNinjas > 3000 )
        {
            cout << "hehe, that's a lot of Ninjas." << endl;
        }

        if( numNinjas > 5000 )
        {
            cout << "Wow, you really like Ninjas! Too much. Goodbye" << endl;
            return 0;
        }

        if( argc > 2 )
        {
            bool success = false;
            grav = Utility::stringTo<double>( string( argv[2] ), success );
            if( !success )
            {
                cout << "argument 2: " << argv[2] << " could not be converted to a double. Defaulting gravity setting to -9.8" << endl;
                grav = -9.8;
            }
            else if( grav > 0 )
            {
                cout << "Gravity argument: " << argv[2] << " was positive. Most people would expect it to be negative (do you fall up?). Just making sure." << endl;
            }
        }
    }
    OgreRenderer r;
    r.setGravity( Utility::makeVector( 0.0, grav, 0.0 ) );
    map<string, WorldObject*> worldObjects;
    vector<double> pos;
    for( unsigned int i = 0; i < numNinjas; ++i )
    {
        //Put too many objects overlapping in the same space, and ODE has a fit.. (or is it OGRE?) Probably ODE
        pos = Utility::makeVector( i * 2000.0, 240.0, -500.0 );
        worldObjects["serpent" + Utility::toString(i)] = new LSystemWorldObject( string("serpent"), r.getSceneManager(), r.getPhysicsEngine() );
        worldObjects["serpent" + Utility::toString(i)]->create( "cube.mesh", pos[0], pos[1], pos[2] );
    }

    //make children
    for( unsigned int i = 0; i < numNinjas / 2; i += 2 )
    {
        LSystemWorldObject* parent2 = dynamic_cast<LSystemWorldObject*>(worldObjects["serpent" + Utility::toString(i+1)]);
        if( parent2 != NULL )
        {
            pos = Utility::makeVector( i * 2000.0, 240.0, -500.0 );
            worldObjects["serpent" + Utility::toString( i + numNinjas )] = worldObjects["serpent" + Utility::toString(i)]->breed( parent2, 10 );
            //we just went ahead and did a half-baked impl. of setPosition.
            worldObjects["serpent" + Utility::toString( i + numNinjas )]->setPosition( pos );
            cout << "made child... " << i << endl;
            //setPosition for LSystemWOrldObject not implemented yet..
            //worldObjects["serpent" + Utility::toString( i + numNinjas)]->setPosition..
        }
        else
        {
            cout << "Hmm... failed to cast wrld object to lsystem object.." << endl;
        }
    }

    bool doPhysics = true;

    for( map<string,WorldObject*>::iterator i = worldObjects.begin(); !doPhysics && i != worldObjects.end(); ++i )
    {
        (*i).second->noPhysics();
    }

    /** 
     * this should really be a new type of world object... (and you just specify width/length/location
     * but at some point we must expose OGRE to user of this object, or implement a lot of wrappers 
     * Keep it simple for now, and if someone wants advanced, they need to learn how to create resources in OGRE..
    **/

    Plane ground( Vector3::UNIT_Y, 0 );
    MeshManager::getSingleton().createPlane( "ground",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        ground, 150000, 150000, 20, 20, true, 1, 5, 5, Vector3::UNIT_Z ); 
    worldObjects["ground"] = new StaticWorldObject( "the ground", r.getSceneManager(), r.getPhysicsEngine() );
    worldObjects["ground"]->create( "ground" );

    Plane wall1( Vector3::UNIT_X, 0 );
    MeshManager::getSingleton().createPlane( "wall1",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        wall1, 15000, 15000, 25, 25, true, 1, 5, 5, Vector3::UNIT_Z ); 

    worldObjects["wall1"] = new StaticWorldObject( "wall1", r.getSceneManager(), r.getPhysicsEngine() );
    worldObjects["wall1"]->create( "wall1", -7000, 0, 0 );

    Plane wall2( (Vector3::UNIT_X * -1), 0 );
    MeshManager::getSingleton().createPlane( "wall2",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        wall2, 15000, 15000, 25, 25, true, 1, 5, 5, Vector3::UNIT_Z ); 

    worldObjects["wall2"] = new StaticWorldObject( "wall2", r.getSceneManager(), r.getPhysicsEngine() );
    worldObjects["wall2"]->create( "wall2", 7000, 0, 0 );

    Plane wall3( Vector3::UNIT_Z, 0 );
    MeshManager::getSingleton().createPlane( "wall3",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        wall3, 15000, 15000, 25, 25, true, 1, 5, 5, Vector3::UNIT_Y ); 

    worldObjects["wall3"] = new StaticWorldObject( "wall3", r.getSceneManager(), r.getPhysicsEngine() );
    worldObjects["wall3"]->create( "wall3", 0, 0, -7000 );

    Plane wall4( (Vector3::UNIT_Z * -1), 0 );
    MeshManager::getSingleton().createPlane( "wall4",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        wall4, 15000, 15000, 25, 25, true, 1, 5, 5, Vector3::UNIT_Y ); 

    worldObjects["wall4"] = new StaticWorldObject( "wall4", r.getSceneManager(), r.getPhysicsEngine() );
    worldObjects["wall4"]->create( "wall4", 0, 0, 7000 );


    r.createScene( worldObjects );

    //really really need a config file for testRenderer...
    double speed = 0.5;
    if( !doPhysics )
    {
        speed /= 2;
    }
    const StandardCamera* cam = r.getFrame()->getCamera();
    const_cast<StandardCamera*>(cam)->setSpeed( speed );
    if( doPhysics )
    {
        r.getPhysicsEngine()->startPhysics();
    }
    r.run();

    return 0;
}
