#include <iostream>
#include <pthread.h>
#include "SystemInterfaceHandlers.h"
#include "Server.h"
//#include "ConfigFileReader.h"

//static map<string,string> configSettings;
//new class to read in a config file - 
//should at least have bool getEntry( string key, T value ); member
//static ConfigFileReader configFileReader;

using namespace std;

int main()
{

    int SystemServerPort = 5012;
    int pid = fork();
    if( pid == 0 )
    {
        cout << "client?" << endl;
        sleep(2);
        SocketMethods socketMethods;
        int clientSock = -1;
        if( !socketMethods.getClientSocket( clientSock, "127.0.0.1", SystemServerPort, true ) )
        {
            cout << "client couldn't connect to server" << endl;
            exit(1);
        }
        SystemInterfaceHandlers clientInterface;
        bool printDebug = true;
        vector<string> replies;
        clientInterface.handle_create( clientSock, "0", replies, printDebug );
        clientInterface.handle_create( clientSock, "2", replies, printDebug );
        clientInterface.handle_create( clientSock, "1", replies, printDebug );
        clientInterface.handle_load( clientSock, "2", replies, printDebug );
        clientInterface.handle_load( clientSock, "0", replies, printDebug );
        map<string,string> ignoreCrossMap;
        map<string,string> systemCrossMap;
        systemCrossMap["random:random"] = 3;
        systemCrossMap["0:1"] = 3;
        systemCrossMap["1:2"] = 2;
        systemCrossMap["3:5"] = 3;
        clientInterface.handle_cross( clientSock, "0", "1", ignoreCrossMap, ignoreCrossMap,
                                      systemCrossMap, ignoreCrossMap, replies, printDebug);
        clientInterface.handle_run( clientSock, "0", "grow", 10, replies, printDebug );
        clientInterface.handle_run( clientSock, "2", "grow", 10, replies, printDebug );
        clientInterface.handle_cross( clientSock, "0", "2", ignoreCrossMap, ignoreCrossMap,
                                      systemCrossMap, ignoreCrossMap, replies, printDebug);
        cout << "cross of 0 & 2: " << replies[replies.size() - 1] << endl;
        clientInterface.handle_getSystem( clientSock, "0", replies, printDebug );
        string system0 = replies.back();
        clientInterface.handle_getSystem( clientSock, "1", replies, printDebug );
        string system1 = replies.back();
        clientInterface.handle_getSystem( clientSock, "2", replies, printDebug );
        string system2 = replies.back();
        cout << "****************** Client *****************" << endl;
        cout << "CLIENT: system0: " << system0 << endl;
        cout << "CLIENT: system1: " << system1 << endl;
        cout << "CLIENT: system2: " << system2 << endl;
    }
    else
    {
        cout << "server? " << endl;
        Server server;
        server.setNotify( true );
        server.setDefaultHandlerLibLocation( "./libSystemCommandHandler.so" );
        server.serverMain( SystemServerPort, false, false, true );
    }
    //fork, and start test client...
    //put command req/replies in Commands.xml ?
    //No, put in the directory with the handler, so they can be kept in sync
    return 0;
}
