#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>

#include "SocketMethods.h"

using namespace std;
using namespace boost;

string shutdown_message = "SHUTDOWN";
string end_message = "END_M";
string startCommand = "start";
string createCommand = "create";
string stopCommand = "stop";
string printCommand = "print";
//get may be depracated
string getCommand = "get";
string destroyCommand = "destroy";
string getSystemCommand = "getSystem";

SocketMethods socketMethods;

///err... I don't think this is the same as the compiled test client
int main( int argc, char** argv )
{
    int port = 5012;
    if( argc > 1 )
    {
        port = atoi( argv[1] );
    }
    
    cout << "client connecting to localhost on port: " << port << endl;
   	 
    string command = "don't quit";
    string reply = "";
   
    int sock = -1;
    if( !socketMethods.getClientSocket( sock, "127.0.0.1", port, true ) )
    {
        cout << "bad client socket" << endl;
        return -1;
    }
    
    while( command != "quit" )
    {
        command = "";
        char c = ' ';
        do
        {
            cin.get( c );
            if( c != '\n' ){
                command += c;
            }

        }while( c != '\n' );

        if( command == "quit" )
            break;
        cout << "sending command: " << command << endl;
        socketMethods.write( sock, command, true );
        socketMethods.recieve( sock, reply, true );
        cout << "got reply: " << reply << endl;
    }

    close( sock );
	return 1;
}
//good example
/*
 Converts ascii text to in_addr struct.  NULL is returned if the 
   address can not be found. 
struct in_addr *atoaddr(char *address) {
  struct hostent *host;
  static struct in_addr saddr;

   First try it as aaa.bbb.ccc.ddd. 
  saddr.s_addr = inet_addr(address);
  if (saddr.s_addr != -1) {
    return &saddr;
  }
  host = gethostbyname(address);
  if (host != NULL) {
    return (struct in_addr *) *host->h_addr_list;
  }
  return NULL;
}
*/

//BROKE 
/* struct hostent* host = gethostbyaddr( (void*)inet_addr("192.168.1.103"), sizeof( "192.168.1.103" ), AF_INET ); 
 
    if( host == NULL )
    {
        cout << "error: " << strerror( h_errno ) << endl; 
        cout << "number: " << h_errno << endl;
        cout << "not found: " << HOST_NOT_FOUND << endl;
        cout << "no address: " << NO_ADDRESS << endl; 
        cout << "no recovery:  " << NO_RECOVERY << endl;
        cout << "try again: " << TRY_AGAIN << endl;
        return -1; 
    }
    
    cout << "host: " << host << endl; 
    cout << "host->h_addr: " << host->h_addr << endl;
    memcpy( &addr.sin_addr, host->h_addr, host->h_length );
    */
  
