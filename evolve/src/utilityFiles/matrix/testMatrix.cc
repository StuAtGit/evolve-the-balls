#include <iostream>
#include <string>

#include "Matrix.h"

using namespace std;
using namespace Matrix;

int main()
{
    vector<int> vec;
    vector<double> vec1, vec2, vec3;
    vector<int> veci1, veci2, veci3;
    vec.resize(9); 

    cout << "test matrix to string: " << endl;
    cout << Matrix::matrix9ToString( vec );

    Matrix::initializePoint( vec1, 0.0, 0.0, 0.0 );
    Matrix::initializePoint( vec2, 0.0, 0.0, 1.0 );
    Matrix::initializePoint( vec3, 1.0, 0.0, 0.0 );
    bool success = false;
    cout << "test get normal of: (" << Utility::vectorToString( vec1, success )
         << "),(" << Utility::vectorToString( vec2, success ) << "),("
         << Utility::vectorToString( vec3, success ) << ")" << endl;
    vec1 = getNormal( vec1, vec2, vec3 );
    cout << Utility::vectorToString( vec1, success ) << endl;

    cout << "Now with ints: " << endl;
    Matrix::initializePoint( veci1, 0, 0, 0 );
    Matrix::initializePoint( veci2, 0, 0, 1 );
    Matrix::initializePoint( veci3, 1, 0, 0 );
    cout << "test get normal of: (" << Utility::vectorToString( veci1, success )
         << "),(" << Utility::vectorToString( veci2, success ) << "),("
         << Utility::vectorToString( veci3, success ) << ")" << endl;
    veci1 = getNormal( veci1, veci2, veci3 );
    cout << Utility::vectorToString( veci1, success ) << endl;


}
