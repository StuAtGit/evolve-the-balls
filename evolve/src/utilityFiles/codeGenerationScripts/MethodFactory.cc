#ifndef Stuart_Smith_MethodFactory_CC
#define Stuart_Smith_MethodFactory_CC

#include "MethodFactory.h"

MethodFactory::MethodFactory( void* obj )
{
    return;
}

bool MethodFactory::addMethod( const string& methodId, const string& methodSpecification, dword methodAddr, bool printDebug )
{
    logger.print( "Notify: entered method: bool addMethod(const string& methodId, constr string& methodSpecification, dword methodAddr, bool printDebug)  of class MethodFactory", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool MethodFactory::callMethod( const string& methodId, unsigned char* ret, unsigned int argSize, unsigned int& retSize, bool printDebug )
{
    logger.print( "Notify: entered method: bool callMethod(const string& methodId, unsigned char* ret, unsigned int argSize, unsigned int& retSize, bool printDebug)  of class MethodFactory", this->doNotify && printDebug );
    bool ret;
    return ret;
}

map<string,string> MethodFactory::getInterfaces(  bool printDebug )
{
    logger.print( "Notify: entered method: map<string,string> getInterfaces( bool printDebug)  of class MethodFactory", this->doNotify && printDebug );
    map<string,string> ret;
    return ret;
}

bool MethodFactory::parseSpecification( const string& spec, string& interface, bool printDebug )
{
    logger.print( "Notify: entered method: bool parseSpecification(const string& spec, string& interface, bool printDebug)  of class MethodFactory", this->doNotify && printDebug );
    bool ret;
    return ret;
}

#endif
