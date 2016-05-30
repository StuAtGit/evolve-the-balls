#ifndef Stuart_Smith_MethodFactory_H
#define Stuart_Smith_MethodFactory_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include "AsmGenerator.h"
#include "Serializer.h"
#include "Timer.h"
#include "Logger.h"

#define DO_DEBUG true

using namespace std;
using namespace boost;

/**TODO: insert class comments*/
class MethodFactory
{
public:
    MethodFactory( void* obj );
    /**
    *
    */
    bool addMethod( const string& methodId, const string& methodSpecification, dword methodAddr,bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool callMethod( const string& methodId, unsigned char* ret, unsigned int argSize, unsigned int& retSize,bool printDebug = DO_DEBUG );
    /**
    *
    */
    map<string,string> getInterfaces( bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool parseSpecification( const string& spec, string& interface,bool printDebug = DO_DEBUG );
private:
};

#endif
