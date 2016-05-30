#ifndef SOCKETS_CLASS_STU_H
#define SOCKETS_CLASS_STU_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
//Because sometimes C++ is just fucking retarded...
namespace C
{
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <errno.h>
    #include <signal.h>
};
#include <boost/lexical_cast.hpp>
#include "CommonAspect.h"
#include "Timer.h"
#include "Commands.h"

using namespace std;
using namespace boost;
using namespace C;
using namespace evolve;

namespace evolve
{
/**
A class that wraps around socket methods
Provides a message boundary facility\n
Unless otherwised noted, a method that returns a bool
returns true on success, false on failure
This is a class (instead of the more apt abstraction, the namespace) because I want to do
stuff like inherit from the CommonAspect
Also, I think I like the idea of a delimiter on a per object basis?
This is still an open issue in my head though..
*/
class SocketMethods : public CommonAspect
{
public:
    SocketMethods();
    /**
     * Calls bind & listen, and sets up a TCP server socket listening on the given port
     * @param sock out param that returns the server socket. If the parameter passed is -1, this method 
     * also calls the openSocket() method to open the socket.
     * @param port the port you want to listen on
     */
    bool getServerSocket( int& sock, int port, bool printDebug );
    /**
    * Wrapper around connectSocket call (easy-to-remember name)
    * @param sock if sock is -1, openSocket is called. Out parameter that is the client socket.
    * @param ip the ip you wish to connect to. Eventually I should add resolution here too, but I
    * don't have any motivation for it yet.
    * @port the port you which to connect on
    */
    bool getClientSocket( int& sock, string ip, int port, bool printDebug );
    /**
    Opens a socket - currently just a wrapper around the POSIX socket() system call
    @param domain the domain to use. Reference your OS to see what is available
    @param type same as above
    @param protocal more of the same
    */
    int openSocket( int domain, int type, int protocol, bool printDebug );
    /**
    Connects a socket to an ip address and a port (this will be a client socket)
    @param sock the socket handle (created by the openSocket or socket() call)
    @param ip the ip address to connect too
    @param port the port to connect too
    */
    bool connect( int& sock, const string& ip, int port, bool printDebug );
    /**
    Binds a socket to a port (this will be a server socket)
    @param socket the socket handle
    @param port the port to bind too
    */
    bool bind( int& sock, int port, bool printDebug );
    /**
    Tells a socket to listen
    */
    bool listen( int sock, bool printDebug );
    /**
    Blocking accept call
    @return a socket descriptor that is a connection to the client
    */
    int accept( int sock, bool printDebug );
    /**
    Writes to the socket. Adds message delimiter to the socket
    @param servSocket the socket to write to. Does not need to be a server socket.
    @param message the message to send. Delimiter is added to end (so not a const&)
    */
    bool write( int sock, string message, bool printDebug );
    /**
    Blocking receive call. Reads from a socket until a message delimiter is hit,
    or an error occurs. Eats the message delimiter from the recieve buffer.
    @param servSocket the socket to read from. Not necessarily a server socket.
    @param message the message recieved (return argument)
    */
    bool receive( int sock, string& message, bool printDebug );
    /**
     * i before e except after .... fuck it
     */
    bool recieve( int sock, string& message, bool printDebug ) { return receive( sock, message, printDebug ); }
    /**
     */
    bool sendCommandSequence( int sock, const vector<string>& commandSequence, vector<string>& replies, bool printDebug );  
    /**
     */
    bool sendCommand( int sock, const string& message, string& reply, bool printDebug );
    /**
     * Sets the message delimiter for write and recv 
     */
    bool setDelimiter( const string& delimiter, bool printDebug );

private:
    //extracts the first message from buffer (based on the current delimiter
    string getCurrentMessage( string buffer, bool printDebug );
    string delimiter; 
    //handles the case where two messages are in one recieve
    //(just store the second in the buffer, and look at it next time
    map<int,string> sockBuffer;
    Commands* commands;
};

};
#endif
