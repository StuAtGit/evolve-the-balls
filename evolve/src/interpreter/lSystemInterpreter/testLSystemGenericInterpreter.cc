#include "LSystemGenericInterpreter.h"
#include <map>
#include <iostream>
#include <string>
#include "Utility.h"
#include "LSystem.h"
#include "LSystemGeneric.h"

using namespace std;

int main()
{
    Logger log;
    Interpreter* interpreter = NULL;
    Interpreter* interpreterCopy = NULL;
    LSystem* lsystem;
    interpreter = new LSystemGenericInterpreter( "1" ); 
    lsystem = new LSystemGeneric();
    bool success = lsystem->initialize( "0.sys" );
    log.print( "Warning: in interpreterGeneric unit test: failed to load L-System", !success );
    for( int i = 0; i < 10; ++i )
    {
        string query;
        lsystem->run( "grow", query );
    }
    //TODO: use lsystem with actual stuff in it (get .sys file & grow it a little)
//	Utility utility;
    map<string,string> attributes;
    attributes["RightAngle"] = "30";
    attributes["LeftAngle"] = "40";
    attributes["AtomicSize"] = "1";
    attributes["StartPoint"] = "0,1,0";
    attributes["UpAngle"] = "20";
    interpreter->setNotify( true );    
    interpreter->setAttributes( attributes );
    
    success = interpreter->getCopy( interpreterCopy );

    log.print( "Warning: in interpreterGeneric unit test: failed to get copy", !success ); 
    string interpretedSystemCopy;
    interpreterCopy->getSystem( interpretedSystemCopy, *lsystem );
    cout << "system (copy): " << interpretedSystemCopy << endl;
    string interpretedSystem;
    interpreter->getSystem( interpretedSystem, *lsystem );
    cout << "system: " << interpretedSystem << endl;
    interpreter->saveAttributes( "0.attr", true );
    interpreter->loadAttributes( "0.attr" );
    interpreter->getAttributes( true );
	string supported = Utility::vectorToString( interpreter->getSupportedAttributes(), success );
	cout << "supported: " << supported << endl;
    Logger* logger = NULL;
    interpreter->getLogger( logger );
    LSystemGenericInterpreter::getPoints( interpretedSystem, *logger, interpreter->getNotify() );
    LSystemGenericInterpreter::printSystem( interpretedSystem );
	delete interpreter;
}

