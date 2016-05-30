#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <boost/lexical_cast.hpp>

#include "Ogre.h"
#include "OgreException.h"

#include "PhysicsEngine.h"
#include "LSystem.h"
#include "InterpreterInterfaceHandlers.h"
#include "SystemInterfaceHandlers.h"
#include "LSystemGenericInterpreter.h"
#include "SocketMethods.h"
#include "OgreRenderer.h"
#include "WorldObject.h"
#include "StaticSphereObject.h"
#include "Utility.h"
#include "Timer.h"


//#define DO_PHYSICS true

using namespace std;
using namespace boost;
using namespace Ogre;
using namespace evolve;

OgreRenderer renderer;
bool print_debug = true;
bool do_notify = true;
bool show_points = true;
long totalPhysicsSeconds = 0;
unsigned long long totalPhysicsMicroseconds = 0;
long totalPhysicsIterations = 0;
long totalRenderingSeconds = 0;
unsigned int totalRenderingMicroseconds = 0;
long totalRenderingIterations = 0;
InterpreterInterfaceHandlers interpreterClient;
int interpreterSock = -1;
SystemInterfaceHandlers systemClient;
int systemSock = -1;
evolve::Timer timer;

map<string, WorldObject*> doPhysics( const map< string, map< int, vector< vector<double> > > >& lsystems, evolve::Timer& timer )
{
    SocketMethods socketMethods;
    vector< double > radii;
    vector<string> replies;
    map<string, WorldObject*> objectList;
    map<int,unsigned int> stopIndices;
 
    for( map< string, map< int, vector< vector<double> > > >::const_iterator iter = lsystems.begin(); iter != lsystems.end(); ++iter )
    {
        string id = (*iter).first;
        bool success = false;
        
        int level = 0;
        //don't really need this loop unless we turn off physics
#ifndef DO_PHYSICS
        if( print_debug && do_notify )
        {
            cout << "physics off, setting indices" << endl;
        }
        stopIndices.clear();
        for( map< int, vector< vector<double> > >::const_iterator i = (*iter).second.begin()
             ; i != (*iter).second.end(); ++i )
        {
            stopIndices[level] = (*i).second.size();
            ++level;
        }

#endif
        interpreterClient.handle_getAttribute( interpreterSock, id, "AtomicSize", replies, true );
        double radius = Utility::stringTo<double>( replies.back() , success, true );
        if( !success )
        {
            cout << "couldn't convert size: " + replies.back() << " to double " << endl;
            cout << "defaulting to 0.5" << endl;
            radius = 5;
        }
        cout << "radius: " << radius << endl;

#ifdef DO_PHYSICS
            timer.startTimer( 4 );
            stopIndices = physics.addSystem( (*iter).second, radius / 2.3 ); 
            timer.stopTimer( 4 );
            totalPhysicsSeconds += timer.getSeconds( 4 );
            totalPhysicsMicroseconds += timer.getMicroseconds( 4 );
            totalPhysicsIterations++;
#endif
        
        //cout << "system with id: " << (*iter).first << " has stop index of: " << stopIndex << endl;

        //for every branch, loop over every point
        timer.startTimer( 5 );
        level = 0;
        for( map< int, vector< vector<double> > >::const_iterator innerIter = (*iter).second.begin(); 
            innerIter != (*iter).second.end(); ++innerIter )
        {
            for( unsigned int i = 0; i < (*innerIter).second.size() && i < stopIndices[level]; ++i )
            {
                if( (*innerIter).second[i].size() < 3 )
                    continue;   

                string name = "system" + id + "_level" + Utility::toString( level ) + "_node" + Utility::toString( i );
                objectList[name] = new StaticSphereObject( name, renderer.getSceneManager(), renderer.getPhysicsEngine() ); 
                objectList[name]->create( "sphere.mesh", (*innerIter).second[i][0], (*innerIter).second[i][1], (*innerIter).second[i][2]  );
//                objectList[name]->setPosition( (*innerIter).second[i] );
                objectList[name]->noPhysics();
            }

            ++level;
        }
        timer.stopTimer( 5 );
        totalRenderingSeconds += timer.getSeconds( 5 );
        totalRenderingMicroseconds += timer.getMicroseconds( 5 );
        totalRenderingIterations++;

    }

    return objectList;
}

void displaySystems( map<string, map<int, vector< vector<double> > > > lsystems )
{
if( print_debug && do_notify && show_points )
    {
        for( map< string, map< int, vector< vector<double> > > >::iterator iter = lsystems.begin(); iter != lsystems.end(); ++iter )
        {
            cout << "system: " << (*iter).first << endl;
            for( map< int, vector< vector<double> > >::iterator innerIter = (*iter).second.begin(); 
                innerIter != (*iter).second.end(); ++innerIter )
            {
                for( unsigned int i = 0; i < (*innerIter).second.size(); ++i )
                {
                    if( (*innerIter).second[i].size() == 2 ) 
                    {
                        cout << "Found what looks like a pointer, at branch: " << (*innerIter).first << " of system " << (*iter).first
                             << " it's contents are: " << (*innerIter).second[i][0] << "," << (*innerIter).second[i][1] << endl;
                        continue;
                    }

                    if( (*innerIter).second[i].size() != 3 )
                    {
                        cout << "Warning: point: " << i << " of branch: " << (*innerIter).first << " of system: " << (*iter).first 
                             << " Has only " << (*innerIter).second[i].size() << " parameters " << endl;
                    }
                    cout << "(" << (*innerIter).second[i][0] << "," << (*innerIter).second[i][1] << "," << (*innerIter).second[i][2] << ")" << endl;
                }
            }

        }
    }
}

void displayPhysicsStats()
{
    if( print_debug && do_notify )
    {
        timer.printStats();
        cout << "total physics seconds: " << totalPhysicsSeconds << endl;
        cout << "total physics microseconds: " << totalPhysicsMicroseconds << endl;
        double physicsTemp = ((double)totalPhysicsMicroseconds) * 1.e-6;
        cout << "total physics ((double)micro)*1.e-6: " << physicsTemp << " seconds " << endl;
        cout << "total physics iterations: " << totalPhysicsIterations << endl;
        cout << "average physics iteration time: " << physicsTemp / totalPhysicsIterations << endl;

        cout << "total Rendering seconds: " << totalRenderingSeconds << endl;
        cout << "total Rendering microseconds: " << totalRenderingMicroseconds << endl;
        double renderingTemp = ((double)totalRenderingMicroseconds) * 1.e-6;
        cout << "total Rendering ((double)micro) * 1.e-6: " << renderingTemp << " second " << endl;
        cout << "total Rendering iterations: " << totalRenderingIterations << endl;

        cout << "other than physics, the program took: " << timer.getMicroseconds(3) - totalPhysicsMicroseconds 
             << " microseconds to run " << endl;
        cout << "thats: " << (((double)timer.getMicroseconds(3)) - ((double)totalPhysicsMicroseconds) ) * 1.e-6 << " seconds " << endl;
        cout << "stop indices: " << endl;
    }

}

void breed( const string& id1, const string& id2, const string& newId1, const string& newId2, string startPoint )
{
    cout << "Notify: testPhysics entered breed()" << endl;

    vector<string> replies;

    bool success = systemClient.handle_makeCopy( systemSock, id1, newId1, replies, true ); 
    success = success && interpreterClient.handle_makeCopy( interpreterSock, id1, newId1, replies, true );
    if( !success )
    {
        cout << "testPhysics: get copy 1 failed!" << endl;
        //exit(1);
    }
    cout << "copy said: " << replies.back() << endl;
    success = systemClient.handle_makeCopy( systemSock, id2, newId2, replies, true );
    if( !success )
    {
        cout << "testPhysics: get copy 2 failed!" << endl;
        //exit(1);
    }
    cout << "copy said: " << replies.back() << endl;
    map<string,string> systemCrossMap;
    map<string,string> optionsCrossMap;
    map<string,string> mutationProbabilitiesCrossMap;
    map<string,string> growthRulesCrossMap;

    systemCrossMap["1:1"] = "3";
    systemCrossMap["2:3"] = "2";
    systemCrossMap["5:random"] = "3";
    systemCrossMap["random:6"] = "2";
    for( int i = 0; i < 1000; ++i )
    {
       ostringstream ostream;
       ostream << i;
       systemCrossMap["random:" + (string)"random"] = "2";
    }
    optionsCrossMap["0:0"] = "3";
    optionsCrossMap["1:2"] = "1";
    optionsCrossMap["random:2"] = "2";
    mutationProbabilitiesCrossMap["1:1"] = "3";
    mutationProbabilitiesCrossMap["3:random"] = "2";
    mutationProbabilitiesCrossMap["2:2"] = "1";

/*
    lSystemComposite.getLSystem( copy1, id1 );   
    lSystemComposite.getLSystem( copy2, id2 );
*/
   cout << "entered cross" << endl;
    systemClient.handle_cross( systemSock, newId1, newId2,  mutationProbabilitiesCrossMap, 
                               growthRulesCrossMap, systemCrossMap, optionsCrossMap, replies, true);
    cout << "exit cross" << endl;
    interpreterClient.handle_setAttribute( interpreterSock, newId1, "StartPoint", startPoint, replies, true );
}

int main()
{
    cout << "start!" << endl;
    timer.startTimer( 0 );
    timer.startTimer( 1 );
    //InterpreterInterfaceHandlers interpreterClient;
    //SystemInterfaceHandlers systemClient;
    SocketMethods socketMethods;
    bool interpreterConnect = socketMethods.getClientSocket( interpreterSock, "127.0.0.1", 5013, true );
    bool lSystemConnect = socketMethods.getClientSocket( systemSock, "127.0.0.1", 5012, true ); 

    if( !lSystemConnect )
    {
        if( print_debug )
        {
            cout << "failed to connect to lsystemServer" << endl;
        }
    }
    if( !interpreterConnect )
    {
        if( print_debug )
        {
            cout << "failed to connect to interpreter" << endl;
        }
    }
    timer.stopTimer( 1 );

    if( print_debug && do_notify )
    {
        cout << "connected" << endl;
    }

    vector<string> ids;
    vector<string> replies;
    bool success = false;
    for( int i = 0; i < 8; ++i )
    {
        ids.push_back( Utility::toString( i, success, true ) );
    }
    map< string, map< int, vector< vector<double> > > > lsystems;
    int startx = 0;
    int starty = 0;
    timer.startTimer( 2 );

    if( print_debug && do_notify )
    {
        cout << "evolving" << endl;
    }

    for( unsigned int i = 0; i < ids.size(); ++i )
    {
        systemClient.handle_create( systemSock, ids[i], replies, true );
        systemClient.handle_load( systemSock, ids[i], replies, true );
        interpreterClient.handle_create( interpreterSock, ids[i], replies, true );
        interpreterClient.handle_load( interpreterSock, ids[i], replies, true );
    }

    unsigned int generationCount = 10;
    for( unsigned int i = 0; i < ids.size() && i < generationCount; ++i )
    {
        string query = "";
        unsigned int num_iterations = 10;
        systemClient.handle_run( systemSock, ids[i], "grow", num_iterations, replies, true );
        ostringstream ostream;
          int pickCoordinate = Utility::getRandomInteger( 1, 2, true );
          int pickHop = Utility::getRandomInteger( 100, 2000, true ); 
          unsigned int tmpx = 0;
          unsigned int tmpy = 0;
          switch( pickCoordinate )
          {
            case 1:
                tmpx = startx + pickHop;
            case 2: 
                tmpy = starty + pickHop;
          }
         
            string startPoint;
            ostream.str("");
            ostream << tmpx;
            ostream << ",";
            ostream << "10,";
            ostream << tmpy;
            cout << "crossing: " << ids[i] << endl;
            string newId = ids[i] + "_kid" + Utility::toString( Utility::getRandomInteger( 0, 10000000, true ), success, true );
            string tmpId = newId + "tmp";
            //void breed( const string& id1, const string& id2, const string& newId1, const string& newId2, string startPoint )
            //TODO (old): check why sending i instead of ids[i] makes seg fault & freeze?
            //(at least it only does in on client... server stays up .. good!
            //BUT! we should at least have a warning of some sort ... no?

            breed( ids[i], ids[i], newId, tmpId, ostream.str() );
            ids.push_back( newId );
//            cout << "done crossing.." << ids[i] << endl;
        interpreterClient.handle_setAttribute( interpreterSock, ids[i], "StartPoint", ostream.str(), replies, true );
        //cout << "getting system: " << ids[i] << endl;
        //cout << "getting interpreted system... " << endl;
//        interpreter.setNotify( true );

        systemClient.handle_getSystem( systemSock, ids[i], replies, true );
        string system = replies.back();
        interpreterClient.handle_getSystem( interpreterSock, ids[i], system, replies, true );
        string interpretedSystem = replies.back();
        LSystemGenericInterpreter interpreter( "blah" );
        lsystems[ids[i]] = interpreter.getPoints( interpretedSystem, true );
       // cout << "done getting interpreted system... " << replies.back() << endl;
    }

    displaySystems( lsystems );    
    
    if( print_debug && do_notify )
    {
        cout << "done evolving" << endl;
    }

    timer.stopTimer( 2 );
   
    if( print_debug && do_notify )
    {
        cout << "starting rendering" << endl;
    }

    timer.startTimer( 3 );
    map< string, WorldObject* > objectList = doPhysics( lsystems, timer );
    timer.stopTimer(3);
    timer.stopTimer(0);
    
    displayPhysicsStats();
    cout << "Rendering..." << endl;
    renderer.setNotify( true );
    renderer.createScene( objectList );
    timer.startTimer( 52 );
    renderer.run();
    timer.stopTimer( 52 );
    cout << "Running physics time: " << renderer.getPhysicsTime() << endl;
    cout << "Running ogre time: " << renderer.getFrame()->getOgreTime() << endl;
    cout << "Running frame time: " << renderer.getFrame()->getFrameTime() << endl;
    cout << "Running time: " << timer.getMicroseconds( 52 ) * 1.e-6 << endl;
}
