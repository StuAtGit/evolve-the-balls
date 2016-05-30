#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <boost/lexical_cast.hpp>
#include "ReadWriteLock.h"

using namespace std;
using namespace boost;

static vector<int> testList;
static ReadWriteLock testLock;

struct test_thread_args
{
    bool doLock;
};

void initList()
{
    for( int i = 0; i < 50; ++i )
    {
        testList.push_back( 2 );
    }
}

void* reader( void* args )
{
    test_thread_args* arg = (test_thread_args*)(args);
    if( arg->doLock == true )
    {
        testLock.readLock();
    }
    string list = "";
    unsigned int size = testList.size();
    for( unsigned int i = 0; i < size; ++i )
    {
        list += lexical_cast<string>( testList[i] ) + ",";
        usleep(0);
    }

    cout << "values in list: " << list << endl;
    if( arg->doLock == true )
    {
        testLock.unlock();
    }
    return NULL;
}

void* writer( void* args )
{
    test_thread_args* arg = (test_thread_args*)(args);
    if( arg->doLock == true )
    {
        testLock.writeLock();
    }
    unsigned int size = testList.size();
    for( unsigned int i = 0; i < size; ++i )
    {
        testList[i]++;
    }
    if( arg->doLock == true )
    {
        testLock.unlock();
    }
    return NULL;
}

int main()
{
    initList();
    test_thread_args* arg = new test_thread_args();
    cout << "testing without locking (values should change in the middle of listing): " << endl;
    arg->doLock = false;
    pthread_t thr;
    pthread_create( &thr, NULL, reader, arg  );
    pthread_t thr2;
    pthread_create( &thr2, NULL, writer, arg  );
    pthread_t thr3;
    pthread_create( &thr3, NULL, reader, arg  );
    cout << "waiting for bad threads to finish" << endl;
    pthread_join( thr, NULL );
    pthread_join( thr2, NULL );
    pthread_join( thr3, NULL );

    test_thread_args* arg2 = new test_thread_args();
    cout << "testing with locking (values should not change in the middle of listing): " << endl;
    arg2->doLock = true;
    pthread_t thr4;
    pthread_create( &thr4, NULL, reader, arg2  );
    pthread_t thr5;
    pthread_create( &thr5, NULL, writer, arg2  );
    pthread_t thr6;
    pthread_create( &thr6, NULL, reader, arg2  );
    cout << "waiting for good threads to finish" << endl;
    pthread_join( thr4, NULL );
    pthread_join( thr5, NULL );
    pthread_join( thr6, NULL );
}





