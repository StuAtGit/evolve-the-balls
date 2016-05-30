#ifndef STU_Mutex_H
#define STU_Mutex_H

#include <pthread.h>
#include <boost/lexical_cast.hpp>
#include "CommonAspect.h"
#include "Utility.h"

#define DO_DEBUG true

using namespace std;
using namespace boost;
using namespace evolve;

namespace evolve
{
/**
 * A class to wrap a simple mutex object
 * This class exists mainly to ease portability issues in the
 * future
 * IMPORTANT: this class is to be maintained in lockstep with whatever
 * threading library we happend to use. Since, right now we have no thread
 * wrapper class, because an acceptable interface has not been decided on.
 * This means we'll have to main the direct pthread_create, init, references
 * in the code where they're used.
 * Eventually, we want to transition to Boost threads. (I think? APR threads?)
 * */
class Mutex : public CommonAspect
{
public:
    Mutex();
    ~Mutex();
    /**
    *Locks this mutex
    */
    bool lock( bool printDebug = DO_DEBUG );
    /**
    *Unlocks this mutex
    */
    bool unlock( bool printDebug = DO_DEBUG );
private:
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;
};
};
#endif
