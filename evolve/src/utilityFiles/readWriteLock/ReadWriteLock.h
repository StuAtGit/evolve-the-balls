#ifndef Stu_ReadWriteLock_H
#define Stu_ReadWriteLock_H

#include <string>
#include <pthread.h>

#include <boost/lexical_cast.hpp>

#include "CommonAspect.h"
#define DO_DEBUG true

using namespace std;
using namespace boost;
using namespace evolve;

namespace evolve
{
/**TODO: insert class comments*/
class ReadWriteLock : public CommonAspect
{
public:
    ReadWriteLock();
    ~ReadWriteLock();
    /**
    *
    */
    bool readLock( bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool writeLock( bool printDebug = DO_DEBUG );
    /**
    *
    */
    bool unlock( bool printDebug = DO_DEBUG );
private:
    pthread_rwlock_t* lock;
    bool initialized;
};
};
#endif
