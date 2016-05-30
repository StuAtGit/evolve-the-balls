#include <iostream>
#include <string>
#include "SocketMethods.h"
#include "Utility.h"

using namespace std;

int main()
{
    SocketMethods socketMethods;
    Logger logger;
    //this is here because I saw a weird free() error that went away when I removed the 
    //Timer() destructor...
    //(Was this a result of the commonAspect() class & weird interaction
    //with that destructor.. worth looking at at *some* point...
    char* bigArray = new char[4000];

    socketMethods.setNotify( true );

    if( fork() == 0 )
    {
        //I'm the child (and the client)
        sleep(1);
        int sock = -1;
        if( !socketMethods.getClientSocket( sock, "127.0.0.1", 2226, true ) )
        {
            logger.print( "Error in testSocketMethods, getClientSocket", true );
            return false;
        }
        bool success = false;
        success = socketMethods.write( sock, "Hello World!", true ); 
        //huge message tests when write() doesn't write everything
        string hugeMessage = "asdfasf2453q45345''dfasdflkajsdflaksjdf;cx zx09cgv873-4697=13-085_)(*%)@(*!$)9853235q234-585r0-9485-sd098asdf;jkasdf;ajsdflajksdfa;lsdjfa;lkj45;l2qk5u4q2-0459q2-581-340812-308123=194-20853646346532450";
        success = success && socketMethods.write( sock, hugeMessage, true );
        //short message tests when two messages get into one recieve buffer
        string shortMessage = "a";
        success = success && socketMethods.write( sock, shortMessage, true );
        success = success && socketMethods.write( sock, shortMessage, true );
        logger.print( "Error in testSocketMethods, writeSocket failed", !success );
        vector<string> commandSequence;
        commandSequence.push_back( "Hello" );
        commandSequence.push_back( "Goodbye" );
        vector<string> replies;
        success = socketMethods.sendCommandSequence( sock, commandSequence, replies, true );
        logger.print( "Error in testSocketMethods, sendCommandSequence failed", !success );       
        cout << "client got replies: " << Utility::vectorToString<string>( replies, success ) << endl;
        close(sock);
    }
    else
    {
        //I'm the parent (and the server)
        int sock = -1;
        if( !socketMethods.getServerSocket( sock, 2226, true ) )
        {
            logger.print( "Error in testSocketMethods, getServerSocket", true );
            return false;
        }

        cout << "server socket is: " << sock << endl;
        int client = socketMethods.accept( sock, true );
        cout << "got client: " << client << endl;

        string reply = "";
        do
        {
            reply = "";
            socketMethods.recieve( client, reply, true );
            if( reply == "Hello" ) {
                socketMethods.write( client, " Wonderful World!", true );
            }
            else if( reply == "Goodbye" ) {
                socketMethods.write( client, " Cruel World", true );
            }
            cout << "got message from client: " << reply << endl;

        }while( reply != "Goodbye" );
        close( client );
    }

    delete [] bigArray;
    return 0;
    
}
