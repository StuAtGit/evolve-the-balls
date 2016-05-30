#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "PhysicsEngine.h"
#include "Utility.h"
#include "Matrix.h"

using namespace std;

int main()
{
    PhysicsEngine pe;
    vector<double> grav = Utility::makeVector( 0.0, 0.0, 0.0 );
    pe.setGravity( grav[0], grav[1], grav[2] );
    grav.clear();
    pe.getGravity( grav );
    cout << "gravity: " << Utility::vectorToString( grav ) << endl;
    vector<double> pos = Utility::makeVector( 0.0, 10.0, 0.0 );
    vector<double> pos2 = Utility::makeVector( 0.0, 50.0, 0.0 );
    vector<double> newpos1;
    vector<double> newpos2;
    pe.addItem( "test1", pos, 5 );
    pe.addItem( "test2", pos2, 2 );
    pe.setMass( "test1", 50 );
    pe.setMass( "test2", 10 );


    cout << "before: " << endl;
    pe.getPosition( "test1", newpos1 );
    pe.getPosition( "test2", newpos2 );
    cout << "pos 1: " << Utility::vectorToString( newpos1 ) << endl;
    cout << "pos 2: " << Utility::vectorToString( newpos2 ) << endl;
    vector<double> vel = Utility::makeVector( 0.0, -3.0, 0.0 );
    pe.setLinearVelocity( "test2", vel ); 
    /*
    for( int i = 0; i < 100; ++i )
        pe.doPhysics();
    */
    pe.startPhysics();
    newpos1.clear();
    newpos2.clear();
    cout << "set vel on 2 (down): " << Utility::vectorToString( vel ) << endl;
    pe.getPosition( "test1", newpos1 );
    pe.getPosition( "test2", newpos2 );
    cout << "pos 1 (should be negative): " << Utility::vectorToString( newpos1 ) << endl;
    cout << "pos 2 (should be positive): " << Utility::vectorToString( newpos2 ) << endl;
    bool success = true;
    JointProperties jointProperties;
    vector<double> midpoint;
    Matrix::midpoint( midpoint, newpos1, newpos2 );
    jointProperties.anchor = midpoint;
    success = pe.connectItems( "fail1", "fail2", "ball", jointProperties );
    if( success )
        cout << "bad return valued for failed function call (1), should be false" << endl;
    success = pe.connectItems( "test1", "fail2", "ball", jointProperties );
    if( success )
        cout << "bad return valued for failed function call (2), should be false" << endl;
    success = pe.connectItems( "test1", "test2", "fail", jointProperties );
    if( success )
        cout << "bad return valued for failed function call (3), should be false" << endl;
    newpos2.clear();

    success = pe.connectItems( "test1", "test2", "ball", jointProperties );
    if( !success )
    {
        cout << "ERROR: failed test, connect should have worked" << endl;
    }
    /*
    for( int i = 0; i < 100; ++i )
        pe.doPhysics();
    */
    sleep(2);
    newpos1.clear();
    newpos2.clear();
    pe.getPosition( "test1", newpos1 );
    pe.getPosition( "test2", newpos2 );
    cout << "pos 1: " << Utility::vectorToString( newpos1 ) << endl;
    cout << "pos 2: " << Utility::vectorToString( newpos2 ) << endl;
    cout << "now positions should be close together" << endl;

}
