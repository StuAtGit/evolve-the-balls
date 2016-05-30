#include "Utility.h"
#include <iostream>
#include <sstream>

using namespace std;

int main()
{
    Utility utility;
    for( int i = 0; i < 30; ++i )
    {
        cout << utility.getRandomInteger( 0, 10 ) << endl;
    }
    string test = "testing input string on \n ostringstream";
    ostringstream ostream;
    ostream << test;
    cout << "output: " << ostream.str() << endl;
}
