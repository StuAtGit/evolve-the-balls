#ifndef STU_INTERPRETER_GENERIC_H
#define STU_INTERPRETER_GENERIC_H

#define DO_DEBUG true

#include "Interpreter.h"
#include "Matrix.h"
#include "Utility.h"
#include "LSystem.h"
#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;
using namespace evolve;

namespace evolve
{
    /**
     * A generic implementation of the interpreter interface. 
     * It interprets lsystems into a set of points, in a batch fashion, and recognizes the following elements:
     * <br/>U,D,L,R,S,[,]<br/>
     * Which are mapped to the attributes:
     * <ul>
     * <li>UpAngle</li>
     * <li>DownAngle</li>
     * <li>Left</li>
     * <li>Right</li>
     * </ul>
     * respectively.<br/>
     * The [,] elements are to support branching/nesting. S is explained below.<br/>
     *
     * The interpreter interprets each element as a point that is a certain distance (determined by the AtmoicSize attribute)
     * from the previous element. If the element is S, the point is placed a certain distance in the current direction of the branch.
     * If the element is U,D,L, or R, the point is placed a certain distance in the direction of the branch, offset by the respective angle set 
     * in the attributes.<br/>
     * e.g.<br/>
     * <table>
     * <tr>
     *  <td>S</td><td>S</td><td>L</td><td>R</td>
     * </tr>
     * <tr>
     *  <td>&nbsp;</td>&nbsp;<td>&nbsp;</td><td>.</td><td>&nbsp;\\</td>
     * </tr>
     * <tr>
     * <td>-&gt;.</td><td>-&gt;.</td><td>/</td><td>.</td>
     * </tr>
     * </table>
     */
    class LSystemGenericInterpreter : public Interpreter
    {
    public:
        LSystemGenericInterpreter( const string& id, bool printDebug = DO_DEBUG );
        virtual ~LSystemGenericInterpreter() {};
        virtual bool getSystem( string& interpretedSystem, System& lSystem, bool printDebug = DO_DEBUG );
        virtual bool getSystem( string& interpretedSystem, string& lSystem, bool printDebug = DO_DEBUG );
        virtual bool saveAttributes( const string& filename, bool printDebug = DO_DEBUG );
        virtual bool loadAttributes( const string& filename, bool printDebug = DO_DEBUG ); 
        virtual bool setAttributes( const map<string,string>& attributes, bool printDebug = DO_DEBUG );
        virtual bool setAttribute( const string& attributeName, const string& attributeValue, bool printDebug = DO_DEBUG );
        virtual bool setId( const string& id, bool printDebug = DO_DEBUG ); 
        virtual map<string,string> getAttributes( bool printDebug = DO_DEBUG );
        virtual string getAttributeValue( const string& attributeName, bool printDebug = DO_DEBUG );
        virtual string getAttributeDescription( const string& attributeName, bool printDebug = DO_DEBUG );
        virtual vector<string> getSupportedAttributes( bool printDebug = DO_DEBUG );
        virtual bool isSupported( const string& AttributeName, bool printDebug = DO_DEBUG );
        virtual bool getCopy( Interpreter*& interpreter, bool printDebug = DO_DEBUG );
        virtual string getId( bool printDebug = DO_DEBUG );
    
        static map<int, vector< vector<double> > > getPoints( const string& interpretedSystem, const Logger& logger, bool doNotify, bool printDebug = DO_DEBUG );
        static void printSystem( const string& interpretedSystem );
    
    protected:
        map<string,string> systemAttributes;
        string rightAngleKey;
        string leftAngleKey;
        string upAngleKey;
        string downAngleKey;
        string atomicSizeKey;
        string startPointKey;
        //not currently used, may be used for static/dynamic...
        string lSystemId;
        string id;
    };
};
#endif
