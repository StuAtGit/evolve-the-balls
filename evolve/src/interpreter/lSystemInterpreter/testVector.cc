#include <vector>
#include <string>
#include <iostream>

using namespace std;

int main()
{
    vector<double> point;
    point.push_back( 0 );
    point.push_back( 0 );

    vector<double> point2;
    point.push_back(0);
    point.push_back(0);

    vector< vector<double>* > points;
    points.push_back( &point );
    points.push_back( &point2 );

    cout << "point 1 size: " << (*points[0]).size() << endl;
    cout << "size: " << points.size() << endl;
    points.pop_back();
    cout << "point 1 size: " << (*points[0]).size() << endl;
    cout << "size: " << points.size() << endl;
}
