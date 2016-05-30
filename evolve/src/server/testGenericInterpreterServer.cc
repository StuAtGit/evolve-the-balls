#include <iostream>
#include <pthread.h>
#include "InterpreterInterfaceHandlers.h"
#include "SystemInterfaceHandlers.h"
#include "Server.h"

using namespace std;

int main()
{
    int SystemServerPort = 5012;
    int InterpreterServerPort = 5013;
    int pid = fork();
    if( pid == 0 )
    {
        sleep(2);
        
        SocketMethods socketMethods;
        //first create lsystems that we can interpret
        int lSysClientSock = -1;
        if( !socketMethods.getClientSocket( lSysClientSock, "127.0.0.1", SystemServerPort, true ) )
        {
            cout << "lsys client couldn't connect to server" << endl;
            exit(1);
        }

       SystemInterfaceHandlers lSysClient;
       vector<string> replies;
       bool printDebug = true;
       lSysClient.handle_create( lSysClientSock, "0", replies, printDebug );
       lSysClient.handle_create( lSysClientSock, "1", replies, printDebug );

       lSysClient.handle_load( lSysClientSock, "0", replies, printDebug );
       lSysClient.handle_load( lSysClientSock, "1", replies, printDebug );

       lSysClient.handle_run( lSysClientSock, "0", "grow", 10, replies, printDebug );
       lSysClient.handle_run( lSysClientSock, "1", "grow", 10, replies, printDebug );

       lSysClient.handle_makeCopy( lSysClientSock, "1", "3", replies, printDebug );

       lSysClient.handle_run( lSysClientSock, "3", "mutate", 1000, replies, printDebug );

       lSysClient.handle_getSystem( lSysClientSock, "0", replies, printDebug );
       string lsystem1 = replies.back();
       lSysClient.handle_getSystem( lSysClientSock, "1", replies, printDebug );
       string lsystem2 = replies.back();
       lSysClient.handle_getSystem( lSysClientSock, "3", replies, printDebug );
       string lsystem3 = replies.back();

       cout << " *********************** system 1 " << lsystem1 << endl;
       cout << " *********************** system 2 " << lsystem2 << endl;
       cout << " *********************** system 3 " << lsystem3 << endl;

        //now test the interpreter
        int clientSock = -1;
        if( !socketMethods.getClientSocket( clientSock, "127.0.0.1", InterpreterServerPort, true ) )
        {
            cout << "Interpreter client couldn't connect to server" << endl;
            exit(1);
        }
        InterpreterInterfaceHandlers clientInterface;
        clientInterface.handle_create( clientSock, "0", replies, printDebug );
        clientInterface.handle_create( clientSock, "1", replies, printDebug );

        clientInterface.handle_load( clientSock, "1", replies, printDebug );
        clientInterface.handle_load( clientSock, "0", replies, printDebug );

        clientInterface.handle_makeCopy( clientSock, "0", "2", replies, printDebug );
        clientInterface.handle_getAttribute( clientSock, "0", "AtomicSize", replies, printDebug );
        cout << "interpreter(0) atomic size " << replies.back() << endl;

        cout << "** (1) interpreting system: " << lsystem1 << endl;
        clientInterface.handle_getSystem( clientSock, "0", lsystem1, replies, printDebug );
        cout << "** (2) interpretation: " << replies.back() << endl;

        cout << "** (3) interpreting System: " << lsystem2 << endl;
        clientInterface.handle_getSystem( clientSock, "1", lsystem2, replies, printDebug );
        cout << "** (4) interpretation: " << replies.back() << endl;

        cout << "** (3) interpreting System: " << lsystem3 << endl;
        clientInterface.handle_getSystem( clientSock, "2", lsystem2, replies, printDebug );
        cout << "** (4) interpretation: " << replies.back() << endl;
    }
    else
    {
        int serverPid = fork();
        if( serverPid == 0 )
        {
            cout << "LSystem server..." << endl;
            Server server;
//            server.setNotify( true );
            server.setDefaultHandlerLibLocation( "./libSystemCommandHandler.so" );
            server.serverMain( SystemServerPort, false, false, true );
            return 0;
        }
        cout << "Interpreter server...." << endl;
        Server server;
        server.setNotify( true );
        server.setDefaultHandlerLibLocation( "./libInterpreterCommandHandler.so" );
        server.serverMain( InterpreterServerPort, false, false, true );
    }
    //fork, and start test client...
    //put command req/replies in Commands.xml ?
    //No, put in the directory with the handler, so they can be kept in sync
    return 0;
}
