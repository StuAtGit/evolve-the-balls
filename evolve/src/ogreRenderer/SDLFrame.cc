#include "SDLFrame.h"

SDLFrame::SDLFrame( Root* root, SceneManager* sceneManager, PhysicsEngine* physicsEngine )
{
    if( root == NULL )
    {
        logger.print( "Warning: passed null root to SDLFrame constructor, bailing out..." );
        return;
    }
    if( sceneManager == NULL )
    {
        logger.print( "Warning: passed null sceneManager to SDLFrame constructor, bailing out..." );
        return;
    }

    this->root = root;
    //PlatformManager::getSingleton().createInputReader();
    initializeSDLWindow();
    ResourceGroupManager::getSingleton().initializeAllResourceGroups();
    this->sceneManager = sceneManager;
    this->physicsEngine = physicsEngine;
    this->raySceneQuery = this->sceneManager->createRayQuery( Ray() );

    this->camera = sceneManager->createCamera( "SDLCamera" );
    this->camera->setPosition( Vector3( 0,0,500 ) );
    this->camera->lookAt( Vector3( 0,0,0 ) );
    this->camera->setNearClipDistance( 5 );

    this->cameraNodeName = "SDLCameraNode";
    SceneNode* cameraNode = sceneManager->getRootSceneNode()->createChildSceneNode( this->cameraNodeName, Vector3( 0, 0, 500 ) ); 
    cameraNode->attachObject( camera );

    Viewport* viewport = this->window->addViewport( this->camera );
    viewport->setBackgroundColor( ColorValue( 0,0,0 ) );
    this->camera->setAspectRatio( viewport->getActualWidth() / viewport->getActualHeight() );

    this->rotateDistance = 0.05;
    this->moveRate = 0.08;
    this->moveVector = Vector3::ZERO;
    this->debugOverlay = OverlayManager::getSingleton().getByName( "Core/DebugOverlay" );
    this->continueSimulation = true;
    this->mouseMoveCamera = true;
    this->cursorSetByUser = true;
    SDL_ShowCursor( false );
    this->move = false;
}

SDLFrame::~SDLFrame()
{
    //try not to mess this up if we can..
    SDL_ShowCursor( true );
    SDL_Quit();
}

bool SDLFrame::frameStarted( const FrameEvent &evt )
{
    //this->physicsEngine->doPhysics();

    SDL_Event event;
    //we only poll to pump events & check for SDL_QUIT
    //everything else is filter based
    if( SDL_PollEvent( &event ) )
    {
        //cout << "poll event.." << endl;
        //Generally, this methods should prefer filtering and Get_State methods
        //but event is still needed for stuff like mouse clicks vs mouse pressed
        handleKeyPress( event );
        handleMouseMove( event );
        handleMouseClicked( event );
        //if simulation was already aborted, don't set it back to true!
        this->continueSimulation = this->continueSimulation ? event.type != SDL_QUIT : this->continueSimulation;
    }
    //else
    {
        //cout << "test: " << test << endl;
        //cout << "event: " << event.type << endl;
    }

    if( this->move )
    {
        this->camera->move( this->moveVector * (this->moveRate * (1/evt.timeSinceLastFrame)) );
    }
    return this->continueSimulation;
}

bool SDLFrame::frameEnded( const FrameEvent &evt )
{
    if( !this->mouseMoveCamera )
    {
        SDL_ShowCursor( true );
    }
    updateStats();
    return true;
}

bool SDLFrame::handleKeyPress( SDL_Event event )
{
    //we might need to normalize these...
    Vector3 up = this->camera->getRealUp();
    Vector3 right = this->camera->getRealRight();
    Vector3 forward = up.crossProduct( right );
    //we might want to get the number of key states (the return argument)
    //And just check if SDLK_BLAH >= num_states, don't do it!
    unsigned char* keystates = SDL_GetKeyState( NULL );
    this->moveVector = Vector3::ZERO; //otherwise we go faster and faster the longer they hold down the key :)

    continueSimulation = ( keystates[ SDLK_ESCAPE ] ) ? false : true;
    this->move = false; 
    //add all our keys to the move vector
    if( keystates[ SDLK_w ] )
    {
        cout << "w" << endl;
        this->moveVector += forward;
        this->move = true;
    }
    if( keystates[ SDLK_s ] )
    {
        this->moveVector += (forward * -1);
        this->move = true;
    }
    if( keystates[ SDLK_a ] )
    {
        this->moveVector += right * -1;
        this->move = true;
    }
    if( keystates[ SDLK_d ] )
    {
        this->moveVector += right;
        this->move = true;
    }
    if( keystates[ SDLK_q ] )
    {
        this->moveVector += up;
        this->move = true;
    }
    if( keystates[ SDLK_e ] )
    {
        this->moveVector += up * -1;
        this->move = true;
    }
    if( keystates[ SDLK_z ] )
    {
        mouseMoveCamera = !mouseMoveCamera;
        //if we don't clear this, and it gets back to the function before someone releases it
        //we toggle through the states too fast..
        keystates[ SDLK_z ] = false;
    }
    if( keystates[ SDLK_t ] )
    {
        vector<double> grav = this->physicsEngine->getGravity();
        this->physicsEngine->setGravity( grav[0], grav[1] + 0.1, grav[2] );
    }
    if( keystates[ SDLK_g ] )
    {
        ///cout << "ZERO GRAVITY!" << "(doesn't work :( )" <<  endl;
        this->physicsEngine->setGravity( 0, 0, 0 );
    }
    if( keystates[ SDLK_b ] )
    {
        vector<double> grav = this->physicsEngine->getGravity();
        this->physicsEngine->setGravity( grav[0], grav[1] - 0.1, grav[2] );
    }

    return true;
}

bool SDLFrame::handleMouseClicked( SDL_Event event )
{
    //check if SDL coordinates match up with ogre requirements of (normalized to [0,1])
    int x = 0;
    int y = 0;
    //x and y are integers [0-screenwidth],[0-screenheight]
    unsigned char state = SDL_GetMouseState( &x, &y );

    //we don't care about click events unless we are in selection mode
    if( this->mouseMoveCamera )
    {
        return true;
    }

    //bool success = true;
    //Viewport->getActualWidth() ??
    double normalizedX = (double)x / (double)this->width;
    double normalizedY = (double)y / (double)this->height;
    //logger.print( "Notify: you clicked at coordinate: " + Utility::toString( x, success ) + "," + Utility::toString( y, success ) );
    //logger.print( "Notify: normalized, this is: " + Utility::toString( normalizedX, success ) + "," + Utility::toString( normalizedY, success ) );
    Ray selectionRay = this->camera->getCameraToViewportRay( normalizedX, normalizedY );
    this->raySceneQuery->setRay( selectionRay );
    RaySceneQueryResult& result = this->raySceneQuery->execute();

    if(  state & SDL_BUTTON( SDL_BUTTON_LEFT ))
    {
        for( RaySceneQueryResult::iterator i = result.begin(); i != result.end(); ++i )
        {
            if( i->movable && i->movable->getName() != "SDLCamera" && i->movable->getName() != "the ground" )
            {
                if ( this->objectsSelected.size() == 0  /* || SHIFT_KEY*/ ) 
                {
                    this->objectsSelected.push_back( i->movable );
                    logger.print( "Notify: I think you pick up the: " + i->movable->getName() );
                }
            }
        }
        return true;
    }
    else if( this->objectsSelected.size() > 0 )
    {
        MovableObject* movable = *(--this->objectsSelected.end());
        this->objectsSelected.pop_back();
        logger.print( "Notify: I think you tried to drop the " + movable->getName() );
        Vector3 currentPosition = movable->getParentSceneNode()->getPosition();
        Vector3 normal = this->camera->getPosition() - currentPosition;
        Plane p( normal, 0 );
        MeshManager::getSingleton().createPlane( "temp" + movable->getName() + "plane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, p, 15000, 15000 );
        /*
         * Debugging code with a quite literal "random bug"
        string id = Utility::toString( Utility::getRandomInteger( 0, 52, success ), success );
        Entity* ent = this->sceneManager->createEntity( "temp" + id, "temp" + id );
        this->sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject( ent );
        */
        std::pair<bool,Real> t = selectionRay.intersects( p );
        MeshManager::getSingleton().remove( "temp" + movable->getName() + "plane" ); 
        if( t.first )
        {
            Vector3 newLocation = selectionRay.getPoint( t.second );
            movable->getParentSceneNode()->setPosition( newLocation );
            this->physicsEngine->setPosition( movable->getName(), newLocation.x, newLocation.y, newLocation.z );
        }
    }

    return true;
}

bool SDLFrame::handleKeyReleased( SDL_Event event )
{
    return true;
}

bool SDLFrame::handleMouseMove( SDL_Event event )
{
    int x = 0;
    int y = 0;

    //if the cursor was not set by the user (so shoudl be by WarpMouse), 
    //or if we aren't in mouse move the camera mode
    if( !mouseMoveCamera || !this->cursorSetByUser )
    {
        //any further events will be by the user         
        this->cursorSetByUser = true;
        //consume the event, but don't move.
        SDL_GetRelativeMouseState( &x, &y );
        return true;
    }

    SDL_ShowCursor( false );
    SDL_GetRelativeMouseState( &x, &y );
    //if the mouse moved, then move the camera, reset the cursor to the center,
    //and set the flag that indicates the next event is the one generated by
    //the cursor being moved by WarpMouse
    if( x != 0 || y != 0 )
    {
        this->camera->yaw( Degree( -this->rotateDistance * x ) );
        this->camera->pitch( Degree( -this->rotateDistance * y ) );
        SDL_WarpMouse( this->width / 2, this->height / 2  );
        //the next mouse event will be by WarpMouse
        this->cursorSetByUser = false;
    }
    return true;
}

//implements the SDL window id hack
void SDLFrame::initializeSDLWindow()
{
    if( this->root == NULL )
    {
        logger.print( "Error: in initializeSDLWindow(), root member was null!" );
        this->window = NULL;
        return;
    }
    if( SDL_WasInit( SDL_INIT_VIDEO ) == 0 )
    {
        this->logger.print( "Notify: starting up SDL window" );
        unsigned int width = 1024;
        unsigned int height = 768;
        bool fullscreen = false;

        parseWindowGeometry( this->root->getRenderSystem()->getConfigOptions(), width, height, fullscreen );
        this->width = width;
        this->height = height;
        SDL_Init( SDL_INIT_VIDEO );
        SDL_SetVideoMode( width, height, 0, 0 );
        SDL_WM_SetCaption( "Stu's SDL test", "stuSDLTest" );
        SDL_SysWMinfo info;
        SDL_VERSION( &info.version );
        SDL_GetWMInfo( &info );
        string dsp(&(DisplayString(info.info.x11.display)[1]));
        vector<string> tokens = Ogre::StringUtil::split(dsp,".");
        if( tokens.size() < 2 )
        {
            logger.print( "Error: split of display string: " + dsp + " didn't work" );
            return;
        }
        bool success = false;
        bool printDebug = true;
        map<string,string> windowHandle;
        string s = Utility::toString( (long)info.info.x11.display, success, printDebug );
        s += ":" + tokens[1] + ":";
        s += Utility::toString( (long)info.info.x11.window, success, printDebug );
        windowHandle["parentWindowHandle"] = s;
        logger.print( "Notify: setting a up a : " + Utility::toString(width,success) + "x" + Utility::toString(height,success) + " window" );
        fullscreen = true;
        this->window = this->root->createRenderWindow( "ogre", width, height, fullscreen, &windowHandle );
        this->window->setActive( true );
        this->window->setAutoUpdated( true );
        this->logger.print( "Notify: SDL window up and running.." );
    }
    else
    {
        logger.print( "Notify: didn't bother with window id hack - SDL already seems initialized" );
        this->window = this->root->initialize( true, "YourAppName" );
    }
}

void SDLFrame::parseWindowGeometry( Ogre::ConfigOptionMap& config, unsigned int& width, unsigned int& height, bool& fullscreen )
{
    Ogre::ConfigOptionMap::iterator option = config.find( "Video Mode" );
    if( option != config.end() )
    {
        string value = option->second.currentValue;
        //stored as widthxheight
        unsigned int xposition = value.find( 'x' );
        bool success = false;
        bool printDebug = true;
        if( xposition != string::npos )
        {
            width = Utility::stringTo<unsigned int>( Utility::trim(value.substr( 0, xposition )), success, printDebug );
            height = Utility::stringTo<unsigned int>( Utility::trim(value.substr( xposition + 1 )), success, printDebug );
        }
    }

    option = config.find( "Full Screen" );
    if( option != config.end() )
    {
        fullscreen = (option->second.currentValue == "Yes");
    }

}

void SDLFrame::updateStats()
{
    /*
    string currentFPSLabel = "Current FPS: ";
    string averageFPSLabel = "Average FPS: ";
    string bestFPSLabel = "Best FPS: ";
    string worstFPSLabel = "Worst FPS: ";
    string triangleCountLabel = "Triangle Count: ";

    OverlayElement* averageFPSOverlay = OverlayManager::getSingleton().getOverlayElement( "Core/AverageFps" );
    OverlayElement* currentFPSOverlay = OverlayManager::getSingleton().getOverlayElement( "Core/CurrFps" );
    OverlayElement* bestFPSOverlay = OverlayManager::getSingleton().getOverlayElement( "Core/BestFps" );
    OverlayElement* worstFPSOverlay = OverlayManager::getSingleton().getOverlayElement( "Core/WorstFps" );
    OverlayElement* triangleCountOverlay = OverlayManager::getSingleton().getOverlayElement( "Core/NumTris" );
    OverlayElement* debugOverlay = OverlayManager::getSingleton().getOverlayElement( "Core/DebugText" );

    RenderTarget::FrameStats frameStatistics = this->window->getStatistics();

    bool success = false;
    averageFPSOverlay->setCaption( averageFPSLabel + Utility::toString( frameStatistics.avgFPS, success, true ) );
    currentFPSOverlay->setCaption( currentFPSLabel + Utility::toString( frameStatistics.lastFPS, success, true ) );
    bestFPSOverlay->setCaption( bestFPSLabel + Utility::toString( frameStatistics.bestFPS, success, true ) );
    worstFPSOverlay->setCaption( worstFPSLabel + Utility::toString( frameStatistics.worstFPS, success, true ) );
    triangleCountOverlay->setCaption( triangleCountLabel + Utility::toString( frameStatistics.triangleCount, success, true ) );
    debugOverlay->setCaption( this->window->getDebugText() );
    */
}

