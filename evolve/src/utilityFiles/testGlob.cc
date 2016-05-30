#include "Utility.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
	vector<string> files = Utility::globFiles( "*.c*", true );
	for( unsigned int i = 0; i < files.size(); ++i )
	{
		cout << "file: " << i << " " << files[i];
	}
	cout << endl;
    string blank = "";
    string space = "     ";
    string mixed = "   \t \t \t \n  \t \t";
    string back = "back ";
    string front = " front";
    string stuff = "  adsf\t \n asdffsd stuff\t \n\n   \t ";
    string morestuff = "morestuff";

    cout << "blank:" << Utility::trim( blank ) << ":" << endl;
    cout << "space:" << Utility::trim( space ) << ":" << endl;
    cout << "back:" << Utility::trim( back ) << ":" << endl;
    cout << "front:" << Utility::trim( front ) << ":" << endl;
    cout << "stuff:" << Utility::trim( stuff ) << ":" << endl;
    cout << "morestuff:" << Utility::trim( morestuff ) << ":" << endl;

}

