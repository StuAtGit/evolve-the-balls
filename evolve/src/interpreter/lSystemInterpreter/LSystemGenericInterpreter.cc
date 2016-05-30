#ifndef STU_INTERPRETER_GENERIC_CC
#define STU_INTERPRETER_GENERIC_CC

#include "LSystemGenericInterpreter.h"

#define TURN_BOOST_FACTOR 1.5

LSystemGenericInterpreter::LSystemGenericInterpreter( const string& id, bool printDebug )
{
    logger.print( "Notify: entered LSystemGenericInterpreter::LSystemGenericInterpreter", this->doNotify && printDebug );
    this->id = id;
    this->doNotify = false;
    this->rightAngleKey = "RightAngle";
    this->leftAngleKey = "LeftAngle";
    this->upAngleKey = "UpAngle";
    this->downAngleKey = "DownAngle";
    this->atomicSizeKey = "AtomicSize";
    this->startPointKey = "StartPoint";
}

bool LSystemGenericInterpreter::getSystem( string& interpretedSystem, string& lSystemString, bool printDebug )
{
    vector<string> system;
   
    for( unsigned int i = 0; i < lSystemString.length(); ++i )
    {
        string word = "";
        for( ; i < lSystemString.length() && lSystemString[i] != ','; ++i )
        {
              word += lSystemString[i];
        }
        system.push_back( word );
    }

    //if a start point was given, parse it, and initialize turtle coordinates appropriately
    vector<double> turtlePoint;
    turtlePoint.push_back( 0 );
    turtlePoint.push_back( 0 );
    turtlePoint.push_back( 0 );

    if( this->systemAttributes.find( startPointKey ) != this->systemAttributes.end() && this->systemAttributes[startPointKey] != "" )
    {
        string startPoint = this->systemAttributes[startPointKey];
        unsigned int pointCounter = 0;
        for( unsigned int i = 0; i < startPoint.size(); ++i )
        {
            string currentCoordinate = "";
            //get the next point - tokenize point coordinates by comma...
            while( i < startPoint.size() && startPoint[i] != ',' )
            {
                currentCoordinate += startPoint[i];
                ++i;
            }
            bool success = false;
            double coordinate = Utility::stringTo<double>( currentCoordinate, success, printDebug );
            logger.print( "Warning: LSystemGenericInterpreter::getSystem(), badly formatted start value :" + currentCoordinate + " at index: " + lexical_cast<string>(i), printDebug && !success );
            if( pointCounter < 3 )
            {
                turtlePoint[pointCounter] = coordinate;
            }
            pointCounter++;
        }
    }

    unsigned int bracketScope = 0;

    vector< vector<double> > turtlePointStack;
    vector< vector<double> > turtleMovementStack; 
    vector< vector<double> > turtleCrossProductStack;

    turtlePointStack.push_back( turtlePoint );

    vector<double> turtleMovementAxis;

    //double turtleAngle = 0;
    vector<double> yAxis;
    vector<double> xAxis;

    //or the rotation axis for the movementAxis
    vector<double> turtleCrossProduct;
    yAxis.push_back( 0 );
    yAxis.push_back( 1 );
    yAxis.push_back( 0 );
    xAxis.push_back( 1 );
    xAxis.push_back( 0 );
    xAxis.push_back( 0 );
    turtleMovementAxis.push_back( 0 );
    turtleMovementAxis.push_back( 0 );
    turtleMovementAxis.push_back( -1 );
    turtleMovementStack.push_back( turtleMovementAxis );
    
    turtleCrossProduct.push_back( -1 );
    turtleCrossProduct.push_back( 0 );
    turtleCrossProduct.push_back( 0 );
    turtleCrossProductStack.push_back( turtleCrossProduct );

    vector<string> coordinates;
   
    ostringstream ostream; 

    //count that includes the start point element (i counter in loop does not)
    int elementCount = 0;
    ostream << "(" << turtlePoint[0] << "," << turtlePoint[1] << "," << turtlePoint[2] << ")";
    coordinates.push_back( ostream.str() );

    for( unsigned int i = 0; i < system.size(); ++i )
    {
        if( system[i] == "S" || system[i][0] == 'R' 
           || system[i][0] == 'L' || system[i][0] == 'U' || system[i][0] == 'D'
           || system[i][0] == '[' || system[i][0] == ']' )
        {
            ;
        }
        else
            continue;

        //count that includes the first element & ignores spurious elements
        elementCount++;

        if( system[i] == "S" )
        {
            bool success = false;
            double atomicSize = Utility::stringTo<double>( this->systemAttributes[atomicSizeKey], success, printDebug );
            logger.print( "Warning: in Utility::getSystem converting atomic size to double failed (check your config file)", printDebug && !success );
            turtlePoint[0] += turtleMovementAxis[0] * atomicSize; 
            turtlePoint[1] += turtleMovementAxis[1] * atomicSize;
            turtlePoint[2] += turtleMovementAxis[2] * atomicSize;

            ostream.str("");
            ostream << "(" << turtlePoint[0] << "," << turtlePoint[1] << "," << turtlePoint[2] << ")";
            coordinates.push_back( ostream.str() );
       }
        else if( system[i][0] == '[' )
        {
            bracketScope++;
            turtleMovementStack.push_back( turtleMovementAxis );
            turtlePointStack.push_back( turtlePoint ); 
            turtleCrossProductStack.push_back( turtleCrossProduct );
            coordinates.push_back( "[" );
        }
        else if( system[i][0] == ']' )
        {
            if( bracketScope <= 0 )
            {
                logger.print( "Warning: LSystemGenericInterpreter::getSystem() Warning: close ] with no open [", printDebug );
                continue;
            }
            bracketScope--; 

            //close the branch, and pop off our saved turtle state
            coordinates.push_back( "]" );

            turtleMovementAxis = turtleMovementStack[ turtleMovementStack.size() - 1 ];
            turtlePoint = turtlePointStack[ turtlePointStack.size() - 1 ];
            turtleCrossProduct = turtleCrossProductStack[ turtleCrossProductStack.size() - 1 ];
            
            turtleMovementStack.pop_back();
            turtlePointStack.pop_back();
            turtleCrossProductStack.pop_back();
        }
        else if( system[i][0] == 'R' || system[i][0] == 'L' )
        {
            bool success = false;
            double angle = 0;

            if( system[i][0] == 'R' )
            {
                angle = Utility::stringTo<double>( this->systemAttributes[rightAngleKey], success, printDebug );
                logger.print( "Warning: in LSystemGenericInterpreter::getSystem, bad right angle (check your config file): " + this->systemAttributes[rightAngleKey], 
                printDebug && (!success || this->systemAttributes[rightAngleKey] == "" ) );
            }
            else
            {
                angle = Utility::stringTo<double>( this->systemAttributes[leftAngleKey], success, printDebug );
                logger.print( "Warning: in LSystemGenericInterpreter::getSystem, bad left angle: (check your config file)" + this->systemAttributes[leftAngleKey],
                               printDebug && (!success || this->systemAttributes[leftAngleKey] == "" ) );
            }
            
            //do our left/right rotation 
            turtleMovementAxis = Matrix::vector3RotateY( turtleMovementAxis, angle );
            
            //now check if we can update the current rotation axis for Up/Down rotations
            vector<double> crossProductTemp = Matrix::vectorCrossProduct( turtleMovementAxis, yAxis ); 
            double mag = sqrt( crossProductTemp[0] * crossProductTemp[0] 
                               + crossProductTemp[1] * crossProductTemp[1] 
                               + crossProductTemp[2] * crossProductTemp[2] );
            
            //if new rotation axis makes sense, update
            if( mag != 0.0 )
            {
                turtleCrossProduct = crossProductTemp;
            }
            
            success = false;
            double atomicSize = Utility::stringTo<double>( this->systemAttributes[atomicSizeKey], success, printDebug );
            logger.print( "Warning: in Utility::getSystem converting atomic size to double failed (check your config file)", printDebug && !success );
            turtlePoint[0] += turtleMovementAxis[0] * (atomicSize * TURN_BOOST_FACTOR); 
            turtlePoint[1] += turtleMovementAxis[1] * (atomicSize * TURN_BOOST_FACTOR);
            turtlePoint[2] += turtleMovementAxis[2] * (atomicSize * TURN_BOOST_FACTOR);
            ostream.str("");
            ostream << "(" << turtlePoint[0] << "," << turtlePoint[1] << "," << turtlePoint[2] << ")";
            coordinates.push_back( ostream.str() );
 
        }
        else if( system[i][0] == 'U' || system[i][0] == 'D' )
        {
            bool success = false;
            double angle = 0;
            if( system[i][0] == 'U' )
            {
                angle = Utility::stringTo<double>( this->systemAttributes[upAngleKey], success, printDebug );
                logger.print( "Warning: in LSystemGenericInterpreter::getSystem, bad up angle: (check config file)" + this->systemAttributes[upAngleKey],
                               printDebug && (!success || this->systemAttributes[upAngleKey] == "" ) );
            }
            else
            {
                angle = Utility::stringTo<double>( this->systemAttributes[downAngleKey], success, printDebug );
                logger.print( "Warning: in LSystemGenericInterpreter::getSystem, bad down angle: (check config file)" + this->systemAttributes[downAngleKey],
                               printDebug && (!success || this->systemAttributes[downAngleKey] == "" ) );
            }
           
            bool isParallel = true;
            turtleMovementAxis = Matrix::vector3Rotate( turtleMovementAxis, turtleCrossProduct, angle, isParallel, printDebug );
            success = false;
            double atomicSize = Utility::stringTo<double>( this->systemAttributes[atomicSizeKey], success, printDebug );
            logger.print( "Warning: in Utility::getSystem converting atomic size to double failed (check your config file)", printDebug && !success );
            turtlePoint[0] += turtleMovementAxis[0] * (atomicSize * TURN_BOOST_FACTOR); 
            turtlePoint[1] += turtleMovementAxis[1] * (atomicSize * TURN_BOOST_FACTOR );
            turtlePoint[2] += turtleMovementAxis[2] * (atomicSize * TURN_BOOST_FACTOR );
            ostream.str("");
            ostream << "(" << turtlePoint[0] << "," << turtlePoint[1] << "," << turtlePoint[2] << ")";
            coordinates.push_back( ostream.str() );

        }
    }

    bool success = false;
    interpretedSystem = Utility::vectorToString( coordinates, success, "" ); 
    logger.print( "Warning: in Utility::getSystem coverting return value to string failed", printDebug && !success );
    return success;

}
bool LSystemGenericInterpreter::getSystem( string& interpretedSystem, System& lSystem, bool printDebug )
{
    logger.print( "Notify: entered LSystemGenericInterpreter::getSystem (object ref)", this->doNotify && printDebug );
    string lSystemString = lSystem.getSystem();
    return this->getSystem( interpretedSystem, lSystemString, printDebug ); 
}

map<int, vector< vector<double> > > LSystemGenericInterpreter::getPoints( const string& interpretedSystem, const Logger& logger, bool doNotify, bool printDebug )
{
    logger.print( "Notify: entered getSystemWithBranches ", printDebug && doNotify );
   
    unsigned int stack_cursor = 0;
    map< int, vector< vector<double> > > branches;
    vector< vector<double> > points;

    //this is all wonky because we want to do everything in one pass (I guess...)
    //Also it needs to be robust, since we could be parsing anything in the client/server arrangement.
    for( unsigned int i = 0; i < interpretedSystem.length(); ++i )
    {
        vector<double> point;
        //if we see a branch, we put what we have so far onto the previous branch level,
        //and increment our stack_cursor (this starts a new branch)
        if( interpretedSystem[i] == '[' )
        {
            branches[stack_cursor].insert( branches[stack_cursor].end(), points.begin(), points.end() );
            points.clear();
            ++stack_cursor;
            continue;
        }
        //close off the branch - save it's coordinates, and decrement the stack cursor
        else if( interpretedSystem[i] == ']' )
        {
            branches[stack_cursor].insert( branches[stack_cursor].end(), points.begin(), points.end() );
            points.clear();
            --stack_cursor;
            //skip trailing comma if formatted like so: [0-4(x,y,z),(x,y,z),], etc...
            if( i < interpretedSystem.length() - 1 && interpretedSystem[i + 1] == ',' )
            {
                ++i;
            }
            continue;
        }

        if( interpretedSystem[i] == '(' && i < interpretedSystem.length() )
        {
            while( interpretedSystem[i] != ')' && i < interpretedSystem.length() )
            {
                ++i;

                string coordinate = "";
                while( interpretedSystem[i] != ',' && interpretedSystem[i] != ')' && i < interpretedSystem.length() )
                {
                    if( isdigit( interpretedSystem[i] ) || interpretedSystem[i] == '.' 
                        || interpretedSystem[i] == '-' || interpretedSystem[i] == 'e' )
                    {
                        coordinate += interpretedSystem[i];
                    }
                    else
                    {
                        break;
                    }
                    ++i;
                }//done reading coordinate value
                
                bool success = false;
                double interpretedSystem = Utility::stringTo<double>( coordinate, success, printDebug );
                if( success )
                {
                    point.push_back( interpretedSystem );
                }
                else
                {
                    logger.print( "Warning: couldn't convert coordinate: " + coordinate + " to a double, for for system: ", printDebug );
                }
                
            }//done reading point
            
            //discard a trailing comma, if formatted like so (x,y,z),(x,y,z), etc.
            if( interpretedSystem[i] == ')' && i < interpretedSystem.length() - 1 && interpretedSystem[i + 1] == ',' )
            {
                /*
                if( printDebug && this->showNotify )
                {
                    interpreterOut << "Notify: skipping trailing comma at i: " << i << endl;
                    if( i > 0 )
                    {
                        interpreterOut << "Last three characters: " << interpretedSystem[i - 1] << interpretedSystem[i] << interpretedSystem[i + 1] << endl;
                    }
                }
                */
                ++i;
            }
        }
        else
        {
            string lastChars = "";
            if( i > 1 )
            {
                bool success = false;
                lastChars = Utility::toString( interpretedSystem[i - 2], success, true ) 
                                   + Utility::toString( interpretedSystem[i - 1], success, true ) 
                                   + Utility::toString( interpretedSystem[i], success, true );
            }
            logger.print(  "Warning: ! bad offset in getSystem last three characters" + lastChars, printDebug  );
        }

        if( point.size() == 3 )
        {
            points.push_back( point );
        }
    }

    if( points.size() > 0 )
    {
       branches[stack_cursor].insert( branches[stack_cursor].end(), points.begin(), points.end() ); 
    }

    return branches;

}

void LSystemGenericInterpreter::printSystem( const string& interpretedSystem )
{
    string tabs = "";
    unsigned int stack_counter = 0;
    for( unsigned int i = 0; i < interpretedSystem.length(); ++i )
    {
        if( interpretedSystem[i] == '[' )
        {
            stack_counter++;
            cout << endl << Utility::makeTabs( stack_counter );
        }
        cout << interpretedSystem[i];
        if( interpretedSystem[i] == ']' )
        {
            stack_counter--;
            cout << endl << Utility::makeTabs( stack_counter );
        }
    }
}

bool LSystemGenericInterpreter::saveAttributes( const string& filename, bool printDebug )
{
    logger.print( "Notify: entered LSystemGenericInterpreter::saveAttributes", this->doNotify && printDebug );

    ofstream fout;
    fout.open( filename.c_str() ); 

    if( !fout.good() )
    {
        logger.print(  "Error: LSystemGenericInterpreter::saveAttributes failed to create file: " + filename + " system attributes not saved ", printDebug );
        fout.close();
        return false;
    }

    for( map<string,string>::iterator i = this->systemAttributes.begin(); i != this->systemAttributes.end(); ++i )
    {
        fout << (*i).first;
        fout << " ";
        fout << (*i).second << endl; 
    }

    fout.close();
    return true;
}


bool LSystemGenericInterpreter::loadAttributes( const string& filename, bool printDebug )
{
    logger.print( "Notify: entered LSystemGenericInterpreter::loadAttributes", this->doNotify && printDebug );
    
    ifstream fin;
    fin.open( filename.c_str() );

    if( !fin.good() )
    {
        logger.print( "Error: LSystemGenericInterpreter::loadAttributes, failed to open file: " + filename, printDebug );

        fin.close();
        return false;
    }

    while( fin.peek() != EOF )
    {
        string keyword;
        string value;
        fin >> keyword;
        fin >> value;
        this->systemAttributes[keyword] = value;
    } 
    
    fin.close();

    return true;
}

bool LSystemGenericInterpreter::setAttributes( const map<string,string>& attributes, bool printDebug )
{
    logger.print( "Notify: entered LSystemGenericInterpreter::setAttributes",this->doNotify && printDebug );
    this->systemAttributes = attributes;
    return true;
}

bool LSystemGenericInterpreter::setAttribute( const string& key, const string& value, bool printDebug )
{
    logger.print( "Notify: entered LSystemGenericInterpreter::setAttribute",this->doNotify && printDebug );
	this->systemAttributes[key] = value;
    return true;
}

bool LSystemGenericInterpreter::setId( const string& id, bool printDebug )
{
    logger.print( "Notify: entered LSystemGenericInterpreter::setId", this->doNotify && printDebug );
    this->id = id;
    return true;
}

map<string,string> LSystemGenericInterpreter::getAttributes( bool printDebug )
{
    logger.print( "Notify: entered LSystemGenericInterpreter::getAttributes",this->doNotify && printDebug );
    return this->systemAttributes; 
}

string LSystemGenericInterpreter::getAttributeValue( const string& key, bool printDebug )
{
    logger.print( "Notify: entered LSystemGenericInterpreter::getAttribute",this->doNotify && printDebug );
	return this->systemAttributes[key];
}

string LSystemGenericInterpreter::getAttributeDescription( const string& attributeName, bool printDebug )
{
    logger.print( "Notify: entered LSystemGenericInterpreter::getAttributeDescription" ,this->doNotify && printDebug );
    if( attributeName == rightAngleKey )
    {
        string desc = "This attribute is one of the angles available in this lSystem. It corresponds with";
               desc += "the R symbol in the system. The notion of \"Right\" is arbitrary";
        return desc;
    }
    else if( attributeName == leftAngleKey )
    {
        string desc = "This attribute is one of the angles available in this lSystem. It corresponds with";
               desc += "the L symbol in the system. The notion of \"Left\" is arbitrary";
        return desc;
    }
    else if( attributeName == this->atomicSizeKey )
    {
        string desc = "This attribute is the radius of an atomic element this lSystem.";
        return desc;
    }
    else if( attributeName == this->startPointKey )
    {
        string desc = "The starting point of the system: where the first element is placed";
        return desc;
    }
    else if( attributeName == this->upAngleKey )
    {
         string desc = "This attribute is one of the angles available in this lSystem. It corresponds with";
               desc += "the U symbol in the system. The notion of \"Up\" is arbitrary";
        return desc;
       
    }
    else if( attributeName == this->downAngleKey )
    {
         string desc = "This attribute is one of the angles available in this lSystem. It corresponds with";
            desc += "the D symbol in the system. The notion of \"Down\" is arbitrary";
        return desc;
   
    }
    else
    {
        return attributeName + " is not supported in this interpreter";
    }
}

vector<string> LSystemGenericInterpreter::getSupportedAttributes( bool printDebug )
{
    logger.print( "Notify: entered LSystemGenericInterpreter::getSupportedAttributes" ,this->doNotify && printDebug );
    vector<string> attr;
    attr.push_back( this->rightAngleKey );
    attr.push_back( this->leftAngleKey );
    attr.push_back( this->atomicSizeKey );
    attr.push_back( this->startPointKey );
    attr.push_back( this->upAngleKey );
    attr.push_back( this->downAngleKey );
    return attr;
}

bool LSystemGenericInterpreter::isSupported( const string& attributeName, bool printDebug )
{
    logger.print( "Notify: entered LSystemGenericInterpreter::isSupported" ,this->doNotify && printDebug );
    return (attributeName == rightAngleKey || attributeName == leftAngleKey || attributeName == atomicSizeKey || attributeName == startPointKey );
}

bool LSystemGenericInterpreter::getCopy( Interpreter*& interpreter, bool printDebug )
{
    logger.print( "Notify: entered LSystemGenericInterpreter::getCopy", this->doNotify && printDebug );
    if( interpreter != NULL )
    {
        //Causes seg faults when you pass a pointer that doesn't point to anything and isn't set to NULL...
        //Is this really bad? (it does tend to encourage the user to initialize their pointers.. 
        delete interpreter;
        interpreter = NULL;
    }

    LSystemGenericInterpreter* tmp = new LSystemGenericInterpreter( this->id );
    *tmp = *this;
    interpreter = tmp;
    return true;
}

string LSystemGenericInterpreter::getId( bool printDebug )
{
    logger.print( "Notify: entered LSystemGenericInterpreter::getId" );
    return this->id;
}

#endif
