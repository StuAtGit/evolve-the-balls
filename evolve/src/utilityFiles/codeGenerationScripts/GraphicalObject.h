#ifndef STU_GraphicalObject_H
#define STU_GraphicalObject_H

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <fstream>
#include "Logger.h" 

#define DO_DEBUG true

using namespace std;

/**TODO: insert class comments*/
class GraphicalObject
{
public:
    /**
    *
    */
     GraphicalObject( bool printDebug = DO_DEBUG );
    /**
    *
    */
    virtual bool draw( bool printDebug = DO_DEBUG ) = 0;
    /**
    *
    */
    bool setPosition( bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool setPosition( bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool setRotation( bool printDebug = DO_DEBUG );
    /**
    *
    */
    vector<double> getPosition( bool printDebug = DO_DEBUG );
    /**
    *
    */
    double getRotation( bool printDebug = DO_DEBUG );
    void setLogfileName( string logFileName, bool printDebug = DO_DEBUG );
    void setNotify( bool notify, bool printDebug = DO_DEBUG );
private:
    bool doNotify;
    Logger logger;
};

#endif
