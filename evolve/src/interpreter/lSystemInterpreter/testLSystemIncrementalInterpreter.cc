#include "LSystemIncrementalInterpreter.h"
#include <map>
#include <iostream>
#include <string>
#include "Utility.h"
#include "LSystem.h"
#include "LSystemGeneric.h"
#include "LSystemIncrementalInterpreter.h"

using namespace std;

int main()
{
    Logger log;
    Interpreter* interpreter = NULL;
    //
    //interpreter->setNotify( true ); this actually works.. weird.
    //(if you do a check for null in the common aspect object, you can
    //even catch the error...)
    Interpreter* interpreterCopy = NULL;
    LSystem* lsystem;
    lsystem = new LSystemGeneric();
    interpreter = new LSystemIncrementalInterpreter( lsystem ); 
    interpreter->setNotify( true );
    bool success = lsystem->initialize( "0.sys" );
    log.print( "Warning: in interpreterIncremental unit test: failed to load L-System", !success );
    for( int i = 0; i < 10; ++i )
    {
        string query;
        lsystem->run( "grow", query );
    }
    //TODO: use lsystem with actual stuff in it (get .sys file & grow it a little)
//	Utility utility;
    map<string,string> attributes;
    attributes["RightAngle"] = "30";
    attributes["LeftAngle"] = "-40";
    attributes["AtomicSize"] = "1";
    attributes["StartPoint"] = "0,1,0";
    attributes["UpAngle"] = "20";
    attributes["DownAngle"] = "-20";

    interpreter->setNotify( true );    
    interpreter->setAttributes( attributes );
    vector<double> newPoint;
    unsigned int index = 0;
    vector<unsigned int> indicies;
    LSystemIncrementalInterpreter* tmp = dynamic_cast<LSystemIncrementalInterpreter*>(interpreter);
    if( tmp == NULL )
    {
        cout << "FAILED CAST!!" << endl;
        delete interpreter;
        return -1;
    }

    for( int i = 0; i < 100; ++i )
    {
        tmp->grow( newPoint, index ); 
        indicies.push_back( index );
    }
    success = interpreter->getCopy( interpreterCopy );
    log.print( "Warning: in interpreterIncremental unit test: failed to get copy", !success ); 
    string interpretedSystemCopy;
    interpreterCopy->getSystem( interpretedSystemCopy, *lsystem );
    cout << "System: " << endl;
    cout << lsystem->getSystem() << endl; 
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
    tmp->getPoints();
    tmp->printSystem();
    cout << "checking getElement (verified against system below):" << endl;
    string tmpElement = "";
    tmp->setNotify( false );
    string checkElements = "";
    cout << "(";
    for( vector<unsigned int>::const_iterator i = indicies.begin(); i != indicies.end(); ++i )
    {
        tmp->getElement( tmpElement, *i );
        cout << tmpElement << ","; 
        checkElements += tmpElement;
    }
    cout << ")" << endl;
    cout << "system: " << endl;
    cout << "(" << lsystem->getSystem() << ")" << endl;
    if( checkElements != lsystem->getSystem() )
    {
        cout << "ERROR? lsystem != to list of all elements generating interpreted string" << endl;
        cout << "look for B's, these can be ignored" << endl;
    }
    vector<double> currentDirection;
    if( tmp->getCurrentDirection( currentDirection ) )
    {
        cout << "Current direction of interpreted system: " <<  Utility::vectorToString( currentDirection ) << endl;
    }
    else
    {
        cout << "Error! failed to get current direction?" << endl;
    }
	delete interpreter;
    delete interpreterCopy;
}

