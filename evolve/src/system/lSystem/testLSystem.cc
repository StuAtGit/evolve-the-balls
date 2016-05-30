#include "LSystem.h"
#include "Utility.h"
#include <assert.h>
#include "LSystemGeneric.h"

string runLSystems()
{
    LSystem* lsystem = new  LSystemGeneric();
    LSystem* lsystem2 = new LSystemGeneric();
 //  lsystem->initialize( "inputFile" );    
    cout << "loading from saveSystem file" << endl;
   if( !lsystem->initialize( "saveSystem" ) )
    {
    	cout << "warning, couldn't open saved  system" << endl;
    	if( !lsystem->initialize( "inputFile" ) )
	    {
	        cout << "error, found no system files to open quitting" << endl;
			delete lsystem;
			delete lsystem2;
   		    return "";
   	    }
    }
    //adding option "STU" to lsystem 1
    cout << "adding option stu to lsytem 1" << endl;
    lsystem->addOption( "STU" );
    cout << "changing mutation probabilites to 1000, 80, 5, 5" << endl;
    lsystem->setMutationProbability( "1", 1000 );
    lsystem->setMutationProbability( "2", 80 );
    lsystem->setMutationProbability( "3", 5 );
    lsystem->setMutationProbability( "4", 5 );
    cout << "adding growth rule STU->STU's GREAT! to lsystem 1" << endl;
    lsystem->setGrowthRule( "STU", "STU's GREAT!" );
   // lsystem->loadSystem( "saveSystem" );

   //*** test with invalid file***/
   cout << "testing system 2 with a non-existent file" << endl;
    lsystem2->initialize( "inputFile" );
    cout << "******************system 1*********************" << endl;
   ((LSystemGeneric*)lsystem)->printStats();
    string query = "";
    for( int i = 0; i < 1000; ++i )
    {
        lsystem->run( "mutate", query );
    }
    ((LSystemGeneric*)lsystem)->printStats();

    cout << "******************system 2*********************" << endl;

    cout << "growing system1" << endl;
    for( int i = 0; i < 5; ++i )
    {
        lsystem->run( "grow", query ); 
        lsystem2->run( "grow", query );
    }
   
    cout << "*************** system 1 now ***************" << endl;
    ((LSystemGeneric*)lsystem)->printStats(  );
    cout << "*************** system 2 now ***************" << endl;
    ((LSystemGeneric*)lsystem2)->printStats(  );
    lsystem->save( "saveSystem" );
	delete lsystem;
	string retu = lsystem2->getSystem();
	delete lsystem2;
    return retu;
}

int main( int argc, char** argv )
{
    runLSystems();
    LSystem* lsystem = new LSystemGeneric();
    cout << "initializing lsystem" << endl;
    lsystem->initialize( "inputFile" );
    lsystem->addOption( "Q" );
    lsystem->addOption( "P" );
    lsystem->addOption( "t" ); 
    cout << "initialized" << endl;
    cout << "system: " << lsystem->getSystem() << endl;
    LSystem* lsystem2 = new LSystemGeneric();
    lsystem2->initialize( "1.sys" ); 

    cout << "testing cross: " << endl; 
    string tmp = "";
    for( int i = 0; i < 100; ++i )
    {
        lsystem2->run( "grow", tmp );
        lsystem->run( "grow", tmp );
    }
    cout << "system1: " << lsystem->getSystem() << endl;
    cout << "system2: " << lsystem2->getSystem() << endl;
    cout << endl;
    bool success = false;
    cout << "options1: " << Utility::vectorToString( lsystem->getOptions(), success ) << endl;
    cout << "options2: " << Utility::vectorToString( lsystem2->getOptions(), success ) << endl;
    cout << "mutation prob 1: " << Utility::mapToString( lsystem->getMutationProbabilities(), success ) << endl;
    cout << "mutation prob 2: " << Utility::mapToString( lsystem2->getMutationProbabilities(), success ) << endl;
    cout << "growth rules 1: " << Utility::mapToString( lsystem->getGrowthRuleMap(), success ) << endl;
    cout << "growth rules 2: " << Utility::mapToString( lsystem2->getGrowthRuleMap(), success ) << endl;
    
    map<string,string> systemCrossMap;
    map<string,string> optionsCrossMap;
    map<string,string> mutationProbabilitiesCrossMap;
    map<string,string> growthRulesCrossMap;
    systemCrossMap["1:1"] = "3";
    systemCrossMap["2:3"] = "2";
    systemCrossMap["5:random"] = "3";
    systemCrossMap["random:6"] = "2";
    optionsCrossMap["0:0"] = "3";
    optionsCrossMap["1:2"] = "1";
    optionsCrossMap["random:4"] = "2";
    mutationProbabilitiesCrossMap["1:1"] = "3";
    mutationProbabilitiesCrossMap["3:random"] = "2";
    mutationProbabilitiesCrossMap["2:4"] = "1";
    cout << "system cross map is: " << Utility::mapToString( systemCrossMap, success ); 
    lsystem->cross( lsystem2, systemCrossMap, optionsCrossMap, 
                    mutationProbabilitiesCrossMap, growthRulesCrossMap );

    cout << "after cross: " << endl;
    cout << "system1: " << lsystem->getSystem() << endl;    
    cout << "system2: " << lsystem2->getSystem() << endl;
    cout << "options1: " << Utility::vectorToString( lsystem->getOptions(), success ) << endl;
    cout << "options2: " << Utility::vectorToString( lsystem2->getOptions(), success ) << endl;
    cout << "mutation prob 1: " << Utility::mapToString( lsystem->getMutationProbabilities(), success ) << endl;
    cout << "mutation prob 2: " << Utility::mapToString( lsystem2->getMutationProbabilities(), success ) << endl;
    cout << "growth rules 1: " << Utility::mapToString( lsystem->getGrowthRuleMap(), success ) << endl;
    cout << "growth rules 2: " << Utility::mapToString( lsystem2->getGrowthRuleMap(), success ) << endl;

    //test getCopy(), test crossMap["random:random"] ... 
    cout << "**************** testing getCopy() ***************" << endl << endl;
    cout << "**************** system 1************************" << endl;
    ((LSystemGeneric*)lsystem)->printStats();
    System* lsystemCopy = new LSystemGeneric();
    lsystem->getCopy( lsystemCopy );
    cout << "***************** copy **************************" << endl;
    ((LSystemGeneric*)lsystemCopy)->printStats();
    string query = "";
    cout << "Growing copy: " << endl;
    ((LSystemGeneric*)lsystemCopy)->setGrowthRule( "S", "B1" );
    ((LSystemGeneric*)lsystemCopy)->setMutationProbability( "2", 10 );
    for( int i = 0; i < 5; ++i )
    {
        lsystemCopy->run( "grow", query );
    }
    for( int i = 0; i < 1000; ++i )
    {
        lsystemCopy->run( "mutate", query );
    }
    cout << "*******original system: " << endl;
    ((LSystemGeneric*)lsystem)->printStats();
    cout << "*******Copy now: " << endl;
    ((LSystemGeneric*)lsystemCopy)->printStats();
   // cout << "mutation cross map: " << assembleMutations( mutationProbabilitiesCrossMap ) << endl;
    delete lsystem;
    delete lsystem2;
    return 0;
}
