#include <iostream>
#include <string>
#include <map>
#include <pthread.h>
#include <boost/lexical_cast.hpp>

#include "Mutex.h"
#include "Logger.h"

using namespace std;
using namespace boost;

#define NUM_THREADS 5

Mutex mutex;
map<string,Mutex*> testMap;
map<string,Mutex> testMap2;
static Logger logger;
static int goodShared = 0;
static int badShared = 0;

class MutexInClass
{
public:
    MutexInClass()
    {
    }

    void setValue( const string& name, int value )
    {
        if( this->mutexes.find(name) == this->mutexes.end() )
        {
            //this->mutexes[name] = new Mutex();
        }
        this->mutexes[name].lock();
        this->obj[name] = value; 
        this->mutexes[name].unlock();
    }

private:
    map<string,Mutex> mutexes;
    map<string,int> obj;
};

MutexInClass testMutexInClass;

//tests for a bug where the destructor causes a double free
void initialize_map( string id )
{
    Mutex mutex;
    mutex.lock();
    mutex.unlock();

    //new test -> old one generated "invalid argument"
    //Don't mix stack + mutexes I guess..
    testMap[id] = new Mutex();
    testMap[id]->lock();
    testMap[id]->unlock();

    /*
     * Save this to remind me of "invalid argument"
     */ 
      cout << "normal map test..." << endl; 
      testMap2[id]; 
      testMap2[id].lock();
      testMap2[id].unlock();
   /**/
}

void* badThread( void* args )
{
    int id = (*((int*)args));
    cout << "In bad thread(" << id << ") shared before: " << badShared << endl;
    badShared++;
    usleep(900);
    cout << "In bad thread(" << id << ") shared after: " << badShared << endl;

    return NULL;
}

void* goodThread( void* args )
{
    int id = (int)(*((int*)args));
    mutex.lock();
    cout << "In good thread(" << id << ") shared before: " << goodShared << endl;
    goodShared++;
    usleep(900);
    cout << "In good thread(" << id << ") shared after: " << goodShared << endl;
    cout << "testing class.." << endl;
    mutex.unlock();
    string fred = "fred";
    string fred2 = "fred2";
    testMutexInClass.setValue( fred, id ); 
    testMutexInClass.setValue( fred2, id );

    return NULL;
}

int main()
{
    cout << "Testing threads, each thread enters, prints out a counter shared with it's fellow threads" << endl;
    cout << ", sleeps, increments it by one, prints it again, then quits" << endl;
    pthread_t goodThreads[NUM_THREADS];
    cout << "Running threads with mutexes: " << endl;
    for( int i = 0; i < NUM_THREADS; ++i )
    {
        pthread_attr_t attr;
        pthread_attr_init( &attr );
        //pthread_attr_setschedpolicy( &attr, SCHED_RR );
        int* id = new int;
        *id = i;
        //usleep( 1 );
        //int id = i;
        int status = pthread_create( &(goodThreads[i]), &attr, goodThread, id );
        logger.print( "Error: creating thread: " + lexical_cast<string>(strerror(status)), status < 0 );
    }
    sleep(2);
    pthread_t badThreads[NUM_THREADS];
    cout << "Running threads without mutexes: " << endl;
    for( int i = 0; i < NUM_THREADS; ++i )
    {
        pthread_attr_t attr;
        pthread_attr_init( &attr );
        //pthread_attr_setschedpolicy( &attr, SCHED_RR );
        int* id = new int;
        *id = i;
        int status = pthread_create( &badThreads[i], &attr, badThread, id );
        logger.print( "Error: creating thread: " + lexical_cast<string>(strerror(status)), status < 0 );
    }

    for( int i = 0; i < NUM_THREADS; ++i )
    {
        pthread_join( goodThreads[i], NULL );
        pthread_join( badThreads[i], NULL );
    }
    
    cout << "testing allocation in maps" << endl;
    for( int i = 0; i < 10; ++i )
    {
        initialize_map( lexical_cast<string>(i) + "asdf" );
    }
}
