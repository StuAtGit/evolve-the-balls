#ifndef STU_COMMANDHANDLER_CC
#define STU_COMMANDHANDLER_CC

#include "CommandHandler.h"

CommandHandler* createCommandHandlerObject()
{
    CommandHandler* obj = new CommandHandler();
    return obj;
}

bool destroyCommandHandlerObject( CommandHandler*& obj )
{
    if( obj == NULL )
    {
        return false;
    }

    //This check is fairly pointless for the parent class but is _highly_ recommended for child classes!
    CommandHandler* def_obj = dynamic_cast<CommandHandler*>(obj);

    if( def_obj == NULL )
    {
        cout << "Warning: passed a non-CommandHandler object to a destroyCommandHandlerObject that expected a CommandHandler, check to make sure your not trying to load multiple command handler libraries" << endl;
        cout << "Warning: not deleting" << endl;
        return false;
    }

    delete def_obj;
    obj = NULL;
    return true;
}

CommandHandler::CommandHandler()
{
    this->commands = Commands::getInstance();
}

bool CommandHandler::handleCommand( const string& command, const string& id, int sock, bool printDebug )
{
    logger.print( "Notify: entered DefaultCommandHandler::handleCommand", this->doNotify && printDebug );
    map<string,string> cont;
    checkIdDoesNotExist<string>( "fred", cont, "blah", sock, printDebug );
    return this->socketMethods.write( sock, "Notify: sucessfully invoked the default command handler (NOP)", printDebug );
}


bool CommandHandler::lock( const string& id, bool printDebug )
{
    this->logger.print( "Notify: entered CommandHandler::lock", this->doNotify && printDebug );
    bool success = false;
    success = this->readWriteLock.readLock();
    //short circuit evaluation hack around if statements
    success = success && this->mutexes[id].lock();
    this->logger.print( "Error: CommandHandler::lock failed", !success && printDebug );
    return success;
}

bool CommandHandler::unlock( const string& id, bool printDebug )
{
    this->logger.print( "Notify: entered CommandHandler::unlock", this->doNotify && printDebug );
    //Well, at least we ensure they called lock() at least once... sort of
    if( this->mutexes.find( id ) == this->mutexes.end() )
    {
        this->logger.print( "Warning: In CommandHandler::unlock, you called CommandHandler::unlock without calling lock.. didn't you???" );
        return false;
    }

    bool success = false;
    success = this->readWriteLock.unlock();
    //short circuit evaluation hack around if statements
    success = success && this->mutexes[id].unlock();
    this->logger.print( "Error: Handler::unlock failed, this could lead to lock ups", !success && printDebug );
    return success;
}

bool CommandHandler::replyToUser( int sock, string successMsg, string errorMsg, bool success, bool printDebug )
{
    this->logger.print( "Notify: entered CommandHandler::replyToUser", this->doNotify && printDebug );
    bool wrote = false;
    if( success )
    {
        wrote = this->socketMethods.write( sock, successMsg, printDebug );
    }
    else
    {
        logger.print( errorMsg, printDebug );
        wrote = this->socketMethods.write( sock, errorMsg, printDebug );
    }

    if( !wrote )
    {
        logger.print( "Warning: write to socket: " + lexical_cast<string>(sock) + " failed", printDebug );
    }

    return wrote;
}

#endif
