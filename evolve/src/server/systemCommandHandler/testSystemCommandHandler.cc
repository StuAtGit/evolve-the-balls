#include <dlfcn.h>
#include <boost/lexical_cast.hpp>
#include "CommandHandler.h"
#include "SystemCommandHandler.h"
#include "Logger.h"

using namespace std;
using namespace boost;

int main()
{
    Logger logger;
    void* dlhandle = NULL;
    if( ( dlhandle = dlopen( "libSystemCommandHandler.so", RTLD_GLOBAL | RTLD_NOW ) ) == NULL )
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
    string id = "client1";
    int sock = -1;
    bool printDebug = true;
    handler->handleCommand( "new command", "lbah", sock, printDebug );
    (*fn_destroy)( handler );
    cout << "handler ptr now: " << handler << endl;
    dlclose( dlhandle );
}
