#ifndef Stuart_Smith_AsmFactory_H
#define Stuart_Smith_AsmFactory_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Timer.h"
#include "Logger.h"

#define DO_DEBUG true

using namespace std;
using namespace boost;

/**TODO: insert class comments*/
class AsmFactory
{
public:
    AsmFactory(  );
    /**
    *
    */
    bool addPop( bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool addInstructionWithOperand( opcode op, dword operand,bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool addMovToEax( dword memAddress,bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool addMovMemToReg( dword memAddress, register dest, register tmp,bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool addPush( bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool addCall( dword callAddr,bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool addReturn( bool printDebug = DO_DEBUG );
private:
};

#endif