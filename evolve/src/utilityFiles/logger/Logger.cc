#ifndef STU_LOGGER_CC
#define STU_LOGGER_CC

#include "Logger.h"

using namespace std;
using namespace evolve;

Logger::Logger()
{
   printToFile = false;
   doNotify = false;
   doExitOnWarnings = false;
   doExitOnErrors = false;
   logFileName = "";
}

Logger::Logger( string logFileName )
{
    printToFile = true;
    this->logFileName = logFileName;
    doNotify = false;
}

const bool Logger::print( const string& message, bool printDebug ) const
{
    if( this->doNotify )
    {
        cout << "Notify: logger entered print" << endl;
    }

    if( !printDebug )
    {
        return true;
    }

    ofstream fout;
    if( this->printToFile )
    {
        fout.open( this->logFileName.c_str(), ios::app | ios::out  );
        if( !fout.good() )
        {
            cout << "Error: Logger could not open file name: " << this->logFileName << endl;
            cout << "Error: (cont) message was: " << endl;
            if( doExitOnErrors )
            {
                cout << "Notify: Logger exiting on Error" << endl;
                exit( -1 );
            }
    
            //dump original message to standard out if file unavailable
            //assume original message follows prefix standards
            cout << message << endl;
            fout.close();
            return false;
        }
        else
        {
            fout << message << endl;
            if( message.find( "Error:" ) != string::npos && this->doExitOnErrors )
            {
                cout << "Notify: Logger exiting on Error - " << message << endl;
                fout << "Notify: Logger exiting on Error - " << message << endl;
                fout.close();
                exit( -1 );
            }

            if( message.find( "Warning:" ) != string::npos && this->doExitOnWarnings )
            {
                cout << "Notify: Logger exiting on Warning - " << message << endl;
                fout << "Notify: Logger exiting on Warning - " << message << endl;
                fout.close();
                exit( -1 );
            }
            fout.close();
            return true;
        }
    }

    cout << message << endl;    

    if( message.find( "Error:" ) != string::npos && this->doExitOnErrors )
    {
        cout << "Notify: Logger exiting on Error - " << message << endl;
        exit( -1 );
    }
    if( message.find( "Warning:" ) != string::npos && this->doExitOnWarnings )
    {
        cout << "Notify: Logger exiting on Warning - " << message << endl;
        exit( -1 );
    }

    return true;
}

bool Logger::setLogFileName( string logFileName, bool printDebug )
{
    if( this->doNotify && printDebug )
    {
        cout << "Notify: logger entered setFileName" << endl;
    }

    ofstream fout;
    //attempt open file
    fout.open( logFileName.c_str(), ios::app | ios::out ); 
    if( !fout.good() )
    {
        fout.close();
        cout << "Warning: gave logger a filename: " << logFileName << " that is currently not available " << endl;
        
        if( doExitOnWarnings )
        {
            cout << "Notify: Logger exiting on Warning" << endl;
            exit( -1 );
        }
        
        return false;
    }

    fout.close();
    this->logFileName = logFileName;
    this->printToFile = true;
    return true;
}

void Logger::setPrintToFile( bool printToFile, bool printDebug )
{
    if( this->doNotify && printDebug )
    {
        cout << "Notify: logger entered setPrintToFile" << endl;
    }

    this->printToFile = printToFile;
}

void Logger::setNotify( bool doNotify, bool printDebug )
{
    if( this->doNotify && printDebug )
    {
        cout << "Notify: logger entered setNotify" << endl;
    }

    this->doNotify = doNotify;
}

void Logger::setExitOnWarnings( bool doExit, bool printDebug )
{
    if( this->doNotify && printDebug )
    {
        cout << "Notify: logger entered setExitOnWarnings" << endl;
    }

    this->doExitOnWarnings = doExit;
}

void Logger::setExitOnErrors( bool doExit, bool printDebug )
{
    if( this->doNotify && printDebug )
    {
        cout << "Notify: logger entered setExitOnError" << endl;
    }

    this->doExitOnErrors = doExit;
}
#endif
