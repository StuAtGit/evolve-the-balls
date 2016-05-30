#ifndef STU_SERVER_H
#define STU_SERVER_H

#define MALLOC_CHECK_ 1

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <dlfcn.h>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include "CommonAspect.h"
#include "Commands.h"
#include "CommandHandler.h"
#include "SocketMethods.h"
#include "Mutex.h"
#include "ReadWriteLock.h"
#include "Utility.h"

using namespace std;
using namespace boost;
using namespace evolve;

#define DO_DEBUG true

namespace evolve
{
    /**
     * Class that factors out all the server listening & spawning thread code
     * Essentially just starts up a server socket, listens, parses a command,
     * and dispatches it to whatever handler library it loaded for this instance
     */
    class Server : public CommonAspect
    {
    public:
        Server();
        virtual ~Server();
        /**
         * invokes the handler method of the loaded library
         * @param command the command to pass to the handler
         * @id the id to pass to the handler
         * @sock the socket -> this should generally be the client socket
         */
        bool invokeHandler( const string& command, const string& id, int sock, bool printDebug );
        /**
         * Starts the main loop for the server, listening for connections, launching threads, etc
         * @port the port this server will listen on
         * @doRoundRobin do round robin thread schedule. Should always be false since you should 
         *               never run as root.
         * @preLoad automatically load existing data into the server. Originally concieved to load
         *          existing lsystems into a lsystem server
         */
        bool serverMain( int port, bool doRoundRobin, bool preLoad, bool printDebug = DO_DEBUG );
        /**
         * load the handler library that provides the specific functionality for this server
         * @param libraryPath the path to the library to use
         */
        bool loadHandlerMethodLib( const string& libraryPath, bool printDebug = DO_DEBUG );
        /**
         * Sets the default handler lib to load when starting up in serverMain
         * @param libLocation a string that is the path aand name of the server
         */
        bool setDefaultHandlerLibLocation( const string& libLocation, bool printDebug = DO_DEBUG );
        /**
         * Returns the current location of the current handler lib
         */
        string getDefaultHandlerLibLocation( bool printDebug = DO_DEBUG );
        
        /**
         * Ignore sig pipes, since we can't do anything about them
         */
        friend void handle_sigpipe( int sig_num );
        /**
         * handler for Ctrl-C, etc
         */
        friend void handle_sock_cleanup( int sig_num );
    
    protected:
        //private methods
        bool closeAndDeleteSocket( int sock, bool printDebug = DO_DEBUG );
        void closeAndDeleteSockets( bool printDebug = DO_DEBUG );
        bool parseCommand( string& command, string& id, bool printDebug );
        friend void* handleClient( void* arg );
    
        //private members
        vector<int> socks; 
        //the location of the default handler library
        string defaultHandlerLib;
        //the pointer at the current handler methods object
        CommandHandler* commandHandler;
        SocketMethods socketMethods;
        Commands* commands;
        //the handle the current library (must track this so we can close it appropriately)
        void* currentHandle;
    };
   
    void* handleClient( void* arg );
    /**
     * Essentially, this here So we can pass the this pointer to the thread handler (handleClient)
     */
    struct thread_args
    {
        Server* _this;
        int sock;
       
        thread_args()
        {
            _this = NULL;
            sock = -1;
        }
        
        thread_args( Server* my_this, int clientSock )
        {
            this->_this = my_this;
            this->sock = clientSock;
        }
    };
};
#endif
