#ifndef _MATRIX_STU_CC
#define _MATRIX_STU_CC

#include <vector>
#include <math.h>
#include <string>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include "Utility.h"
#include "Logger.h"

using namespace std;
using namespace boost;
using namespace evolve;

namespace evolve
{
/**
A set of methods that perform matrix operations
(matrices are represented by vectors)
They are expected to be laid out like so:\n
0 1 2\n
3 4 5\n
7 8 9\n
*/
namespace Matrix
{
    /**
     * Logger for the matrix methods
     */
    static Logger logger;

    /**
    Adds two vectors together element-wise, using the length of the
    smallest vector.
    @param vec1 a vector to add
    @param vec2 ditto
    */
    template <class T>
    static inline vector<T> matrixAdd( const vector<T>& vec1, const vector<T>& vec2 )
    {
        int maxLength;
        vector<T> result;
        if( vec1.size() < vec2.size() )
        {
            maxLength = vec1.size();
        }
        else
        {
            maxLength = vec2.size();
        }

        for( int i = 0; i < maxLength; ++i )
        {
            result.push_back( vec1[i] + vec2[i] );
        }

        return result;
    }

    /**
    Returns a string representation of a matrix with 9 elements
    @param vec the vector to get a string representation of
    @return at string representation of the vector as a matrix
    */
    template <class T>
    static inline string matrix9ToString( const vector<T>& vec, bool printDebug = true )
    {
        if( vec.size() != 9 )
        {
            logger.print( "Warning: in print matrix, size of matrix != 9, its: " + lexical_cast<string>(vec.size()), printDebug );

            return "";
        }

        ostringstream ostream;
        for( int i = 0; i < 9; ++i )
        {
            ostream << vec[i] << " ";
            if( (i + 1) % 3 == 0 )
            {
                ostream << '\n';
            }
        }

        return ostream.str();
    }

    /**
    * Rotates the given vector around the Y axis
    * @param vec the vector to rotate (must be a 3 dimensinal vector)
    * @param degree How many degrees to rotate the vector (in degrees)
    */
    template <class T>
    static inline vector<T> vector3RotateY( const vector<T>& vec, double degree, bool printDebug = true,
                                      bool doNotify = false )
    {
        degree *= (3.14/180);
        if( vec.size() != 3 )
        {
            if( printDebug )
            {
                logger.print( "Warning: this function will only work on vectors of size 3 (only works in a three-dimensional manifold", printDebug );
                return vec;
            }
        }
        vector<T> result;
        vector<T> rotMatrix;
        rotMatrix.resize(9);
        rotMatrix[0] = cos(degree);
        rotMatrix[1] = 0;
        rotMatrix[2] = -1 * sin( degree );
        rotMatrix[3] = 0;
        rotMatrix[4] = 1;
        rotMatrix[5] = 0;
        rotMatrix[6] = sin( degree );
        rotMatrix[7] = 0;
        rotMatrix[8] = cos( degree );

        logger.print( "Notify: rotationMatrix \n " + matrix9ToString( rotMatrix ) , printDebug && doNotify );

        return vector3MultMatrix( vec, rotMatrix, printDebug );
    }

    /**
    * vector3Rotate
    * rotates about an almost-arbitrary axis:
    * the axis is assumed to have one of its
    * endpoints at at the origin (simplifies/speeds the calcuation 
    * and serves our purposes)
    * @param vec the vector to rotate
    * @param rotateAxis the axis to rotate the vector about
    * @param degree the amount to rotate the vector (in degrees)
    * @param isParallel Out argument, indicating we tried to rotate an axis parallel to the rotation axis
    * @return the rotated vector
    * thanks to http://www.mines.edu/~gmurray/ArbitraryAxisRotation/ArbitraryAxisRotation.html
    */
    template <class T>
    static inline vector<T> vector3Rotate( const vector<T>& vec, const vector<T>& rotateAxis, double degree, bool& isParallel, bool printDebug = true, bool doNotify = false )
    {
        if( vec.size() != 3 || rotateAxis.size() != 3 )
        {
            logger.print( "Warning: this function will only work on vectors in a three dimensional manifold (the vector size & rotation axis size must be 3)", printDebug );
            return vec;
        }

        degree *= (3.14/180);
        vector<T> result;
        vector<T> rotMatrix;
        double u = rotateAxis[0];
        double u2 = u*u;
        double v = rotateAxis[1];
        double v2 = v*v;
        double w = rotateAxis[2];
        double w2 = w*w;
        double L = sqrt( u * u + v * v + w * w );
        double L2 = L*L;
        double sine = sin( degree );
        double cosine = cos( degree );

        //This test appears to work?
        //If this starts failing, use -ffloat-store, and put 0.0 into a variable
        //(see g++ man page)
        if( L2 == 0.0 )
        {
            logger.print( "Warning the magnitude of the rotation axis was zero, (the vector being rotated is parallel to the rotation axis)", printDebug );
            isParallel = true;
            return vec;
        }

        isParallel = false;
        rotMatrix.resize(9);
        rotMatrix[0] = (u2 + (v2 + w2) * cosine) / L2;
        rotMatrix[1] = (u * v * (1 - cosine) - w * L * sine) / L2;
        rotMatrix[2] = (u * w * (1 - cosine) + v * L * sine) / L2;

        rotMatrix[3] = (u * v * (1 - cosine) + w * L * sine) / L2;
        rotMatrix[4] = (v2 + (u2 + w2) * cosine) / L2;
        rotMatrix[5] = (v * w * (1 - cosine) - u * L * sine) / L2;

        rotMatrix[6] = (u * w * (1 - cosine) - v * L * sine) / L2;
        rotMatrix[7] = (v * w * (1 - cosine) + u * L * sine) / L2;
        rotMatrix[8] = (w2 + (u2 + v2) * cosine) / L2;

        logger.print( "Notify: rotation matrix is: \n" + matrix9ToString( rotMatrix ), printDebug && doNotify );

        return vector3MultMatrix( vec, rotMatrix, printDebug );
    }

    /**
    *vectorCrossProduct
    *Takes the cross product of two vectors
    *@param vec1 vector 1
    *@param vec2 vector 2
    *@returns the crossProduct
    */
    template <class T>
    static inline vector<T> vectorCrossProduct( const vector<T>& vec1, const vector<T>& vec2, bool printDebug = true, bool doNotify = false )
    {
        logger.print( "Notify: entered vectorDotProduct", printDebug && doNotify );
        vector<T> result;
        if( vec1.size() != 3 || vec2.size() != 3 )
        {
            logger.print( "Warning: vectorDotProdcut takes two vectors of size 3 (only works in a 3-manifold), one of the vectors was not 3", printDebug );

            T defaultValue;
            Utility::setDefaultValue( defaultValue );
            result.push_back( defaultValue );
            result.push_back( defaultValue );
            result.push_back( defaultValue );
            return result;
        }

        //x
        result.push_back( vec1[1] * vec2[2] - vec1[2] * vec2[1] );
        //y
        result.push_back( vec1[0] * vec2[2] - vec1[2] * vec2[0] );
        //z
        result.push_back( vec1[0] * vec2[1] - vec1[1] * vec2[0] );

        return result;
    }

    /**handy function to initialize a vector to a point
     * Does:
     * pt.push_back( x );
     * pt.push_back( y );
     * pt.push_back( z );
     * Note that repeated calls can be used to initialize a matrix
     *
     * @param pt the vector to add the coordinates to
     * @param x the first element pushed back
     * @param y the second element pushed back
     * @param z the third element pushed back
     */
    template <class T>
    static inline void initializePoint( vector<T>& pt, T x, T y, T z )
    {
        pt.push_back( x );
        pt.push_back( y );
        pt.push_back( z );
    }
    
    /**
     * Generates the normal from three points like so:
     * (p1 - p2) x (p1 - p3)
     * To get normal facing the other way, just negate
     * the points are assumed to have 3 coordinates (to be in a 3-manifold)
     * @param p1 the first point
     * @param p2 the second point
     * @param p3 the third point
     * @return (p1 - p2) x (p1 - p3)
     */
    template <class T>
    static inline vector<T> getNormal( const vector<T>& p1, const vector<T>& p2, const vector<T>& p3, bool printDebug = true, bool doNotify = false )
    {
        logger.print( "Notify: entered Matrix::getNormal", doNotify && printDebug );
        vector<T> ret;
        vector<T> tmp;
        T defaultValue;
        Utility::setDefaultValue( defaultValue );
        Matrix::initializePoint( ret, defaultValue, defaultValue, defaultValue );
        Matrix::initializePoint( tmp, defaultValue, defaultValue, defaultValue );
        if( p1.size() < 3 || p2.size() < 3 || p3.size() < 3 )
        {
            logger.print( "Warning: p1 size: " + lexical_cast<string>( p1.size() ) +
                          " p2 size: " + lexical_cast<string>( p2.size() ) + 
                          " p3 size: " + lexical_cast<string>( p3.size() ) +
                          " all sizes should be at least 3 ", printDebug );
             return ret;
        }

        ret[0] = p1[0] - p2[0];
        ret[1] = p1[1] - p2[1];
        ret[2] = p1[2] - p2[2];

        tmp[0] = p2[0] - p3[0];
        tmp[1] = p2[1] - p3[1];
        tmp[2] = p2[2] - p3[2];

        ret = vectorCrossProduct( ret, tmp );

        return ret;
    }

    /**
    Multiplies a 3 dimensional vector by a matrix
    @param vec3 a 3 dimensional vector
    @param matrix the matrix to multiply the vector by
    @return the result of the multiplication (a 3 dimensional vector)
    */
    template <class T>
    static inline vector<T> vector3MultMatrix( const vector<T>& vec3, const vector<T>& matrix, bool printDebug = true, bool doNotify = false )
    {
        if( vec3.size() != 3 )
        {
            logger.print( "Warning: the first argument: vec3 needs to be a three-dimensional vector", printDebug );
            return vec3;
        }

        if( matrix.size() % 3 != 0 )
        {
            logger.print( "Warning: matrix needs to be of a size that is a integer multiple of 3", printDebug );

            return vec3;
        }

        vector<T> result;
        result.resize(3);
        int k = 0;
        for( unsigned int i = 0 ; i < matrix.size(); i += 3 )
        {
            for( int j = 0; j < 3; ++j )
            {
                logger.print( "i + j: " + lexical_cast<string>(i + j) + " matrix[i + j]: " + lexical_cast<string>( matrix[i + j] ) + " vec: " + lexical_cast<string>( vec3[j]), printDebug && doNotify );

                result[k] += matrix[i + j] * vec3[j];

                logger.print( "result[" + lexical_cast<string>(k) + "]" + lexical_cast<string>(k), printDebug && doNotify );
            }

            ++k;
        }

        return result;
    }

    template <class T>
    static inline T magnitude( vector<T> vec )
    {
        T mag;
        for( unsigned int i = 0; i < vec.size(); ++i )
        {
            mag += vec[i] * vec[i];
        }
        return sqrt( mag );
    }
    /**
    Copies a vector into an array
    The caller is reponsible for allocating the appropriate amount of memory
    in the array to hold result!
    @param result the array result
    @param vec the vector to copy into the array
    */
    template <class T>
    static inline void vectorToArray( T result[], const vector<T>& vec )
    {
        for( unsigned int i = 0; i < vec.size(); ++i )
        {
            result[i] = vec[i];
        }
    }

    /**
    Copies an array into a vector
    @param vec the vector result
    @param array the array of values to be copied 
    @param size the size of the array. The caller is responsible for 
           getting this right!
    */
    template <class T>
    static inline void arrayToVector( vector<T>& vec, T array[], unsigned int size )
    {
        for( unsigned int i = 0; i < size; ++i )
        {
            vec.push_back( array[i] );
        }
    }

    /**
     * Gets the midpoint between two points (in Euclidean space)
     * @param midpoint the midpoint return value (will be resized() appropriately)
     * @param point1 the first point
     * @param point2 the second point
     */
    template <class T>
    static inline bool midpoint( vector<T>& midpoint, const vector<T>& point1, const vector<T>& point2 )
    {
        if( point1.size() != point2.size() )
        {
            logger.print( "Warning: in Matrix::midpoint, the dimensions of the points are not equal" );
            logger.print( "Point1: " + Utility::vectorToString( point1 ) );
            logger.print( "Point2: " + Utility::vectorToString( point2 ) );
            return false; 
        }
        midpoint.resize(point1.size());
        for( unsigned int i = 0; i < point1.size(); ++i )
        {
            midpoint[i] = (point1[i] + point2[i]) / 2;
        }
        return true;
    }
};
};
#endif
