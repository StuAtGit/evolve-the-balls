#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace C
{
    #include <gdk/gdk.h>
    #include <gdk-pixbuf/gdk-pixbuf.h>
}

using namespace std;
using namespace C;

bool handle_gdk_error( GError* gdkError )
{
    if( gdkError )
    {
        cerr << "Error in gdk: " << gdkError->message << endl;
    }

    return !gdkError;
}

int main( int argc, char** argv )
{
    ifstream fin;
    string filename = "Media/testGDK.jpg";
    fin.open( filename.c_str(), ios::in | ios::binary );
    if( !fin.good() )
    {
        cout << "Error: can't open file: " << filename << endl;
        return -1;
    }

    fin.seekg( 0, ios::end );
    unsigned int fsize = fin.tellg(); 
    unsigned char* buffer = new unsigned char[fsize];
    fin.seekg( 0, ios::beg );
    fin.read( (char*)buffer, fsize );

    char** test = NULL;
    //to test what happens when a constructor calls this multiple times
    gdk_init( 0, &test );
    gdk_init( 0, &test );
    gdk_init( 0, &test );

    GError* gdkError = NULL;
    GdkPixbufLoader* imageLoader = gdk_pixbuf_loader_new();
    gdk_pixbuf_loader_write( imageLoader, buffer, fsize, &gdkError );
    if( !handle_gdk_error( gdkError ) ) { cout << "loader" << endl; }
    !gdkError && gdk_pixbuf_loader_close( imageLoader, &gdkError ); 
    if( !handle_gdk_error( gdkError ) ) { cout << "loader" << endl;  }

    GdkPixbuf* pixbuf = gdk_pixbuf_loader_get_pixbuf( imageLoader );
    string filenameOut = "testGDKout.jpg";
    string type = "jpeg";
    !gdkError && cout << "test" << endl && gdk_pixbuf_save( pixbuf, filenameOut.c_str(), type.c_str(), &gdkError );
    if( !handle_gdk_error( gdkError ) ) { cout << "save" << endl; }

    unsigned char* data = gdk_pixbuf_get_pixels( pixbuf );
    unsigned int w = gdk_pixbuf_get_width( pixbuf ); 
    unsigned int h = gdk_pixbuf_get_height( pixbuf ); 

    cout << "has alpha: " << gdk_pixbuf_get_has_alpha( pixbuf ) << endl;
    cout << "width: " << w << endl;
    cout << "height: " << h << endl;
}


