#ifndef OGRE_RENDERER_CC_STU
#define OGRE_RENDERER_CC_STU

#include "OgreRenderer.h"

OgreRenderer::OgreRenderer()
{
    GDKCodec::startup();
    this->root = new Root();
    setupResources();
    this->setupSuccessful = this->root->showConfigDialog();
    if( !this->setupSuccessful )
        return;

    this->sceneManager = this->root->createSceneManager( ST_INTERIOR );
    this->root->initialize( false, "" );
    this->physicsEngine = new PhysicsEngine();
    this->physicsEngine->setGravity( 0, 0.0, 0 );
    this->frame = new OISFrame( this->root, this->sceneManager, this->physicsEngine );
    //TextureManager::getSingleton().setDefaultNumMipmaps(5);
}

OgreRenderer::~OgreRenderer()
{
    GDKCodec::shutdown();
}

SceneManager* OgreRenderer::getSceneManager()
{
    logger.print( "Notify: entered OgreRenderer::getSceneManager, returning: " + Utility::toString( this->sceneManager ), this->doNotify );
    return this->sceneManager;
}

PhysicsEngine* OgreRenderer::getPhysicsEngine()
{
    logger.print( "Notify:: entered OgreRenderer::getPhysicsEngine()", this->doNotify );
    return this->physicsEngine;
}

bool OgreRenderer::run()
{
    if( !this->setupSuccessful )
    {
        this->logger.print( "Warning: setup failed for OgreRenderer(), not rendering" );
        return false;
    }

    try
    {
        this->root->startRendering();
    }
    catch( Exception& e )
    {
        cout << "Exception: " << e.getFullDescription().c_str() << endl;
        cout << "In file: " << e.getFile() << endl;
        cout << "At line: " << e.getLine() << endl;
        return false;
    }
    return true;
}

bool OgreRenderer::createScene( const map<string,WorldObject*>& objects )
{
    sceneManager->setAmbientLight( ColorValue( 0.1, 0.1, 0.1 ) );
    sceneManager->setShadowTechnique( SHADOWTYPE_TEXTURE_MODULATIVE );
//    sceneManager->setSkyDome( true, "Examples/CloudySky", 30, 9 );

    this->frame->addWorldObjects( objects );
    this->worldObjects = objects;

    Light* light = sceneManager->createLight( "light1" );
    light->setType( Light::LT_POINT );
    light->setPosition( Vector3( 250, 150, 250 ) );
    light->setSpecularColor( ColorValue::White );
    light->setDiffuseColor( ColorValue::White );
    Light* light2 = sceneManager->createLight( "light2" );
    light2->setType( Light::LT_POINT );
    light2->setPosition( Vector3( 250, 150, -250 ) );
    light2->setSpecularColor( ColorValue::White );
    light2->setDiffuseColor( ColorValue::Blue );

    this->root->addFrameListener(this->frame);
    return true;
}

void OgreRenderer::setupResources()
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

OISFrame* OgreRenderer::getFrame()
{
    return this->frame;
}

double OgreRenderer::getPhysicsTime()
{
    return this->frame->getPhysicsTime();
}

bool OgreRenderer::setGravity( vector<double> gravity )
{
    if( gravity.size() < 3 )
    {
        logger.print( "Warning: gravity vector to OgreRenderer::setGravity() has less than 3 dimensions" );
        return false;
    }
    return this->physicsEngine->setGravity( gravity[0], gravity[1], gravity[2] );
}

#endif
