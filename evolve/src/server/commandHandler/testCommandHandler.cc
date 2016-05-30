#include <dlfcn.h>
#include <iostream>
#include <map>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "CommandHandler.h"

using namespace evolve;

int main()
{
    try
    {
    Logger logger;
//    map<string, bool(CommandHandler::*)(Server*, string id, int sock, bool printDebug)> handlers;
    void* dlhandle = NULL;
    if( ( dlhandle = dlopen( "libCommandHandler.so", RTLD_GLOBAL | RTLD_NOW ) ) == NULL )
    {
        logger.print( "Error: dlopen failed: " + lexical_cast<string>( dlerror() ) ); 
        exit(0);
    }

    CommandHandler* (*fn_create)();
    bool (*fn_destroy)(CommandHandler*&);

    if( ( fn_create = (CommandHandler*(*)())dlsym( dlhandle, "createCommandHandlerObject" ) ) == NULL )
    {
        logger.print( "Error: dlopen failed: " + lexical_cast<string>( dlerror() ) ); 
        exit(0);
    }

    if( ( fn_destroy = (bool (*)(CommandHandler*&))dlsym( dlhandle, "destroyCommandHandlerObject" ) ) == NULL )
    {
        logger.print( "Error: dlopen failed: " + lexical_cast<string>( dlerror() ) ); 
        exit(0);
    }
    CommandHandler* handler = (*fn_create)();
    /*
    DefaultCommandHandler* hdl = dynamic_cast<DefaultCommandHandler*>(handler); 
    if( hdl == NULL )
    {
        cout << "failed to cast " << endl;
        exit(1);
    }
    dlclose( dlhandle );
    */
    string id = "client1";
    int sock = -1;
    bool printDebug = true;
    cout << "handler before: " << handler << endl;
    
    handler->handleCommand( "new command", id, sock, printDebug );
    (*fn_destroy)( handler );
    cout << "handler ptr now: " << handler << endl;
    }
    catch( ... )
    {
        cout << "afsdasfdaf" << endl;
    }
}
