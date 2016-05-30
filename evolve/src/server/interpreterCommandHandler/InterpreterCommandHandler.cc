#ifndef STU_InterpreterCommandHandler_CC
#define STU_InterpreterCommandHandler_CC

#include "InterpreterCommandHandler.h"

CommandHandler* createCommandHandlerObject()
{
    CommandHandler* obj = new InterpreterCommandHandler();
    return obj;
}

bool destroyCommandHandlerObject( CommandHandler*& obj )
{
    if( obj == NULL )
    {
        return false;
    }

    InterpreterCommandHandler* lsys_obj = dynamic_cast<InterpreterCommandHandler*>(obj);

    if( lsys_obj == NULL )
    {
        cout << "Warning: passed a non-InterpreterCommandHandler object to destroySCommandHandlerObject for InterpreterCommandHandler" << endl;        cout << "Warning: not deleting" << endl;
        return false;
    }

    delete lsys_obj;
    obj = NULL;
    return true;
}


InterpreterCommandHandler::InterpreterCommandHandler()
{
    this->commands = Commands::getInstance();
    return;
}

InterpreterCommandHandler::~InterpreterCommandHandler()
{
    return;
}

bool InterpreterCommandHandler::handleCommand( const string& command, const string& id, int sock, bool printDebug )
{
    logger.print( "Notify: entered method: virtual bool InterpreterCommandHandler::handleCommand(const string& command, const string& id, int sock,  bool printDebug)  of class InterpreterCommandHandler", this->doNotify && printDebug );
    if( command == this->commands->createCommand )
    {
        return this->handleCreate( id, sock, printDebug );
    }
    else if( command == this->commands->loadCommand )
    {
        return this->handleLoad( id, sock, printDebug );
    }
    else if( command == this->commands->getSystemCommand )
    {
        return this->handleGetSystem( id, sock, printDebug );
    }
    else if( command == this->commands->makeCopyCommand )
    {
        return this->handleMakeCopy( id, sock, printDebug );
    }
    else if( command == this->commands->setAttributeIntCommand )
    {
        return this->handleSetAttribute( id, sock, printDebug );
    }
    else if( command == this->commands->getAttributeIntCommand )
    {
        return this->handleGetAttributeValue( id, sock, printDebug );
    }
    this->socketMethods.write( sock, "Warning: unrecognized comand: " + command + " passed to server", printDebug );
    return false;
}

bool InterpreterCommandHandler::handleCreate( const string& id, int sock, bool printDebug )
{
    logger.print( "Notify: entered method: bool InterpreterCommandHandler::handleCreate(const string& id, int sock,  bool printDebug)  of class InterpreterCommandHandler", this->doNotify && printDebug );
    if( !this->checkIdDoesNotExist<Interpreter*>( id, this->interpreters, "createSystem", sock, printDebug ) )
    {
        return false;
    }

    this->readWriteLock.writeLock();
    this->interpreters[id] = new LSystemGenericInterpreter( id );
    this->readWriteLock.unlock();
    this->socketMethods.write( sock, "Notify: successfully created an interpreter with the id: " + id, printDebug );
    return true;
}

bool InterpreterCommandHandler::handleLoad( const string& id, int sock, bool printDebug )
{
    logger.print( "Notify: entered method: LOCAL bool handleGetSystem(const string& id, int sock,  bool printDebug)  of class InterpreterCommandHandler", this->doNotify && printDebug );
    if( !this->checkIdExists<Interpreter*>( id, this->interpreters, "loadSystem", sock, printDebug ) )
    {
        return false;
    }

    //query client for the filename
    string filename = id + ".attr";
    this->lock( id, printDebug );
    bool loaded = this->interpreters[id]->loadAttributes( filename, printDebug );
    this->unlock( id, printDebug );
    string successMessage = "Notify: successfully load attribute file: " + filename + " for interpreter: " + id;
    string errorMessage = "Warning: failed to load attribute file: " + filename + " for interpreter: " + id;
    replyToUser( sock, successMessage, errorMessage, loaded, printDebug );
    return loaded;
}

bool InterpreterCommandHandler::handleGetSystem( const string& id, int sock, bool printDebug )
{
    logger.print( "Notify: entered method: LOCAL bool handleGetSystem(const string& id, int sock,  bool printDebug)  of class InterpreterCommandHandler", this->doNotify && printDebug );
    bool success = false;
    string system = "";
    success = this->checkIdExists<Interpreter*>( id, this->interpreters, "getSystem", sock, printDebug );
    //Note that the system we are trying to retrieve is the lsystem
    success = success && getBasicType<string>( system, this->commands->systemRequest, sock, printDebug ); 
    if( !success )
    {
        return false;
    }
    string interpretedSystem = "";
    this->lock( id, printDebug );
    success = this->interpreters[id]->getSystem( interpretedSystem, system, printDebug );
    this->unlock( id, printDebug );
    string successMessage = interpretedSystem;
    string errorMessage = "Warning: failed to interpret the given system: " + system + " using the interpreter: " + id;
    replyToUser( sock, successMessage, errorMessage, success, printDebug );
    return success;
}

bool InterpreterCommandHandler::handleMakeCopy( const string& id, int sock,  bool printDebug )
{
    logger.print( "Notify: bool InterpreterCommandHandler::handleMakeCopy(const string& id, int sock,  bool printDebug)  of class InterpreterCommandHandler", this->doNotify && printDebug );
    bool success = false;
    string copyId = "";
    success = this->checkIdExists<Interpreter*>( id, this->interpreters, "makeCopy, original's id", sock, printDebug );
    success = success && getBasicType<string>( copyId, this->commands->systemIdRequest, sock, printDebug );
    success = success && checkIdDoesNotExist<Interpreter*>( copyId, this->interpreters, "makeCopy, new id", sock, printDebug );

    if( !success )
    {
        return false;
    }
    Interpreter* copy = NULL;
    this->lock( id, printDebug );
    success = this->interpreters[id]->getCopy( copy, printDebug );
    this->unlock( id, printDebug );
    
    if( !success )
    {
        string msg = "Error: InterpreterCommandHandler::handleMakeCopy failed";
        logger.print( msg, printDebug );
        this->socketMethods.write( sock, msg, printDebug );
        return false;
    }

    this->readWriteLock.writeLock();
    this->interpreters[copyId] = copy;
    this->interpreters[copyId]->setId( copyId );
    this->readWriteLock.unlock();
    string msg = "Notify: successfully created a new copy of the interpreter: " + id + " with a new id of: " + copyId;
    return this->socketMethods.write( sock, msg, printDebug );
}

bool InterpreterCommandHandler::handleSetAttribute( const string& id, int sock,  bool printDebug )
{
    logger.print( "Notify: bool Interpreter::handleSetAttribute(const string& id, int sock,  bool printDebug)  of class InterpreterCommandHandler", this->doNotify && printDebug );

    bool success = false;
    string attributeName = "";
    string attributeValue = "";
    success = checkIdExists<Interpreter*>( id, this->interpreters, "setAttribute", sock, printDebug );
    success = success && getBasicType<string>( attributeName, this->commands->getAttributeNameIntRequest, sock, printDebug );
    success = success && getBasicType<string>( attributeValue, this->commands->getAttributeValueIntRequest, sock, printDebug );
    if( !success )
    {
        return false;
    }

    this->lock( id, printDebug );
    success = this->interpreters[id]->setAttribute( attributeName, attributeValue, printDebug );
    this->unlock( id, printDebug );
    string successMsg = "Notify: successfully set attribute: " + attributeName + " to: " + attributeValue + " for interpreter: " + id;
    string failureMsg = "Error: setAttribute failed on interpreter: " + id;
    replyToUser( sock, successMsg, failureMsg, success, printDebug );
    return success;
}

bool InterpreterCommandHandler::handleGetAttributeValue( const string& id, int sock,  bool printDebug )
{
    logger.print( "Notify: entered method: bool InterpreterCommandHandler::handleGetAttribute(const string& id, int sock,  bool printDebug)  of class InterpreterCommandHandler", this->doNotify && printDebug );
    bool success = false;
    string attributeName = "";
    success = checkIdExists<Interpreter*>( id, this->interpreters, "getAttribute", sock, printDebug );
    success = success && getBasicType<string>( attributeName, this->commands->getAttributeNameIntRequest, sock, printDebug );
    if( !success )
    {
        return false;
    }
    this->lock( id, printDebug );
    string attributeValue = this->interpreters[id]->getAttributeValue( attributeName, printDebug );
    this->unlock( id, printDebug );
    this->socketMethods.write( sock, attributeValue, printDebug );
    return true;
}

#endif
