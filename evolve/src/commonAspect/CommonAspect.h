#ifndef STU_CommonClass_H
#define STU_CommonClass_H

#include <iostream>
#include <string>

#include "Logger.h"

#define DO_DEBUG true

using namespace std;
using namespace evolve;

/**
 * Evolve namespace. All objects gradually being moved into this.
 */
namespace evolve
{

/**
 * Various aspects of evolve objects that cut across 
 * all objects. Root of object hierachy.
 */
class CommonAspect 
{
public:
    CommonAspect();
    virtual ~CommonAspect() {};
    /**
    */
    bool setNotify( bool notify, bool printDebug = DO_DEBUG );
    /**
    */
    bool getNotify( bool printDebug = DO_DEBUG );
    /**
     * Returns a pointer at this logger object
     * Note that if you pass it a non-null pointer, it will attempt to delete it, if it doesn't
     * point at allocated memory, this will make it seg fault. Haven't decided if this is good
     * or bad behavior (it does encourage one to intialize their pointers!)
     * @param logger
     */
    bool getLogger( Logger*& logger, bool printDebug = DO_DEBUG );
    /*
     * Serialized the objects state into the given buffer
     * @param buf buffer into which the state is serialized (return value)
     * @param size the size of the state buffer
     * */
    virtual bool serialize( unsigned char* buf, unsigned int& size );
    /*
     * Re-build the objects state from the data buffer
     * @param buf the state buffer (as returned by serialize)
     * @param size the size of state buffer (for sanity checks)
     */
    virtual bool deserialize( const unsigned char* buf, unsigned int size );
protected:
    bool doNotify;
    Logger logger;
};
};
#endif
