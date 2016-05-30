#ifndef STU_SYSTEM_H
#define STU_SYSTEM_H

#include <fstream>
#include <map>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sys/time.h>
#include "CommonAspect.h"

using namespace std;
using namespace evolve;

#define DO_DEBUG true

namespace evolve
{
/**
 * Generic interface for systems
 * Base class for lsystems or whatever other 
 * representations that come about
 */
class System : public CommonAspect
{

 public:
    virtual ~System() {};
    /**
    Initialize the system from a file stream.
    The filestream is expected to point at a file.
    @param file the filestream to be read from
    */
    virtual bool initialize( ifstream& file, bool debug = DO_DEBUG ) = 0;
    /**
    Initialize the system from a file.
    The filename is to the name of a file in the lSystem save file format.
    @param file the name of the file to read from
    */
    virtual bool initialize( string file, bool debug = DO_DEBUG ) = 0;
    /**
     * Save the lsystems state to a file
     * @param filename the name of the file to save to
     */
    virtual bool save( string filename, bool printDebug = DO_DEBUG ) = 0;
    /**
    runs the system one step.
    @param request any requests you wish to make of the system
    @param reply any reply the system sends back to the caller
    */
    virtual bool run( const string& request, string& reply, bool printDebug = DO_DEBUG ) = 0; 
    /**
     * @return the id of this system as a string
     * */
    const virtual string getId( bool printDebug = DO_DEBUG ) = 0;
    /**
     * Returns a copy of this system. Not implemented as
     * copy constructor to allow for different behavior in case
     * of LSystemInterface, etc, where you don't want a copy of 
     * the object itself, rather it creates a remote copy on the
     * server and returns an object controlling that remote copy.
     * It returns a pointer so any LSystem type can be returned
     *
     * @param a pointer pointing at a <b>copy</b> of this lsystem.
     *        if this pointer is not set to null delete will be
     *        called on the pointer, and any info pointed at will
     *        be lost. In addtion, if it is not set to null, and
     *        points at nothing, undefined behavior may result.
     *        <b>Initialize your pointers!</b>
     * 
     * @return true on success, false on failure      
     * * */
    const virtual bool getCopy( System*& copy, bool printDebug = DO_DEBUG ) = 0;
    /**
     * Returns a copy of this system in string form
     */
    const virtual string getSystem( bool printDebug = DO_DEBUG ) = 0;
    /**
     * @param id the id to set this class to
     * @return if the id was set, true, false otherwise
     * */
    virtual bool setId( string id, bool printDebug = DO_DEBUG ) = 0;
    /**
    * Crosses this system with another system.
    *
    * The cross maps should be 
    * structured as follows:\n
    *        The key can be random:[number], [number]:random, random:random or [number]:[number], \n
    *        where the random keyword specifies the index to use should be choosen at random. \n
    *        Otherwise, the key is assumed to be a number that indicates the index to use. 
    *        The operation to the left of the colon always refers to this object,\n 
    *        while the operation on the right refers to the LSystem object being passed to the method. \n
    *        The value is an integer 1-3, that specifies which operation to use, as follows:\n
    *        1) move the element at the index in this system to the index in the other system (1->2)\n
    *        2) move the element at the index in the other system to this system (2->1) \n
    *        3) swap elements at this index\n
    *        So one element in this map should look like: (random:random,3)
    *        (swaps two random elements from each system) \n
    *
    * @param system the system to cross this system with (a pointer for downcasting)
    *
    * Note to extenders of this class... yes the interface isn't perfect, not too general, but
    * remember, you just need to respect the argument *types*, and you can change the 
    * semantics all you want.
    * Just be sure to doc your stuff..
    * This interface was built AFTER the lSystem class was, so that's
    * why it's the way it is
    *
    * May make own data type to pass as the cross map (that can be extended for each
    * type of sytem). That is, if it becomes necessary.
    **/
    virtual bool cross( System* system, const map<string,string>& crossMap, 
                        const map<string,string>& crossMap,
                        const map<string,string>& crossMap, 
                        const map<string,string>& crossMap, 
                        bool printDebug = DO_DEBUG ) = 0;

};
};
#endif
