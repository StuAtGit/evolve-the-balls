#include <iostream>
#include <pthread.h>
#include "Server.h"
//#include "ConfigFileReader.h"

//static map<string,string> configSettings;
//new class to read in a config file - 
//should at least have bool getEntry( string key, T value ); member
//static ConfigFileReader configFileReader;
using namespace evolve;

int main()
{

    int serverPort = 5010;
    int pid = fork();
    if( pid == 0 )
    {
        cout << "client?" << endl;
        sleep(2);
        SocketMethods socketMethods;
        int clientSock = -1;
        bool printDebug = true;
        bool success = false;
        if( !socketMethods.getClientSocket( clientSock, "127.0.0.1", serverPort, printDebug ) )
        {
            cout << "client couldn't connect to server" << endl;
            exit(1);
        }
        socketMethods.write( clientSock, "HOBBBIDILY-HOOOBIDILY ...... WHOOO!?0", printDebug );
        string reply = "";
        socketMethods.recieve( clientSock, reply, printDebug );
        cout << "CLIENT: got reply: " << reply << endl;
        vector<string> commandSequence;
        commandSequence.push_back( "AFSDFSDFWETRWL%KTJ$:L%A$*&!)@($*_@!#$*@)#A(^*_#$)*^%(#$^#$%#$%+%$()FDSLKJVZXC><M$%LK'rlkja43-0598123509785fd?0" );
        commandSequence.push_back( "AH!?0" );
        commandSequence.push_back( "A!?0" );
        vector<string> replies;
        socketMethods.sendCommandSequence( clientSock, commandSequence, replies, printDebug );
        cout << "CLIENT: got replies: " << Utility::vectorToString<string>( replies, success ) << endl;
    }
    else
    {
        cout << "server? " << endl;
        Server server;
        server.setNotify( true );
        server.setDefaultHandlerLibLocation( "./commandHandler/.libs/libCommandHandler.so" );
        server.serverMain( serverPort, false, false, true );
    }
    //fork, and start test client...
    //put command req/replies in Commands.xml ?
    //No, put in the directory with the handler, so they can be kept in sync
    return 0;
}
