#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <Ogre.h>

#include "OgreGDKCodec.h"

using namespace std;
using namespace Ogre;

int main()
{
    GDKCodec::startup();
    GDKCodec::shutdown();
}
