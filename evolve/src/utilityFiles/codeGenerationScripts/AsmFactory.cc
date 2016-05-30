#ifndef Stuart_Smith_AsmFactory_CC
#define Stuart_Smith_AsmFactory_CC

#include "AsmFactory.h"

AsmFactory::AsmFactory(  )
{
    return;
}

bool AsmFactory::addPop(  bool printDebug )
{
    logger.print( "Notify: entered method: bool addPop( bool printDebug)  of class AsmFactory", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool AsmFactory::addInstructionWithOperand( opcode op, dword operand, bool printDebug )
{
    logger.print( "Notify: entered method: bool addInstructionWithOperand(opcode op, dword operand, bool printDebug)  of class AsmFactory", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool AsmFactory::addMovToEax( dword memAddress, bool printDebug )
{
    logger.print( "Notify: entered method: bool addMovToEax(dword memAddress, bool printDebug)  of class AsmFactory", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool AsmFactory::addMovMemToReg( dword memAddress, register dest, register tmp, bool printDebug )
{
    logger.print( "Notify: entered method: bool addMovMemToReg(dword memAddress, register dest, register tmp, bool printDebug)  of class AsmFactory", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool AsmFactory::addPush(  bool printDebug )
{
    logger.print( "Notify: entered method: bool addPush( bool printDebug)  of class AsmFactory", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool AsmFactory::addCall( dword callAddr, bool printDebug )
{
    logger.print( "Notify: entered method: bool addCall(dword callAddr, bool printDebug)  of class AsmFactory", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool AsmFactory::addReturn(  bool printDebug )
{
    logger.print( "Notify: entered method: bool addReturn( bool printDebug)  of class AsmFactory", this->doNotify && printDebug );
    bool ret;
    return ret;
}

#endif