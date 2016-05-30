#ifndef STU_InterpreterCommandHandler_H
#define STU_InterpreterCommandHandler_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include "Interpreter.h"
#include "LSystemGenericInterpreter.h"
#include "Timer.h"
#include "Logger.h"
#include "Commands.h"
#include "SocketMethods.h"
#include "CommandHandler.h"
#include "LSystemGeneric.h"

#define DO_DEBUG true

using namespace std;
using namespace boost;
using namespace evolve;

/**TODO: insert class comments*/
class EXPORT InterpreterCommandHandler : public CommandHandler
{
public:
    LOCAL InterpreterCommandHandler();
    /**
    *
    */
    LOCAL virtual ~InterpreterCommandHandler();
    /**
    *
    */
    EXPORT virtual bool handleCommand( const string& command, const string& id, int sock, bool printDebug = DO_DEBUG );
    /**
    *
    */
    LOCAL bool handleCreate( const string& id, int sock, bool printDebug = DO_DEBUG );
    /**
     * 
     */
    LOCAL bool handleLoad( const string& id, int sock, bool printDebug = DO_DEBUG );
    /**
    * Retrieves the interpreted version of a system that is supplied via the client
    * May want a way of doing this without bouncing stuff around?
    */
    LOCAL bool handleGetSystem( const string& id, int sock, bool printDebug = DO_DEBUG );
    /**
    *
    */
    LOCAL bool handleMakeCopy( const string& id, int sock, bool printDebug = DO_DEBUG );
    /**
    *
    */
    LOCAL bool handleSetAttribute( const string& id, int sock, bool printDebug = DO_DEBUG );
    /**
    *
    */
    LOCAL bool handleGetAttributeValue( const string& id, int sock, bool printDebug = DO_DEBUG );
private:
    //methods
    //members
    map<string,Interpreter*> interpreters;
};

/**
 * Don't forget to cast the object to the right type before deleting
 * the name must remain the same, so the server can always look up the right symbol
 * This means you can't have two handler libraries loaded at the same time, because
 * the symbols would conflict
 */
extern "C"
{
    /**
     * Entry points in the shared object to deal with name-mangling, creates
     * a DefaultCommandHanlder object and returns it
     * */
    EXPORT CommandHandler* createCommandHandlerObject();
    /**
     * Entry point in the dll to destroy the object, attempts to downcast the object
     * to the correct type (in this case, a DefaultCommandHanlder)
     * and then delete it using the appropriate destructor
     */
    EXPORT bool destroyCommandHandlerObject( CommandHandler*& obj );
}

#endif
