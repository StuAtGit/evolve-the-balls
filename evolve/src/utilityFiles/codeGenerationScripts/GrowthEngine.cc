#ifndef Stu_GrowthEngine_CC
#define Stu_GrowthEngine_CC

#include "GrowthEngine.h"

GrowthEngine::GrowthEngine(  )
{
    return;
}

bool GrowthEngine::grow( unsigned int steps, bool printDebug )
{
    logger.print( "Notify: entered method: bool grow(unsigned int steps, bool printDebug)  of class GrowthEngine", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool GrowthEngine::addSystem( const string& id, const System& system, bool printDebug )
{
    logger.print( "Notify: entered method: bool addSystem(const string& id, const System& system, bool printDebug)  of class GrowthEngine", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool GrowthEngine::addInterpreter( const string& id, const Interpreter& interpreter, bool printDebug )
{
    logger.print( "Notify: entered method: bool addInterpreter(const string& id, const Interpreter& interpreter, bool printDebug)  of class GrowthEngine", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool GrowthEngine::getSystem( const string& id, System& system, bool printDebug )
{
    logger.print( "Notify: entered method: bool getSystem(const string& id, System& system, bool printDebug)  of class GrowthEngine", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool GrowthEngine::getInterpreter( const string& id, Interpreter& interpreter, bool printDebug )
{
    logger.print( "Notify: entered method: bool getInterpreter(const string& id, Interpreter& interpreter, bool printDebug)  of class GrowthEngine", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool GrowthEngine::addWorldObject( const string& id, const WorldObject& worldObject, bool printDebug )
{
    logger.print( "Notify: entered method: bool addWorldObject(const string& id, const WorldObject& worldObject, bool printDebug)  of class GrowthEngine", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool GrowthEngine::getWorldObject( const string& id, WorldObject& worldObject, bool printDebug )
{
    logger.print( "Notify: entered method: bool getWorldObject(const string& id, WorldObject& worldObject, bool printDebug)  of class GrowthEngine", this->doNotify && printDebug );
    bool ret;
    return ret;
}

#endif