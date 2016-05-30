#ifndef _STU_UTIL_NAMESPACE_H
#define _STU_UTIL_NAMESPACE_H

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <exception>
#include <glob.h>
#include <sys/time.h>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <OGRE/Ogre.h>
#include "Logger.h"

using namespace std;
using namespace boost;
using namespace evolve;

namespace evolve
{
/**
An amalgam of useful methods
String conversions, trim, etc,
Nice place to wrap system calls like rand() & strtoll that may cause portability grief
*/
namespace Utility
{

    static Logger logger;

    /**
     * SetDefaultValue methods set the variable used in stringTo
     * to a sane error value
     * */
    static inline void setDefaultValue( unsigned int& val )
    {
        val = 0;
    }

    static inline void setDefaultValue( int& val )
    {
        val = 0;
    }

    static inline void setDefaultValue( long& val )
    {
        val = 0;
    }
 
    static inline void setDefaultValue( long long& val )
    {
        val = 0;
    }
 
    static inline void setDefaultValue( double& val )
    {
        val = 0;
    }
 
    static inline void setDefaultValue( float& val )
    {
        val = 0;
    }
    
    static inline void setDefaultValue( string& val )
    {
        val = "";
    }   
    /**
    Converts a string to a <T>
    Bascially wraps around boost::lexical_cast and does error handling 
    Verifies we have a number 
    @param number the string to convert
    @param success if the conversion succeeded
    @return the integer. If an error occurs, will return -1
    */
    template<class T>
    static inline T stringTo( string input, bool& success, bool printDebug = true )
    {
        success = true;
        try
        {
            return lexical_cast<T>( input ); 
        }
        catch( const exception& e )
        {
            logger.print( "Warning: in Utility::stringTo<T>, cast failed: " + lexical_cast<string>(e.what()) + " input was: " + input, printDebug );
            T t;
            success = false;
            Utility::setDefaultValue( t );
            return t;
        }
    }

    /**
     * Converts a given variable of type T to a string
     * Currently uses the boost::lexical_cast method, and can
     * handle any type that that library can handle
     * @param type the variable you wish to be coverted to a string
     * @param success whether the conversion was successful
     * @return the string representation of type
     */
    template<class T>
    static inline string toString( T type, bool& success, bool printDebug = true )
    {
        success = true;
        try
        {
            return lexical_cast<string>( type ); 
        }
        catch( const exception& e )
        {
            logger.print( "Warning: in Utility::toString<T>, cast failed ", printDebug );
            logger.print( "Warning: the reason given was: " + lexical_cast<string>(e.what()), printDebug );
            success = false;
            return "";
        }
       
    }

    /**
     * Converts a given variable of type T to a string (lazy edition)
     * I just didn't bother with the error check enough to justify it..
     * Currently uses the boost::lexical_cast method, and can
     * handle any type that that library can handle
     * @param type the variable you wish to be coverted to a string
     * @param success whether the conversion was successful
     * @return the string representation of type
     */
    template<class T>
    static inline string toString( T type ) //debug is hard-coded to "on"
    {
        try
        {
            return lexical_cast<string>( type ); 
        }
        catch( const exception& e )
        {
            logger.print( "Warning: in Utility::toString<T> (!lazy!), cast failed ", true );
            logger.print( "Warning: the reason given was: " + lexical_cast<string>(e.what()), true );
            return "";
        }
       
    }


    /**
    method to glob for filenames according to a pattern
    special characters recognized: * ? [ ~
    @param pattern the pattern to glob for 
    @returns a vector of the filenames that matched the pattern
    */
    static inline vector<string> globFiles( string pattern, bool printDebug )
    {
        glob_t pglob;

        int ret = glob( pattern.c_str(), GLOB_TILDE | GLOB_MARK, NULL, &pglob );  
        vector<string> files;

        if( ret == 0 )
        {
            for( unsigned int i = 0; i < pglob.gl_pathc; ++i )
            {   
                string file = pglob.gl_pathv[i];
                files.push_back( file ); 
            }
        }
        logger.print( "Error: error in globfiles", ret != 0 && printDebug );

        globfree( &pglob );
        
        return files;
        
        
    }

    /**
    * parses a string representation of a vector into a vector of ints
    * (deprecated)
    * @param vec the string representation of the vector
    * @param success whether or not the parse was successfully completed
    * @param delim the character in the string delimiting elements
    */  
    template <class T>
    static inline vector<T> parseVector( const string& vec, bool& success, bool printDebug, char delim = ',' )
    {
        vector<T> ret;
        for( unsigned int i = 0; i < vec.size(); ++i )
        {
            string entry = "";
            while( i < vec.size() && vec[i] != ',' )
            {
                entry += vec[i];
                ++i;
            }
            success = false;            
            T tmp;
            tmp = stringTo<T>( entry, success, printDebug );
            if( !success )
            {
                logger.print( "Warning: conversion failed in parseVector", printDebug );
                return ret;
            }
            ret.push_back( tmp );
        }
        success = true;
        return ret;
    }
 
    static inline unsigned int zeroOut( unsigned long& pos )
    {
        if( pos == string::npos )
            pos = 0;
        return pos;
    }

    static inline string makeTabs( unsigned int count )
    {
        string ret;
        for( unsigned int i = 0; i < count; ++i )
        {
            ret += "    ";
        }
        return ret;
    }

    static inline string trim( const string& s )
    {
        if( s.length() == 0 )
        {
            return s;
        }

        unsigned long firstNotWhite = s.find_first_not_of( " \t\n" ); 
        zeroOut( firstNotWhite );

        unsigned long lastNotWhite = s.find_last_not_of( " \t\n" );
        zeroOut( lastNotWhite );
        
        return s.substr( firstNotWhite, lastNotWhite - firstNotWhite + 1 );
    }

    /**
     * @param vec - string to turn into a list
     * @param success indicates a conversion failure (if false)
     * @param delims = delims to tokenize by, defaults to ","
     * */
    template <class T>
    static inline vector<T> split( const string& vec, bool& success, const string& delims = "," )
    {
        vector<T> list;
        char_separator<char> sep( delims.c_str() );
        tokenizer< char_separator<char> > tokens( vec, sep );
        tokenizer< char_separator<char> >::iterator i = tokens.begin();
        for( ; i != tokens.end(); ++i )
        {
            T tmp;
            tmp = stringTo<T>( Utility::trim(*i), success, true );
            list.push_back( tmp );
        }
        return list;
    }
   
    /**
     * lazy version
     */
    template <class T>
    static inline vector<T> split( const string& vec, const string& delims = "," )
    {
        bool success = false;
        return split<T>( vec, success, delims );
    }

    /**
    * parses a string representation of a map into a map
    * @param mapString the string representation of the map
    * @param success whether or not the parse successfully completed
    * @param itemDelim the character that deliminates between keys and values
    * @param pairStartDelim the character that indicates the key value pair will start
    * @param pairEndDelim the character that
    */ 
    template <class K, class V>
    static inline map<K,V> parseMap( const string& mapString, 
                       bool& success, bool printDebug, char itemDelim = ',',
                       char pairStartDelim = '(', char pairEndDelim = ')' )
    {
        success = true;
        map<K,V> ret;
        for( unsigned int i = 0; i < mapString.size(); ++i )
        {
            while( i < mapString.size() && mapString[i] != pairStartDelim )
            {
                ++i;
            }

            if( i < mapString.size() && mapString[i] == pairStartDelim )
            {
                ++i;
            }

            string keyString = "";
            while( i < mapString.size() && mapString[i] != itemDelim )
            {
                keyString += mapString[i];
                ++i;
            }
            K key;
            key = Utility::stringTo<K>( keyString, success, printDebug ); 
            if( !success )
            {
                logger.print( "Warning: conversion of " + keyString + " to a key failed in parseMap, typename of key is:" + typeid(K).name(), printDebug );
                return ret;
            }

            string valueString = "";
            //skip over the deliminator
            if( i < mapString.size() && mapString[i] == itemDelim )
            {
                ++i;
            }
            while( i < mapString.size() && mapString[i] != pairEndDelim )
            {
                valueString += mapString[i];
                ++i;
            }
            V value;
            value = Utility::stringTo<V>( valueString, success, printDebug ); 
            if( !success )
            {
                logger.print( "Warning: conversion of " + valueString + " to a value failed in parseMap, typeid of value is: " + typeid(V).name(), printDebug );
                return ret;
            }

            ret[key] = value;
        } 

        return ret;
    }

    /**
    Handy util function for re-assembling the elements of the vector (into a string)
    Note that it only works if the given type T can be turned into a string using Utility::toString
    a vector of strings into one big string)
    @param vec the vector to assemble
    @param delim a delimiter to insert between vector elements
    @param doDelim whether or not to add delimiters between the vector elements
    */
    template <class T>
    static inline string vectorToString( const vector<T>& vec, bool& success, string delim = ",", bool doDelim = true )
    {
        string temp;
        success = false;
        for( unsigned int i = 0; i < vec.size(); ++i )
        {
            if( !doDelim )
            {
                temp += Utility::toString<T>( vec[i], success, true );
                if( !success )
                {
                    logger.print( "Warning: in Utility::assembleVector, failed to convert type to string" );
                    return "";
                }
            }
            else
            {
                if( i != 0 )
                {
                    temp += (delim + Utility::toString<T>( vec[i], success, true ));
                }
                else
                {
                    temp += Utility::toString<T>( vec[i], success, true );
                }
                if( !success )
                {
                    logger.print( "Warning: in Utility::assembleVector, failed to convert type to string" );
                    return "";
                }
            }
        }

        return temp;
    }

    /** lazy version **/
    template <class T>
    static inline string vectorToString( const vector<T>& vec, string delim = ",", bool doDelim = true )
    {
        bool success;
        return Utility::vectorToString( vec, success, delim, doDelim );
    }

    /**
    * Constructs a string representation of a hash (a map keyed with strings)
    * @param hash The hash to assemble
    * @param itemDelim the character to insert between the key and the value
    * @param pairStartDelim the character to insert before a pair starts
    * @param pairEndDelim the character to insert after the pair ends 
    */ 
    template <class K, class T>
    static inline string mapToString( const map<K,T>& hash, bool& success, string itemDelim = ","
                        , string pairStartDelim = "(", string pairEndDelim = ")"  )
    {
        string ret = "";
        success = true;
        ostringstream ostream1;
        ostringstream ostream2;
        for( typename map<K,T>::const_iterator i = hash.begin(); i != hash.end(); ++i )
        {
            ostream1 << (*i).first;
            ostream2 << (*i).second;
            ret += pairStartDelim + ostream1.str() + itemDelim + ostream2.str() + pairEndDelim;
            ostream1.str( "" );
            ostream2.str( "" );
        }

        return ret;
    }

    /**
    * returns a random number offset <= x < offset + range.
    * (need to verify range spec)
    * @param offset The lowest value possible
    * @param range The cardinality (number) of possible values
    */ 
    static inline int getRandomInteger( int offset, int range, bool print_debug )
    {
        if( range <= 0 ) 
        {
            logger.print( "Warning: passed a range of: " + lexical_cast<string>(range) + " to getRandomInteger, returning zero", print_debug );
            return 0;
        }  
        timeval t;
        gettimeofday( &t, NULL );
        unsigned int seed = t.tv_usec;
        return offset + rand_r( &seed ) % range;
    }

    /**
    *returns the keys to a map 
    *@param hash The map to get the keys from
    */
    template <class K, class T>
    static inline vector<K> getMapKeys( const map<K,T>& hash )
    {
        vector<K> keys;
        for( typename map<K,T>::const_iterator i = hash.begin(); i != hash.end(); ++i )
        {
            keys.push_back( (*i).first );
        } 

        return keys;
    }

   
    static inline vector<double> toVector( const Ogre::Vector3& vec )
    {
        vector<double> ret;
        ret.push_back( vec.x );
        ret.push_back( vec.y );
        ret.push_back( vec.z );
        return ret;
    }
    
    static inline Ogre::Vector3 toVector3( const vector<double>& vec )
    {
        Ogre::Vector3 ret;
        if( vec.size() < 3 )
        {
            logger.print( "Warning: Utility::vectorToVector3, argument less then 3 dimensions, returning empty Vector3" );
            return ret;
        }

        ret.x = vec[0];
        ret.y = vec[1];
        ret.z = vec[2];
        return ret;
    }

    /**
     * Gets the 3 legs of the axis-aligned bounding box around an ogre scene node.
     * Useful when working with Ogre & ODE
     * @param object the entity/movableObject to get the aabb for
     * @param legx the leg in the x direction
     * @param legy the leg in the y direction
     * @param legz the leg in the z direction
     */
    static inline void getAABBLegs( Ogre::MovableObject* object, double& legx, double& legy, double legz )
    {
        Ogre::AxisAlignedBox box = object->getBoundingBox();
        Ogre::Vector3 flb = box.getCorner( Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM );
        Ogre::Vector3 frb = box.getCorner( Ogre::AxisAlignedBox::FAR_RIGHT_BOTTOM );
        Ogre::Vector3 nlb = box.getCorner( Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM );
        Ogre::Vector3 nlt = box.getCorner( Ogre::AxisAlignedBox::NEAR_LEFT_TOP );
        legx = ( flb - frb ).length();
        legy = ( nlt - nlb ).length();
        legz = ( nlb - flb ).length();
    }

    /**
     * Gets the 3 legs of the axis-aligned bounding box around an ogre scene node.
     * Useful when working with Ogre & ODE
     * @param sceneNode the node to get the aabb for
     * @param name the name to send in to the getAttachedObject call for ogre. 
     *             The name of the entity for the node, I believe.
     * @param legx the leg in the x direction
     * @param legy the leg in the y direction
     * @param legz the leg in the z direction
     */
    static inline void getAABBLegs( Ogre::SceneNode* sceneNode, const string& name, double& legx, double& legy, double legz )
    {
        getAABBLegs( sceneNode->getAttachedObject( name ), legx, legy, legz );
    }


    template <class T>
    static inline vector<T> makeVector( T a, T b, T c)
    {
        vector<T> ret;
        ret.push_back(a);
        ret.push_back(b);
        ret.push_back(c);
        return ret;
    }

    template <class T>
    static inline T max( T a, T b )
    {
        return a > b ? a : b;
    }
};
};
#endif

