#ifndef SOCKETS_CLASS_STU_CC
#define SOCKETS_CLASS_STU_CC

#include "SocketMethods.h"
#include <sys/time.h>

SocketMethods::SocketMethods()
{
    this->doNotify = false;
    this->commands = Commands::getInstance();
    this->delimiter = this->commands->endMessageBlock;
}

bool SocketMethods::getServerSocket( int& sock, int port, bool printDebug )
{
    logger.print( "Notify: entered SocketMethods::getServerSocket", printDebug && this->doNotify );
    if( !this->bind( sock, port, printDebug ) )
    {
        logger.print( "Error: in socketMethods::getServerSocket, bindSocket failed", printDebug );
        return false;
    }

    if( !this->listen( sock, printDebug ) )
    {
        logger.print( "Error: in socketMethods::getServerSocket, connectSocket failed", printDebug ); 
        return false;
    }

    return true;
}

bool SocketMethods::getClientSocket( int& sock, string ip, int port, bool printDebug )
{
    logger.print( "Notify: entered SocketMethods::getClientSocket", printDebug && this->doNotify );
    return connect( sock, ip, port, printDebug );
}

int SocketMethods::openSocket( int domain, int type, int protocol, bool printDebug )
{
    logger.print( "Notify: entered openSocket", this->doNotify && printDebug  );

    int ret = C::socket( domain, type, protocol );
    if( ret < 0 )
    {
        logger.print( "Error: SocketMethods::openSocket failed: " + lexical_cast<string>(strerror( errno )), printDebug );
        return -1;
    }
    else
    {
        return ret;
    }
}

bool SocketMethods::connect( int& sock, const string& ip, int port, bool printDebug )
{
    logger.print( "Notify: entered connect", this->doNotify && printDebug );

    struct sockaddr_in addr;
    memset( &(addr), 0, sizeof( addr ) );
    addr.sin_family = AF_INET;
    addr.sin_port = htons( port );
    addr.sin_addr.s_addr = inet_addr( ip.c_str() );

    if( sock == -1 )
    {
        sock = this->openSocket( PF_INET, SOCK_STREAM, 0, printDebug );
        //openSocket should log the error
        if( sock < 0 )
            return false;
    }

    int connectStatus = C::connect( (int)sock, (struct sockaddr*)&(addr), sizeof( addr ) );

    if( connectStatus == -1 )
    {
        logger.print( "Error: connection to server" + lexical_cast<string>(strerror( errno )), printDebug );
        return false;
    }
    return true;
}

bool SocketMethods::bind( int& sock, int port, bool printDebug )
{
    logger.print( "Notify: entered bind", this->doNotify && printDebug );

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    int ret = -1;
    if( sock == -1 )
    {
        sock = this->openSocket( PF_INET, SOCK_STREAM, 0, printDebug );
        //openSocket logs the error
        if( sock < 0 )
            return false;
        int flag = 1;
        ret = setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof( flag ) );
        logger.print( "Warning: setsockopt( .. SO_REUSEADDR .. ) failed, attempting to continue", ret < 0 && printDebug );
    }
    
    ret = C::bind( sock, (struct sockaddr*)&addr, sizeof( addr ) ); 

    if( ret < 0 )
    {
        logger.print( "Error: Socket bind to port: " + lexical_cast<string>(port) + " failed: " + strerror( errno ), printDebug );
        return false; 
    }
    else
    {
        return true;
    }
}

bool SocketMethods::listen( int sock, bool printDebug )
{
    logger.print( "Notify: entered listen", this->doNotify && printDebug );

    int ret = C::listen( sock, 1 );
    if( ret < 0 )
    {
        logger.print( " Error: Socket error while preparing to listen: " + lexical_cast<string>( strerror( errno ) ), printDebug );
        return false;
    }

    return true;
}

int SocketMethods::accept( int servSocket, bool printDebug )
{
    logger.print( "Notify: entered accept", this->doNotify && printDebug );

   // struct sockaddr addrInfo;
    //should figure out what exactly the third
    //argument is supposed to be 
    //note, if you collect addrInfo
    //a bad address seems to cause it to exit (abruptly)
    //this is not really OK
    int ret = C::accept( servSocket, /*&addrInfo*/NULL, 0  );
    
    if( ret < 0 )
    {
        logger.print( "Error: Server socket error while accepting connections: " + lexical_cast<string>(strerror( errno )), printDebug );
    } 
    
    return ret;
}

bool SocketMethods::write( int servSocket, string message, bool printDebug )
{
    Timer timer;
    timer.startTimer( 0 );

    message += this->delimiter;
    logger.print( "Notify: entered SocketMethods::write, message is: " + message, printDebug && this->doNotify );

    int wrote = 0;
    while( wrote < (int)message.size() && wrote != -1) 
    {
        wrote = send( servSocket, message.c_str(), message.size(), 0x00000000 );
    }

    if( wrote == -1 )
    {
        logger.print( "Error: in write: " + lexical_cast<string>(strerror(errno)) + " perhaps the socket shut down unexpectedly?", printDebug );
        return false;
    }
    else if( wrote < (int)message.size() )
    {
        logger.print( "Error: write wrote less then message size", printDebug );
        return false;
    }

    timer.stopTimer( 0 );
    logger.print( "Notify: wrote message in: " + lexical_cast<string>( timer.getMicroseconds( 0 ) * 1e-6 ), this->doNotify && printDebug );
    return true;
    
}

bool SocketMethods::receive( int sock, string& message, bool printDebug )
{
    Timer timer;
    timer.startTimer( 0 );
    logger.print( "Notify: entered recieve", this->doNotify && printDebug );

    message = "";
    //first check to see if we have anything buffered for this socket
    //(if the whole (or start) of the message came in with the last recieve()
    if( sockBuffer[sock].length() > 0 )
    {
        //check if we only have a partial message buffered
        if( message.find( this->delimiter ) == string::npos )
        {
            message += sockBuffer[sock];
            sockBuffer[sock].clear();
        }
        else
        {
            message = this->getCurrentMessage( sockBuffer[sock], printDebug );
            unsigned int totalLength = message.length() + this->delimiter.length();
            //if there is STILL more left
            if( sockBuffer[sock].length() > totalLength )
            {
                sockBuffer[sock] = sockBuffer[sock].substr( totalLength, sockBuffer[sock].length() );
            }
            return true;
        }
    }
    int size = 10;
    while( message.find( this->delimiter ) == string::npos )
    {
        char buf[size];
        memset( buf, 0, sizeof(char) * size );
        int res = 1;
       //leave room for NULL character at end
        res = recv( sock, buf, size - 1, 0 );
  
        if( res == -1 )
        {
            logger.print( "Error: in recieve: " + lexical_cast<string>( strerror( errno ) )+ " (most likely this is normal if no system error printed) ", printDebug  );
            return false;
        }
        else if( res == 0 )
        {
            logger.print( "Notify: socket read 0, assuming closed at other end", printDebug && doNotify );
            return false;
        }
        else
        {
            //null terminate buffer
            if( res < size )
            {
                buf[res] = '\0';
            }
            else
            {
                buf[size - 1] = '\0';
            }
        }
        message += buf;
        logger.print( "Notify:: in recvSocket, message so far: " + message, printDebug && this->doNotify );
    }

    string tmp = this->getCurrentMessage( message, printDebug );
    //hmm. saw this case once - two messages sent, the beginning of the second in with the first
    if( message.length() > tmp.length() + this->delimiter.length() )
    {
        //save anything after the eom delimiter to the end of the string 
        //for the next time
        this->sockBuffer[sock] += message.substr( tmp.length() + this->delimiter.length(), message.length() ); 
    }

    message = tmp; 
    timer.stopTimer( 0 );
    logger.print( "Notify: wrote message in: " + lexical_cast<string>( timer.getMicroseconds( 0 ) * 1e-6 ), printDebug && doNotify );

    return true;
}

bool SocketMethods::sendCommandSequence( int sock, const vector<string>& commandSequence, vector<string>& replies, bool printDebug )
{
    logger.print( "Notify: entered SocketMethods::sendCommandSequence", this->doNotify && printDebug );
    for( unsigned int i = 0; i < commandSequence.size(); ++i )
    {
        bool success = false;
        string reply = "";
        success = sendCommand( sock, commandSequence[i], reply, printDebug );
        logger.print( "Notify: in SocketMethods::sendCommandSequence, sent: " + commandSequence[i] + " recieved: " + reply, this->doNotify && printDebug );
        replies.push_back( reply );
        if( !success )
        {
            logger.print( "Warning: in SocketMethods::handleCommand, sendCommand failed, terminating command sequence, at command: " + commandSequence[i]  + " with reply: " + reply );
            return false;
        }
    }
    return true;
}
   
bool SocketMethods::sendCommand( int sock, const string& message, string& reply, bool printDebug )
{
    logger.print( "Notify: entered SocketMethods::sendCommand", this->doNotify && printDebug );
    bool success = false;
    success = this->write( sock, message, printDebug ); 
    success = success && this->recieve( sock, reply, printDebug );
    if( !success )
    {
        logger.print( "Error: SocketMethods::sendCommand write/recieve error", printDebug );
        return false;
    }
    if( reply.find( "Error:" ) != string::npos )
    {
        logger.print( "Warning: in SocketMethods::sendCommand, reply was an error message: " + reply, printDebug );
        return false;
    }
    if( reply.find( "Warning:" ) != string::npos )
    {
        logger.print( "Warning: in SocketMethods::sendCommand, reply was a warning message: " + reply, printDebug );
        return false;
    }

    return true;
}

bool SocketMethods::setDelimiter( const string& delimiter, bool printDebug )
{
    logger.print( "Notify: entered setDelimiter", this->doNotify && printDebug );
    this->delimiter = delimiter;
    return true;
}

string SocketMethods::getCurrentMessage( string buffer, bool printDebug )
{
    int index = buffer.find( this->delimiter );
    return buffer.substr( 0, index );
}
#endif
