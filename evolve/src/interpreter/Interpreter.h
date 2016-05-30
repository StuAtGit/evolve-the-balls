#ifndef STU_INTERPRETER_H
#define STU_INTERPRETER_H

#define DO_DEBUG true

#include "System.h"
#include "Logger.h"
#include "CommonAspect.h"
#include <string>
#include <map>
#include <vector>

using namespace std;
using namespace evolve;

namespace evolve 
{
/**
 * Provides an interface to an object that can interpret lSystems
 * The interpretation is a string (which will most likely need further interpretation)
 * The interpretation of a particular interpreter can be controlled by a set of attributes.
 * For example, in the generic interpreter, the attributes UpAngle, DownAngle, etc. control
 * the angle that a particular element in the lsystem will cause the system to turn.
 */
class Interpreter : public CommonAspect
{
public:
    virtual ~Interpreter() {};
    //virtual bindInterpreter( LSystem* lsystem, int port, bool isStatic, bool printDebug = DO_DEBUG ) = 0;
    /**
     * Retrieves the interpretation of the lsystem. The second version is ok for now, since 
     * the System interface requires getSystem to return a string version of the object.
     *
     * @param system the system being interpreted (as an object or in string form).
     *               Note that in some classes this may be a in parameter, in others, an out.
     *               (some implementations may associate themselves with only one particular lsystem
     *                e.g. to interpreter it incementally over time).
     *                These classes should just return the system they are currently interpreting.
    */
    virtual bool getSystem( string& interpretedSystem, System& system, bool printDebug = DO_DEBUG ) = 0;
    virtual bool getSystem( string& interpretedSystem, string& system, bool printDebug = DO_DEBUG ) = 0;
   /**
     * Saves the attributes of this interpreter to a file
     * @param filename the name of the file to write this interpreter out too
     */
    virtual bool saveAttributes( const string& filename, bool printDebug = DO_DEBUG ) = 0;
    /**
     * loads this interpeter from a file
     * @param filename the name of the file to load
     */
    virtual bool loadAttributes( const string& filename, bool printDebug = DO_DEBUG ) = 0;
    /**
     * Sets the attributes of this lsystem
     * @param attributes a map of the attributes with key->attribute name and value->attribute value
     */
    virtual bool setAttributes( const map<string,string>& attributes, bool printDebug = DO_DEBUG ) = 0;
    /**
     * Sets a particular attribute of this lSystem (creates it if it is not currently present)
     * @param attributeName the name of the attribute to set
     * @param attributeValue the value of the attribute to set
     */
    virtual bool setAttribute( const string& attributeName, const string& attributeValue, bool printDebug = DO_DEBUG ) = 0;
    /**
     * Sets the id of this interpreter
     * @param id the id to set it to
     */
    virtual bool setId( const string& id, bool printDebug = DO_DEBUG ) = 0;
    /**
     * Returns a map of the attributes of this intepreter
     * @return a map, key=name of attribute, value=value of attribute
     */
    virtual map<string,string> getAttributes( bool printDebug = DO_DEBUG ) = 0;
    /**
     * Returns the attribute value for a given attribute
     * @param attributeName
     * @return the value the attribute
     */
    virtual string getAttributeValue( const string& attributeName, bool printDebug = DO_DEBUG ) = 0;
    /**
     * Returns a description of the given attribute
     * @param attributeName the name of the attribute 
     * @return the description of the attribute
     */
    virtual string getAttributeDescription( const string& attributeName, bool printDebug = DO_DEBUG ) = 0;
    /**
     * Returns a list of supported attributes
     * @return supported attributes
     */
    virtual vector<string> getSupportedAttributes( bool printDebug = DO_DEBUG ) = 0;
    /**
     * Check if a particular attribute is supported
     * @param attributeName the name of the attribute to check
     * @return whether or not the attributes is supported
     */
    virtual bool isSupported( const string& attributeName, bool printDebug = DO_DEBUG ) = 0;
    /**
     * Creates a copy of the current interpreter.
     * Note that if you pass it a non-null pointer, it will attempt to delete it, 
     * if it doesn't point at allocated memory, this will make it seg fault. (This is to encourage
     * you to allocate and/or intitialize your pointers to NULL
     * @param interpreter a pointer at a copy of this lSystem (the memory is allocated in this method)
     */
    virtual bool getCopy( Interpreter*& interpreter, bool printDebug = DO_DEBUG ) = 0 ;
    /**
     * gets the id of this intepreter
     * return id
     */
    virtual string getId( bool printDebug = DO_DEBUG ) = 0;
    //virtual bool setIsStatic( bool isStatic, bool printDebug = DO_DEBUG ) = 0;
    //virtual bool getIsStatic( bool printDebug = DO_DEBUG ) = 0;
};
};
#endif
