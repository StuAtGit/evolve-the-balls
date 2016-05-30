#include <string>
#include <vector>
#include <map>
#include <iostream>

#include <boost/lexical_cast.hpp>

#include "Server.h"
#include "CommandHandler.h"

using namespace std;
using namespace boost;
using namespace evolve;

int main()
{
    int systemServerPort = 5012;
    int interpreterServerPort = 5013;
    string ip = "127.0.0.1";

    if( fork() == 0 )
    {
        Server server;
        server.setDefaultHandlerLibLocation( "./interpreterCommandHandler/.libs/libInterpreterCommandHandler.so" );
        server.serverMain( interpreterServerPort, false, false, true );
    }
    else
    {
        Server server;
        server.setDefaultHandlerLibLocation( "./systemCommandHandler/.libs/libSystemCommandHandler.so" );
        server.serverMain( systemServerPort, false, false, true );
    }
    
    return 0;
}
