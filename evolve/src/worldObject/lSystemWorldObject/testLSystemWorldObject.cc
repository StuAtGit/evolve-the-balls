#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "LSystemWorldObject.h"
#include "LSystemGeneric.h"

using namespace std;

int main()
{
    map<string,LSystemWorldObject*> objs;
    objs["testNULL"] = new LSystemWorldObject( string("serpent"), NULL, NULL );
    LSystemGeneric* system = new LSystemGeneric();
    objs["testNULL"]->setSystem( system ); 
    objs["test"] = new LSystemWorldObject( string("serpent"), NULL, NULL );
    objs["test"]->setSystem( system );
    WorldObject* tmp = objs["test"]->breed( objs["testNULL"], 20 );
    if( tmp == NULL )
    {
        cout << "Warning: breed() failed" << endl;
    }

    cout << "testing interpreter with null obj: " << objs["testNULL"]->grow();
    cout << "testing: " << objs["test"]->grow() << endl;
}
