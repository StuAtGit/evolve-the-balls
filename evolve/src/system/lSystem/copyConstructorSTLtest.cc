#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

string getTest( vector< map< int, string> > test )
{
    string ret;
    for( unsigned int i = 0; i < test.size(); ++i )
    {
        for( map<int,string>::iterator j = (test[i]).begin(); j != (test[i]).end(); ++j )
        {
           ostringstream ostream; 
           ostream << "<";
           ostream << i;
           ostream << " (" << (*j).first << "," << (*j).second << ")>";
           ret += ostream.str();
        }
    }

    return ret;
}

int main()
{
    vector< map< int, string> > test;
    vector< map< int, string> > test2;
   
    map<int,string> element1;
    element1[0] = "element 1 index 0";
    test.push_back( element1 );
    test2 = test;
    cout << " test 1: " << getTest(test) << endl;
    cout << " test 2: " << getTest(test2) << endl;
    map<int,string> element2;
    element2[0] = "element 2 index 0";
    test2.push_back( element2 );
    cout << " test 1: " << getTest(test) << endl;
    cout << " test 2: " << getTest(test2) << endl;
    element2[1] = "element 1 index 1";
    test.push_back( element2 );
    cout << " test 1: " << getTest(test) << endl;
    cout << " test 2: " << getTest(test2) << endl;
}
