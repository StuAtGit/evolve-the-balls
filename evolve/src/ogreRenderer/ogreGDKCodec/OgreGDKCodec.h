#ifndef STU_OGRE_GDK_CODEC_H
#define STU_OGRE_GDK_CODEC_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
namespace GDK
{
    #include <gdk/gdk.h>
    #include <gdk-pixbuf/gdk-pixbuf.h>
}
#include "OgreImageCodec.h"
#include "OgreStableHeaders.h"

#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreLogManager.h"
#include "OgreException.h"

#include "CommonAspect.h"
#include "Utility.h"

using namespace std;
using namespace Ogre;
using namespace GDK;

namespace Ogre
{
    class GDKCodec : public ImageCodec, public CommonAspect
    {
    public:
        GDKCodec( const string& type );
        friend GDKCodec* initGDKCodec( const string& type );
        virtual ~GDKCodec();
        virtual String getType() const;
     
        DataStreamPtr code( MemoryDataStreamPtr& input, CodecDataPtr& pData ) const;
        void codeToFile( MemoryDataStreamPtr& input, const String& outFileName, CodecDataPtr& pData ) const;
        Codec::DecodeResult decode( DataStreamPtr& input ) const;
        /** Static method to startup and register the PGF codec **/
        static void startup();
        /** Static method to shutdown and unregister the PGF codec **/
        static void shutdown();
         

     private:
        bool checkGDKError( GError* gerror );   
        GError* gdkError;
        string type;
    };

    GDKCodec* initGDKCodec( const string& type );
}
#endif
