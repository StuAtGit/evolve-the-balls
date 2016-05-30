#include <iostream>
#include <string.h>

using namespace std;

template<class T>
bool serialize( T data, char*& state, unsigned int& size )
{
    size = sizeof(T);
    state = new char[size];
    return memcpy( state, (void*)&data, size );
}

int main()
{
}

