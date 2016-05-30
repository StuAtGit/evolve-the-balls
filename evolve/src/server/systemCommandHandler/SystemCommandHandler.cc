#ifndef STU_LSYSTEM_COMMAND_HANDLER_CC
#define STU_LSYSTEM_COMMAND_HANDLER_CC

#include "SystemCommandHandler.h"

CommandHandler* createCommandHandlerObject()
{
    CommandHandler* obj = new SystemCommandHandler();
    return obj;
}

bool destroyCommandHandlerObject( CommandHandler*& obj )
{
    if( obj == NULL )
    {
        return false;
    }

    SystemCommandHandler* lsys_obj = dynamic_cast<SystemCommandHandler*>(obj);

    if( lsys_obj == NULL )
    {
        cout << "Warning: passed a non-SystemCommandHandler object to destroySystemCommandHandlerObject" << endl;
        cout << "Warning: not deleting" << endl;
        return false;
    }

    delete lsys_obj;
    obj = NULL;
    return true;
}

SystemCommandHandler::SystemCommandHandler()
{
}

bool SystemCommandHandler::handleCommand( const string& command, const string& id, int sock, bool printDebug )
{

    if( command == this->commands->createCommand )
    {
        return this->handleCreate( id, sock, printDebug );
    }
    else if( command == this->commands->getSystemCommand )
    {
        return this->handleGetSystem( id, sock, printDebug );
    }
    else if( command == this->commands->runCommand )
    {
        return this->handleRun( id, sock, printDebug );
    }
    else if( command == this->commands->loadCommand )
    {
        return this->handleLoad( id, sock, printDebug );
    }
    else if( command == this->commands->crossCommand )
    {
        return this->handleCross( id, sock, printDebug );
    }
    else if( command == this->commands->makeCopyCommand )
    {
        return this->handleMakeCopy( id, sock, printDebug );
    }
    
    string errorMsg = "Warning: in SystemCommandHandler::handleCommand you sent me a command: " + command + " that cannot be handled by this command handler";
    logger.print( errorMsg );
    return this->socketMethods.write( sock, errorMsg, printDebug ); 
}

bool SystemCommandHandler::handleCreate( const string& id, int sock, bool printDebug )
{
    logger.print( "Notify: entered SystemCommandHandler::handleCreate", this->doNotify && printDebug );

    if( !checkIdDoesNotExist<System*>( id, this->systems, "SystemCommandHandler::handleCreate", sock, printDebug ) )
    {
        return false;
    }

    //TODO... expand this command to handle different types of systems?
    //Must be done in a fairly dynamic fashion, though...
    //look up dp's Factory Method

    //We need to write lock the WHOLE container because we are writing to the container,
    //not just the lsystem
    this->readWriteLock.writeLock();
    this->systems[id] = new LSystemGeneric();
    this->systems[id]->setId( id );
    this->readWriteLock.unlock();
    return this->socketMethods.write( sock, "Notify: successfully created System with an id of: " + id, printDebug );
}

bool SystemCommandHandler::handleGetSystem( const string& id, int sock, bool printDebug )
{
    logger.print( "Notify: entered SystemCommandHandler::handleGetSystem", this->doNotify && printDebug );
    if( !checkIdExists<System*>(  id, this->systems, "System::CommandHandler handleGetSystem", sock, printDebug ) )
    {
        return false;
    }
    cout << "getting system: " << id << endl;
    this->lock(  id, printDebug ); 
    string systemString = this->systems[id]->getSystem( printDebug );
    this->unlock(  id, printDebug );
    return this->socketMethods.write( sock, systemString, printDebug );
}

bool SystemCommandHandler::handleRun( const string& id, int sock, bool printDebug )
{
    logger.print( "Notify: entered SystemCommandHandler::handleEvolve", this->doNotify && printDebug );
    unsigned int iterations = 0xffffffff; 
    string mode = "";
    bool success = false;
    success = checkIdExists<System*>(  id, this->systems, "SystemCommandHandler::handleEvolve", sock, printDebug );
    success = success && getBasicType<unsigned int>( iterations, this->commands->runIterationRequest, sock, printDebug );
    success = success && getBasicType<string>( mode, this->commands->getModeRequest, sock, printDebug );

    if( !success )
    {
        return false;
    }
   
    //reserved for future use ;)
    string query = "";
    for( unsigned int i = 0; i < iterations; ++i )
    {
        this->lock(  id, printDebug );
        //run can dump A LOT of output with debug on (not technically a notify, but ... ) 
        bool success = this->systems[id]->run( mode, query, this->doNotify && printDebug );
        this->unlock(  id, printDebug );

        if( query != "" )
        {
            //handleQuery() ? someday...
            //and then keep evolving...
        }
        else if( !success )
        {
            string error = "Warning: evolve signaled error on the " + lexical_cast<string>( i ) + " iteration"; 
            logger.print( error, printDebug ); 
            this->socketMethods.write( sock, error, printDebug );
            return false;
        }
    }
    return this->socketMethods.write( sock, "Notify: ran system successfully for " + lexical_cast<string>( iterations ) + " iterations", printDebug );
}

bool SystemCommandHandler::handleLoad( const string& id, int sock, bool printDebug )
{
    logger.print( "Notify: entered SystemCommandHandler:handleLoad", this->doNotify && printDebug );
    if( !checkIdExists<System*>( id, this->systems, "handleLoad", sock, printDebug ) )
    {
        return false;
    }
   
    //I should really just ask for the filename...
    string filename = id + ".sys";
    this->lock( id, printDebug );
    bool loaded = this->systems[id]->initialize( filename, printDebug );
    this->unlock( id, printDebug );

    string successMessage = "Notify: sucessfully loaded system with id: " + id + " from save file: " + filename;
    string errorMessage = "Warning: failed to load system with id: " + id + " from save file: " + filename;
    replyToUser( sock, successMessage, errorMessage, loaded, printDebug );
    return loaded;
}

bool SystemCommandHandler::handleCross( const string& id, int sock, bool printDebug )
{
    logger.print( "Notify: entered SystemCommandHandler:handleCross", this->doNotify && printDebug );

    /** TODO: Get the id of the other system, and all the cross maps... :X */
    /** don't forget to use getMap & getBasicType */
    string crossId = "";
    map<string,string> systemCrossMap;
    map<string,string> optionsCrossMap;
    map<string,string> mutationCrossMap;
    map<string,string> growthRulesCrossMap;
    bool success = false;

    //short circuit evaluation to avoid if statements
    success = checkIdExists<System*>( id, this->systems, "handleCross", sock, printDebug );
    success = success && getBasicType<string>( crossId, this->commands->systemIdRequest, sock, printDebug  );
    success = success && checkIdExists<System*>( crossId, this->systems, "handleCross, getting cross id", sock, printDebug );
    success = success && getMap<string, string>( systemCrossMap, this->commands->systemCrossMapRequest, sock, printDebug  );
    success = success && getMap<string, string>( optionsCrossMap, this->commands->optionsCrossMapRequest, sock, printDebug  );
    success = success && getMap<string, string>( mutationCrossMap, this->commands->mutationProbabilitiesCrossMapRequest, sock, printDebug  );
    success = success && getMap<string, string>( growthRulesCrossMap, this->commands->growthRulesCrossMapRequest, sock, printDebug  );

    //if any of the above calls failed, bail out (the get functions handle error logging, etc
    if( !success )
    {
        return false;
    }

    this->lock( id, printDebug );
    if( id != crossId )
    {
        this->lock( crossId, printDebug );
    }
    success = this->systems[id]->cross( this->systems[crossId], systemCrossMap, optionsCrossMap, mutationCrossMap, growthRulesCrossMap );
    if( id != crossId )
    {
        this->unlock(  crossId, printDebug );
    }
    this->unlock( id, printDebug );
   
    string errorMessage = "Error: cross of system: " + id + " and system: " + crossId + " failed"; 
    string successMessage = "Notify: cross of system: " + id + " and system: " + crossId + " succeeded";
    replyToUser( sock, successMessage, errorMessage, success, printDebug );
    return success;
}

bool SystemCommandHandler::handleMakeCopy( const string& id, int sock, bool printDebug )
{
    logger.print( "Notify: entered SystemCommandHandler::handleMakeCopy" );
    string newId = "";
    bool success = false;
    success = checkIdExists<System*>( id, this->systems, "makeCopy, id to be copied", sock, printDebug );
    success = success && getBasicType<string>( newId, this->commands->systemIdRequest, sock, printDebug  );
    success = success && checkIdDoesNotExist<System*>( newId, this->systems, "SystemCommandHandler::handleMakeCopy", sock, printDebug );

    if( !success )
    {
        return false;
    }

    System* copy = NULL;
    this->lock(  id, printDebug );
    if( !this->systems[id]->getCopy( copy ) )
    {
        this->unlock(  id, printDebug );
        string message = "Error: in lSystemCommandHandler::handleMakeCopy getCopy failed";
        logger.print( message );
        this->socketMethods.write( sock, message, printDebug );
        return false;
    }
    this->unlock( id, printDebug );
    this->readWriteLock.writeLock();
    this->systems[newId] = copy;
    this->systems[newId]->setId( newId );
    this->readWriteLock.unlock();
    string message = "Notify: successfully made a copy of lsystem with id: " + id + " into a new system with id: " + newId;
    return this->socketMethods.write( sock, message, printDebug ); 
}

bool SystemCommandHandler::handleLoadSavedSystems( const string& id, int sock, bool printDebug )
{
    logger.print( "Notify: entered SystemCommandHandler::handlerLoadSavedSystems", this->doNotify && printDebug );
    vector<string> lSystemSaveFilenames = Utility::globFiles( "*.sys", printDebug );

    for( unsigned long i = 0; i < lSystemSaveFilenames.size(); ++i )
    {
        unsigned long indexOfDot = lSystemSaveFilenames[i].find_last_of( '.' );
        if( indexOfDot == string::npos )
        {
            logger.print( "Warning: in SystemCommandHandler::handleLoadSavedSystems, something weird is going on with glob or find_last_of", printDebug );
            continue;
        }
        string id = lSystemSaveFilenames[i].substr( 0, indexOfDot );
        bool success = false;
        //think about how I want to do this.... create & load want to talk to the client
        //success = success && createSystem( id, printDebug );
        //success = success && loadSystem( id, printDebug );
        logger.print( "Warning: SystemServer:loadSavedSystems, failed to create & load system from file: " +
                      lSystemSaveFilenames[i], !success && printDebug ); 
        //if one fails... now what?
        //do we really want to overwrite already loaded systems?
    }
    return false;
}

#endif 
