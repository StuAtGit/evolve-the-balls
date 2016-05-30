#ifndef STU_LOGGER_H
#define STU_LOGGER_H

#include <iostream>
#include <fstream>

#define DO_DEBUG true

using namespace std;

namespace evolve
{
/**
A simple class that provides a logging facility 
for debug and notification messages.
Will eventually be phased into all other classes\n
Unless otherwise noted, methods which return bool return true 
on success, false on failure.\n
Unless otherwise noted, the printDebug argument turns debug
messages on or off.
*/

class Logger
{
    public:
        /**
        Construct a logger that writes to standard out
        */
        Logger();
        /**
        Contruct a logger that writes to a file
        */
        Logger( string logFileName );
        /**
        Print a message to the log.
        @param message the message to write
        @param printDebug Note that this variable determines an additional aspect
                          to it's normal role: if false, the method immediately returns
                          and does nothing.
        */
        const bool print( const string& message, bool printDebug = DO_DEBUG ) const;
        /**
        *  
        * Set the filename of the file to log to
        * Opens the file, if the file open is succesful, sets the
        * printToFile flag to true 
        * @param fileName the name of the file to log to
        */
        bool setLogFileName( string fileName, bool printDebug = DO_DEBUG );
        /**
        Sets the print to file flag (to tell a logger to write to a file)
        @param printToFile if true, will attempt to log to a file, if false
                           will log to standard out
        */
        void setPrintToFile( bool printToFile, bool printDebug = DO_DEBUG );
        /**
        Sets the notify flag
        @param doNotify turns notification messages on or off
        */
        void setNotify( bool doNotify, bool printDebug = DO_DEBUG );

        /**
        * Set whether or not the program running will exit on Warnings
        * @param doWarningExit whether or not to exit
        */
        void setExitOnWarnings( bool doWarningExit, bool printDebug = DO_DEBUG );

        /**
         * Set whether or not the program running will exit on Errors
         * @param doErrorExit whether or not to exit
         */
        void setExitOnErrors( bool doErrorExit, bool printDebug = DO_DEBUG );

    private:
        bool printToFile;
        bool doNotify;
        string logFileName;
        bool doExitOnWarnings;
        bool doExitOnErrors;
};
};
#endif
