#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

/*
template<class T>
bool serialize( T data, char*& state, unsigned int& size )
{
    if( sizeof(T) > INT_MAX )
    {
        cout << "Data type way too big" << endl;
        return false;
    }
    state = new char[sizeof(T)];
    size = sizeof(T);

    //this is kind of silly, but a byte isn't always 8 bits,
    //so we can't just say 0xFF
    //I really can't believe I bothered to handle this case.
    //Oh well
    char mask = 0x0; //a char is always 1 byte though
    for( int i = 0; i < CHAR_BIT; ++i )
    {
        mask = (mask << 1) | 0x01; 
    }
    //index needs to handle negative so we can count down
    for( int i = sizeof(T) - 1; i >= 0; --i )
    {
        T tmp = data >> i * CHAR_BIT; 
        state[i] = tmp & mask; //We still assume 8 here... oh well
    }
}
*/

template<class T>
bool serialize( T data, char*& state, unsigned int& size )
{
    size = sizeof(T);
    state = new char[size];
    memcpy( state, (void*)&data, size );
}

int main()
{
    //int i = 0x123456AA;
    cout << sizeof(long long) << endl;
    long long i = 0x123456789ABCDEF0LL;
//    i = i << sizeof(long) * CHAR_BIT;
//    i = i | 0x09ABCDEF;
    unsigned int s = 0;
    char* state = NULL;
    serialize( i, state, s );
    for( int i = s - 1; i >= 0; --i )
    {
        printf( "%x,", int(state[i]) & 0xFF ); 
    }
}

