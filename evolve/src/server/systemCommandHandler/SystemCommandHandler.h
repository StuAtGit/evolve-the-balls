#ifndef STU_LSYSTEM_COMMAND_HANDLER_H
#define STU_LSYSTEM_COMMAND_HANDLER_H

#include "CommandHandler.h"
#include "System.h"
#include "LSystemGeneric.h"
#include "SocketMethods.h"
#include "Commands.h"
#include "Mutex.h"
#include "ReadWriteLock.h"

#define DO_DEBUG true
using namespace evolve;

//TODO: documentation? (look in in LSystem)
class EXPORT SystemCommandHandler : public CommandHandler 
{
public:
    LOCAL SystemCommandHandler();
    LOCAL virtual ~SystemCommandHandler() {};
    EXPORT virtual bool handleCommand( const string& command, const string& id, int sock, bool printDebug = DO_DEBUG );
    LOCAL bool handleCreate( const string& id, int sock, bool printDebug = DO_DEBUG );
    LOCAL bool handleGetSystem( const string& id, int sock, bool printDebug = DO_DEBUG );
    LOCAL bool handleRun( const string& id, int sock, bool printDebug = DO_DEBUG );
    LOCAL bool handleLoad( const string& id, int sock, bool printDebug = DO_DEBUG );
    LOCAL bool handleCross( const string& id, int sock, bool printDebug = DO_DEBUG );
    LOCAL bool handleMakeCopy( const string& id, int sock, bool printDebug = DO_DEBUG );
    LOCAL bool handleLoadSavedSystems( const string& id, int sock, bool printDebug = DO_DEBUG );

private:
    map<string,System*> systems;
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
