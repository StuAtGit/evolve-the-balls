/**
Used to have non-portable methods, but these are gradually being filtered out to the Utility class
Now just has the consumeComments method
**/
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

#define print_debug false 
#define debug_out cout

#ifndef _CONSUMECOMMENTS_
#define _CONSUMECOMMENTS_
void consumeComments( string& word, ifstream& fin, const string commentMark, bool debug )
{
    bool moreLines = false;
    int count = 0;
    if( debug )
    {
        cout << "Notify: Entering consumeComments" << endl;
    }
  
    //if the line begins with the comment mark, consume it and move on 
    while( word.find( commentMark ) == 0 || moreLines )
    {
        word = "";
        count++;
        char dump;
        //might be easier with some sort of fin.seek()
        
        while( fin.peek() != EOF && fin.peek() != '\n' )
        {
            fin >> dump;
            if( debug )
            {
                cout << "Notify: dumping: " << dump << endl;
            }
        }

        if( fin.peek() == '\n' )
        {
            char newline = '\n';
            fin.read( &newline, sizeof( newline ) );
        }
       
        if( fin.peek() == '#' )
        {
            moreLines = true;
        }
        else
        {
            moreLines = false;
        }
    }

    if( count > 0 && fin.peek() != EOF )
    {
        fin >> word;
    }

    if( debug )
    {
        cout << "Notify: exiting consumeComments" << endl;
        cout << "Notify: word now: " << word << endl;
    }
}
#endif
