#include "Logger.h"
#include <iostream>

using namespace std;
using namespace evolve;

int main()
{
    Logger logger;
    Logger loggerFile( "loggerTestLogFile" );
    Logger logger3;

    logger.setNotify( true );
    loggerFile.setNotify( true );
    logger3.setNotify( true );
    logger.print( "Logger test" );
    loggerFile.print( "Logger test");
    logger3.print( "Logger test 2" );
    logger3.setPrintToFile( true );
    logger3.setLogFileName( "loggerTestFile2" );
    logger3.print( "Logger test2" );
    logger.setExitOnErrors( true );
    logger.print( "Warning:" );
    logger.print( "Error: testing exit on error " );
    cout << "you shouldn't see this message.." << endl;
   
}
