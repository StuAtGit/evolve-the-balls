#include <string>
#include <vector>
#include <map>
#include <ostream>

#include "Ogre.h"
#include "OgreException.h"

#include "OgreGDKCodec.h"
#include "OISFrame.h"

#include "Utility.h"
#include "CommonAspect.h"
#include "PhysicsEngine.h"
#include "WorldObject.h"
#include "StaticWorldObject.h"


using namespace std;
using namespace Ogre;
//stuAtOgre
//stu@)gre

#define setBackgroundColor setBackgroundColour
#define ColorValue ColourValue
#define initialize initialise
#define initializeAllResourceGroups initialiseAllResourceGroups
#define setDiffuseColor setDiffuseColour
#define setSpecularColor setSpecularColour



class OgreLearn : public CommonAspect
{
public:
    OgreLearn()
    {
        GDKCodec::startup();
        this->root = new Root();
        setupResources();
        //set up jpg here? startup?
        this->setupSuccessful = this->root->showConfigDialog();
        if( !this->setupSuccessful )
            return;

        this->sceneManager = this->root->createSceneManager( ST_EXTERIOR_CLOSE );
        this->root->initialize( false, "" );
        this->physicsEngine = new PhysicsEngine();
        this->physicsEngine->setGravity( 0, -0.9, 0 );
        this->frame = new OISFrame( this->root, this->sceneManager, this->physicsEngine );
        TextureManager::getSingleton().setDefaultNumMipmaps(5);
        createScene();

        //** we should add this when it compiles... **/
        this->root->addFrameListener(this->frame);
    }

    void run()
    {
        if( !this->setupSuccessful )
        {
            this->logger.print( "Warning: setup failed for OgreLearn(), not rendering" );
            return;
        }

        this->root->startRendering();
    }

    ~OgreLearn()
    {
        GDKCodec::shutdown();
        //delete this->frame;
    } 
protected:
    void setupResources()
    {
        ConfigFile config;
        config.load( "resources.cfg" );

        ConfigFile::SectionIterator i = config.getSectionIterator();

        string sectionName, typeName, archName;
        while( i.hasMoreElements() )
        {
            sectionName = i.peekNextKey();
            ConfigFile::SettingsMultiMap* settings = i.getNext();
            for( ConfigFile::SettingsMultiMap::iterator j = settings->begin(); j != settings->end(); ++j )
            {
                typeName = j->first;
                archName = j->second;
                ResourceGroupManager::getSingleton().addResourceLocation( archName, typeName, sectionName );
            }
        }
    }

    void createScene()
    {
        sceneManager->setAmbientLight( ColorValue( 0.1, 0.1, 0.1 ) );
        sceneManager->setShadowTechnique( SHADOWTYPE_STENCIL_ADDITIVE );
        sceneManager->setSkyDome( true, "Examples/CloudySky", 30, 9 );
        //sceneManager->getRootSceneNode()->setPosition( Vector3( 0, 0, 0 ) );
//        sceneManager->setWorldGeometry( "terrain.cfg" );

        bool success= false;
        for( int i = 0; i < 200; ++i )
        {
            string name = "Ninja" + Utility::toString( i, success );
            this->worldObjects[name] = new WorldObject( name, this->sceneManager, this->physicsEngine );
            this->worldObjects[name]->create( "ninja.mesh" );
            this->worldObjects[name]->setPosition( Utility::makeVector( 0.0, i * 100.0, i * 20.0 ) );
            vector<double> vel = Utility::makeVector( 0.0, 1 * ((i + 1)*0.3), 0.0 );
            this->worldObjects[name]->setLinearVelocity( vel );
            this->worldObjects[name]->setMass( 20 );
        }

    
        Plane ground( Vector3::UNIT_Y, 0 );
        MeshManager::getSingleton().createPlane( "ground",
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            ground, 150000, 150000, 20, 20, true, 1, 5, 5, Vector3::UNIT_Z ); 
        this->worldObjects["ground"] = new StaticWorldObject( "the ground", this->sceneManager, this->physicsEngine );
        this->worldObjects["ground"]->create( "ground" );
        this->frame->addWorldObjects( worldObjects );

        Light* light = sceneManager->createLight( "light1" );
        light->setType( Light::LT_POINT );
        light->setPosition( Vector3( 250, 150, 250 ) );
        light->setDiffuseColor( ColorValue::White );
        light->setSpecularColor( ColorValue::White );
        
    }

    OISFrame* frame;
    Root* root;
    SceneManager* sceneManager;
    string cameraNodeName;
    string pitchNodeName;
    bool setupSuccessful;
    PhysicsEngine* physicsEngine;
    map<string,WorldObject*> worldObjects;
};

int main( int argc, char** argv )
{
    try
    {
        OgreLearn renderer;
        renderer.run();
    }
    catch( Exception& e )
    {
        cout << "Exception: " << e.getFullDescription().c_str() << endl;
        cout << "In file: " << e.getFile() << endl;
        cout << "At line: " << e.getLine() << endl;
    }

    return 0;
}
