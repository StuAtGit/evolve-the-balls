#ifndef SERVER_STU_CC
#define SERVER_STU_CC

#include "Server.h"

//For signal handlers...
//(Would be nice if we could keep this in sync with the objects logger...)
static Logger static_logger;

void handle_sigpipe( int sig_num )
{
    //for now, this looks Ok (and what else can we do?) and threads that were handling
    //the bad connection go away (I was worried they would linger..)
    static_logger.print( "Error: LSystemServer caught SIGPIPE" );
    static_logger.print( "Error: ignoring sigpipe & praying the client just went away or the writer knew what to do" );
}

void handle_sock_cleanup( int sig_num )
{
    static_logger.print( "Notify: Entered handle_sock_cleanup" );
    _exit(0);
}

Server::Server()
{
    this->commands = Commands::getInstance();
    this->commandHandler = NULL;
    this->defaultHandlerLib = "./libDefaultCommandHandler.so";
    this->currentHandle = NULL;
}

Server::~Server()
{
    this->closeAndDeleteSockets();
}

void* evolve::handleClient( void* t_args )
{
    if( t_args == NULL )
    {
        return NULL;
    }
    
    int sock = ((thread_args*)(t_args))->sock;
    Server* _this = ((thread_args*)(t_args))->_this;

    bool printDebug = true; 
    string command = "";
    while( command != _this->commands->shutdownMessage )
    {
        command = "";
        bool success = _this->socketMethods.receive( sock, command, printDebug ); 

        if( !success ) 
        {
            _this->logger.print( "Notify: Server::handleClient client left the server or error reading socket", _this->doNotify );
            break;
        }
        else
        {
            static_logger.print( "Notify: Server::handleClient got command: " + command, _this->doNotify && printDebug );
            string id = "";
            if( !_this->parseCommand( command, id, printDebug ) )
            {
                string errorMsg = "Warning: Server::handleClient, parsing of command: " + command + " failed, format is: command?id";
                if( !_this->socketMethods.write( sock, errorMsg, printDebug ) )
                {
                    _this->logger.print( "Error: write to client failed, perhaps it left abruptly" );
                    break;
                }
                _this->logger.print( errorMsg, printDebug ); 
            }
            else
            {
                if( command == _this->commands->reloadCommandHandler )
                {
                    //unless the client sent us DEFAULT_LIB
                    //we assume they're sending us the new name of a lib
                    string backup = _this->defaultHandlerLib;
                    if( id != _this->commands->defaultLibId )
                    {
                        _this->defaultHandlerLib = id;
                    }
                    if( !_this->loadHandlerMethodLib( _this->defaultHandlerLib ) )
                    {
                        string errorMsg = "Warning: failed loading library: " + _this->defaultHandlerLib + 
                                          + " resetting to previous library";
                        _this->logger.print( errorMsg );
                        _this->socketMethods.write( sock, errorMsg, printDebug );
                        _this->defaultHandlerLib = backup;
                        if( !_this->loadHandlerMethodLib( _this->defaultHandlerLib ) )
                        {
                            _this->logger.print( "Error: couldn't reset to old library, no handler lib loaded!" );
                        }
                    }
                    else
                    {
                        string message = "Notify: successfully reloaded library: " + _this->defaultHandlerLib;
                        _this->socketMethods.write( sock, message, printDebug );
                    }
                }
                else
                {
                    _this->invokeHandler( command, id, sock, printDebug );
                }
            }
        }
    }
   
    //cleanup the allocated memory
    if( t_args != NULL ) {
        delete (thread_args*)(t_args);
    }

    close( sock );
    pthread_exit( NULL );
    return NULL;
}

bool Server::invokeHandler( const string& command, const string& id, int sock, bool printDebug )
{
    this->logger.print( "Notify: entered Server::invokeHandler, command is " + command + " id is: " + id, this->doNotify && printDebug );
    if( this->commandHandler == NULL )
    {
        string errorMsg = "Warning: in Server::invokeHandler, commandHandler object not initialized";
        this->logger.print( errorMsg, printDebug );
        this->socketMethods.write( sock, errorMsg, printDebug );
        return false;
    }
    this->commandHandler->handleCommand( command, id, sock, printDebug );
    return true;
}

bool Server::serverMain( int port, bool doRoundRobin, bool preLoad, bool printDebug )
{
    this->logger.print( "Notify: entered Server::LSystemServerMain", printDebug && this->doNotify );
    signal( SIGPIPE, handle_sigpipe );
    signal( SIGINT, handle_sock_cleanup );
    signal( SIGQUIT, handle_sock_cleanup );

    int serverSocket = -1;
    if( !this->socketMethods.getServerSocket( serverSocket, port, printDebug ) )
    {
        this->logger.print( "Error: Server::lSystemServerMain failed to open server socket" +
                            lexical_cast<string>(strerror( errno ) ), printDebug );
        return false;
    }

    cout << "server socket: " << serverSocket << endl;
    socks.push_back( serverSocket );

    if( !loadHandlerMethodLib( this->defaultHandlerLib ) )
    {
        logger.print( "Error: in Server::initDefaultCommandHandler couldn't load default handler method library: " + this->defaultHandlerLib, printDebug ); 
        return false;
    }

    while( 1 )
    {
        logger.print( "Notify: Server::lSystemServerMain starting up on port: " + lexical_cast<string>(port), this->doNotify && printDebug );
        cout << "server socket: " << serverSocket << endl;
        int clientSocket = socketMethods.accept( serverSocket, printDebug );
        if( clientSocket != -1 )
        {
            socks.push_back( clientSocket );
            pthread_t pthread;
            pthread_attr_t pthread_attr;
            pthread_attr_init( &pthread_attr );
            //we should never really do this, because we should never run as root!
            if( doRoundRobin )
            {
                int ret = pthread_attr_setschedpolicy( &pthread_attr, SCHED_RR );
                logger.print( "Notify: Server::lSystemServerMain set thread schedule policy to Round Robin", ret != -1 && this->doNotify && printDebug );
                logger.print( "Warning: Server::lSystemServerMain failed to set thread schedule policy to Round Robin", ret == -1 && printDebug );
            }
            thread_args* t_args = new thread_args( this, clientSocket );
            //just passing &sock may work, but isn't right (see think dir)
            //pthread_create simply seg faults if you forget to link to libpthread
            logger.print( "Notify: server kicking off thread", this->doNotify && printDebug );
            int ret = pthread_create( &pthread, &pthread_attr, handleClient, t_args );
            logger.print( "Notify: thread created", this->doNotify && printDebug && ret == 0 );
            if( ret != 0 )
            {
                string errorMsg = lexical_cast<string>(strerror(ret));
                logger.print( "Error: Server::lSystemServerMain failed in thread creation: " + 
                               errorMsg );
                this->socketMethods.write( clientSocket, "Error: system busy: " + errorMsg + " try again later", printDebug );
                delete t_args;
                closeAndDeleteSocket( clientSocket, printDebug );
            }
            
        }
        else
        {
            logger.print( "Error: Server::lSystemServerMain: accept failed ", printDebug );
            break;
        }

    }

    closeAndDeleteSockets( printDebug );
    return true;
}

bool Server::loadHandlerMethodLib( const string& filename, bool printDebug )
{
    if( this->commandHandler != NULL )
    {
        bool (*deleteObjMethod)(CommandHandler*&);
        if( (deleteObjMethod = (bool (*)(CommandHandler*&))dlsym( this->currentHandle, "destroyCommandHandlerObject" ) ) == NULL )
        {
            logger.print( "Warning: in Server::loadHandlerMethodLib symbol lookup of: createCommandHandlerObject failed, " + lexical_cast<string>( dlerror() ), printDebug );
            return false;        
        }
        if( !((*deleteObjMethod)(this->commandHandler)) )
        {
            string errorMsg = "Error: destruction of commandHandler failed, possible memory leak..";
            logger.print( errorMsg, printDebug );
        }
        this->commandHandler = NULL;
        //must close this, especially if we try to reload the same library (or the same handle
        //gets used, and the library isn't really reloaded)
        if( dlclose( this->currentHandle ) != 0 )
        {
            logger.print( "Warning: in Server::loadHandleMethodLib, dlclose failed? " + lexical_cast<string>( dlerror() ), printDebug );
        }
        this->currentHandle = NULL;
    }

    this->currentHandle = dlopen( filename.c_str(), RTLD_GLOBAL | RTLD_NOW );
    if( currentHandle == NULL )
    {
        logger.print( "Warning: in Server::loadHandlerMethodLib dlopen of: " + filename + " failed, " + lexical_cast<string>( dlerror() ), printDebug );
        return false;
    }
    CommandHandler* (*getObjMethod)();
    if( (getObjMethod = (CommandHandler* (*)())dlsym( this->currentHandle, "createCommandHandlerObject" ) ) == NULL )
    {
        logger.print( "Warning: in Server::loadHandlerMethodLib symbol lookup of: createCommandHandlerObject failed, " + lexical_cast<string>( dlerror() ), printDebug );
        return false;        
    }
    //I suppose we just trust that this method worked?
    this->commandHandler = (*getObjMethod)(); 
       //do this so the command handler prints out notify messages if 
    //the server is set to as well
    this->commandHandler->setNotify( this->getNotify() );
    return true;
}

bool Server::setDefaultHandlerLibLocation( const string& libLocation, bool printDebug )
{
    logger.print( "Notify: entered Server::setDefaultHandlerLibLocation", this->doNotify && printDebug );

    this->defaultHandlerLib = libLocation;
    return true;
}

string Server::getDefaultHandlerLibLocation( bool printDebug )
{
    logger.print( "Notify: entered Server::getDefaultHandlerLibLocation", this->doNotify && printDebug );

    return this->defaultHandlerLib;
}

bool Server::closeAndDeleteSocket( int sock, bool printDebug )
{
    logger.print( "Notify: entered closeAndDeleteSock", printDebug && this->doNotify );

    for( vector<int>::iterator i = this->socks.begin(); i != this->socks.end(); ++i )
    {
        if( sock == *i )
        {
            close( *i );
            socks.erase( i );
            return true;
        }
    }

    return false;
}

void Server::closeAndDeleteSockets( bool printDebug )
{
    logger.print( "Notify: entered closeAndDeleteSockets" );
    for( vector<int>::iterator i = this->socks.begin(); i != this->socks.end(); ++i )
    {
        close( (*i) );
    }
    this->socks.clear();
}


bool Server::parseCommand( string& command, string& id, bool printDebug )
{
    logger.print( "Notify: entered Server::parseCommand ", this->doNotify && printDebug );
    
    char_separator<char> sep( "?" );
    tokenizer< char_separator<char> > tok(command, sep);
    tokenizer< char_separator<char> >::iterator iter = tok.begin();
    if( iter == tok.end() )
    {
        logger.print( "Warning: in Server::parseCommand tried to parse empty command: " + command );
        return false;
    }
    cout << "parse command command is: " << *iter << endl;
    //if we actually change command, the we can mess up the tokenizer
    //(this only happened on big strings though... bug in boost?
    string tmp_command = *iter;
     ++iter;
    if( iter == tok.end() )
    {
        logger.print( "Warning: in Server::parseCommand tried to parse command without an id delimiter (?): " + command );
        return false;
    }
    cout << "parsCommand, id is: " << *iter << endl;
    command = tmp_command;
    id = *iter; 

    return true;
}

#endif
