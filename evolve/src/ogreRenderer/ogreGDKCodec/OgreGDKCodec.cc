#ifndef STU_OGRE_GDK_CODEC_CC
#define STU_OGRE_GDK_CODEC_CC

#include "OgreGDKCodec.h"


 GDKCodec::GDKCodec( const string& type )
 {
    //this->gdkError = (GError**)malloc( sizeof( GError* ) );
    //*(this->gdkError) = NULL; 
    char** fakeArgv = NULL;
    int fakeArgc = 0;
    gdk_init( &fakeArgc, &fakeArgv );
    this->type = type;
 }

 GDKCodec::~GDKCodec()
 {
    free( this->gdkError );
 }

 String GDKCodec::getType() const
 {
    return this->type;
 }

 DataStreamPtr GDKCodec::code( MemoryDataStreamPtr& input, CodecDataPtr& pData ) const
 {
    logger.print( "Warning: GDKCodec::codeToFile NOT IMPLEMENTED!" );
 }

 void GDKCodec::codeToFile( MemoryDataStreamPtr& input, const String& outFileName, CodecDataPtr& pData ) const
 {
    logger.print( "Warning: GDKCodec::codeToFile NOT IMPLEMENTED!" );
    /** TODO: implement this
     * Need to clarify MemoryDataStrreamPtr (vs MemoryDataStream - no Ptr) size()?, etc
    GdkPixbufLoader* imageLoader = gdk_pixbuf_loader_new();
    gdk_pixbuf_loader_write( imageLoader, (unsigned char*)input.getPtr(), input.size(), &(this->gdkError) );
    !(this->gdkError) && gdk_pixbuf_loader_close( imageLoader, &(this->gdkError) );
    if( this->gdkError )
    {
        logger.print( "In OgreGDKCodec::decode, gdk error: " + this->gdkError );
        return ret;
    }
    GdkPixbuf* pixbuf = gdk_pixbuf_loader_get_pixbuf( imageLoader );
    gdk_pixbuf_save( pixbuf, outFileName.c_str(), "jpeg", &(this->gdkError) );
    **/
 }

 Codec::DecodeResult GDKCodec::decode( DataStreamPtr& input ) const
 {
    MemoryDataStream memStream( input, true );
    DecodeResult ret;
    int w,h;

    GdkPixbufLoader* imageLoader = gdk_pixbuf_loader_new();
    gdk_pixbuf_loader_write( imageLoader, (unsigned char*)memStream.getPtr(), memStream.size(), (GError**)(&(this->gdkError)) );
    !(this->gdkError) && gdk_pixbuf_loader_close( imageLoader, (GError**)(&(this->gdkError)) );
    if( this->gdkError )
    {
        bool success = false;
        logger.print( "Warning: In GDKCodec::decode, gdk error: " + Utility::toString( (char*)(this->gdkError->message), success ) );
        return ret;
    }
    GdkPixbuf* pixbuf = gdk_pixbuf_loader_get_pixbuf( imageLoader );
    unsigned char* data = gdk_pixbuf_get_pixels( pixbuf );
    w = gdk_pixbuf_get_width( pixbuf ); 
    h = gdk_pixbuf_get_height( pixbuf ); 

    ImageData* imageData = new ImageData();
    imageData->depth = 1;//?? bits_per_sample?? is it 1? bytes or bits?
    imageData->width = w;
    imageData->height = h;
    imageData->num_mipmaps = 0;
    imageData->flags = 0;
    bool hasAlpha = gdk_pixbuf_get_has_alpha( pixbuf );
    hasAlpha && (imageData->format = PF_BYTE_RGBA);
    !hasAlpha && (imageData->format = PF_BYTE_RGB);

   unsigned dstPitch = imageData->width * PixelUtil::getNumElemBytes(imageData->format);
   imageData->size = dstPitch * imageData->height;
   MemoryDataStreamPtr output( new MemoryDataStream( data, imageData->size, true ) );

   // return decoded image
   ret.first = output;
   ret.second = CodecDataPtr(imageData);
   return ret;
 }
 
 GDKCodec* Ogre::initGDKCodec( const string& type )
 {
    return new GDKCodec( type );
 }

 
 /** Static method to startup and register the PGF codec **/
 void GDKCodec::startup()
 {
    Codec::registerCodec( initGDKCodec( "jpg" ) );
    Codec::registerCodec( initGDKCodec( "png" ) );
    Codec::registerCodec( initGDKCodec( "xpm" ) );
    Codec::registerCodec( initGDKCodec( "tiff" ) );
    Codec::registerCodec( initGDKCodec( "pnm" ) );
    Codec::registerCodec( initGDKCodec( "ras" ) );
    Codec::registerCodec( initGDKCodec( "bmp" ) );
    Codec::registerCodec( initGDKCodec( "gif" ) );
 }

 /** Static method to shutdown and unregister the PGF codec **/
 void GDKCodec::shutdown()
 {
 }
 

#endif
