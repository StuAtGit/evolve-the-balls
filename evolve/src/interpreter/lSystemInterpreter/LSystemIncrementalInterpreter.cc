#ifndef STU_INTERPRETER_INCREMENTAL_CC
#define STU_INTERPRETER_INCREMENTAL_CC

#include "LSystemIncrementalInterpreter.h"

 LSystemIncrementalInterpreter::LSystemIncrementalInterpreter( const System* lSystem ) : LSystemGenericInterpreter( "incrementalInterpeter" )
 {
    this->system = const_cast<System*>(lSystem);
    this->startMovementVectorKey = "StartDirection";

    vector<double> startPoint;
    for( int i = 0; i < 3; ++i )
    {
        startPoint.push_back( 0 );
    }
    this->points.push_back( startPoint );
    this->turtlePointStack.push_back( pair<unsigned int, vector<double> >( 0, startPoint ) );
    //always starts moving in the negative z (arbitrary)
    //this will get re-written with key value (if present) in first grow() call after load()
    this->turtleMovementStack.push_back( Utility::makeVector( 0.0, 0.0, -1.0 ) );

    //we will need to re-calculate this if we change the start movement direction
    //in the config file (which we will probably do..)
    this->turtleVerticalAxisStack.push_back( Utility::makeVector( -1.0, 0.0, 0.0 ) );
    this->turtleHorizontalAxisStack.push_back( Utility::makeVector( 0.0, 1.0, 0.0 ) );
    this->currentSystemIndex = 0;
    this->bracketCount = 0;
    this->justTurned = false;
 }

 bool LSystemIncrementalInterpreter::getSystem( string& interpretedSystem, string& lSystem, bool printDebug )
 {
    logger.print( "Notify: In LSystemIncrementalInterpeter::getSystem( string, string )", this->doNotify );
    lSystem = this->system->getSystem();
    return this->getSystem( interpretedSystem );
 }

 bool LSystemIncrementalInterpreter::getSystem( string& interpretedSystem, System& lSystem, bool printDebug )
 {
    logger.print( "Notify: In LSystemIncrementalInterpeter::getSystem( string, System& )", this->doNotify );
    lSystem = *(this->system);
    return this->getSystem( interpretedSystem );
 }

 bool LSystemIncrementalInterpreter::getSystem( string& interpretedSystem, bool printDebug )
 {
    logger.print( "Notify: In LSystemIncrementalInterpeter::getSystem( string& )", this->doNotify );
    ostringstream ostream;
    for( unsigned int i = 0; i < this->points.size(); ++i )
    {
        if( this->points[i].size() < 3 )
        {
            logger.print( "Warning: in LSystemIncrementalInterpreter, bad point at index: " + Utility::toString(i) );
            logger.print( Utility::vectorToString( this->points[i] ) );
            continue;
        }
        ostream << "(" << (this->points[i])[0] << "," << (this->points[i])[1] << "," << (this->points[i])[2] << ")";
    }
    interpretedSystem = ostream.str();
    return true;
 }

 bool LSystemIncrementalInterpreter::getCopy( Interpreter*& interpreter, bool printDebug )
 {
    logger.print( "Notify: In LSystemIncrementalInterpeter::getCopy", this->doNotify );
    if( interpreter != NULL )
    {
        delete interpreter;
    }

    interpreter = new LSystemIncrementalInterpreter( this->system );
    LSystemIncrementalInterpreter* tmp = dynamic_cast<LSystemIncrementalInterpreter*>(interpreter); 
    *tmp = *this;
    return true;
 }
 unsigned int LSystemIncrementalInterpreter::getSize()
 {
    logger.print( "Notify: In LSystemIncrementalInterpeter::getSize()", this->doNotify );
    return this->points.size();
 }
 bool LSystemIncrementalInterpreter::grow( vector<double>& newPoint, unsigned int& index )
 {
    logger.print( "Notify: In LSystemIncrementalInterpeter::grow()", this->doNotify );

    if( this->system == NULL )
    {
        logger.print( "Warning: in LSystemIncrementalInterpreter::grow(), NULL system" );
        return false;
    }
    string lSystemString = this->system->getSystem();
    bool success = false;
    vector<string> lSystem = Utility::split<string>( lSystemString, success );
    if( !success )
    {
        this->logger.print( "Warning: failed to parse lSystemString: " + lSystemString );
        return false;
    }
    if( this->currentSystemIndex >= lSystem.size() )
    {
        this->logger.print( "Notify: in LSystemIncrementalInterpreter::grow() system index bigger than system", this->doNotify );
        return false;
    }
    //we should clear this, in order to guarantee we never lie to the user about adding a new point 
    //(Since the index argument gives no indication)
    if( newPoint.size() != 0 )
    {
        logger.print( "Warning: in LSystemIncrementalInterpreter::grow(), passed non-empty vector as newPoint argument, clearing it.." );
        newPoint.clear();
    }

    //Just to make sure..
    //Also, 0xFFFFFFFE is the index for "[" and 0xFFFFFFFF is "["
    //so we can't generate new elements for these indicies.
    //You'll very likely run out of memory first, though ;)
    if( this->points.size() >= 0xFFFFFFFD )
    {
        logger.print( "Warning: in LSystemIncrementalInterpreter::grow(), max number of points already generated" );
        return false;
    }

    //first time through, we see if someone configured the start point & initial movement vector
    if( this->currentSystemIndex == 0 && this->turtlePointStack.size() == 1 && this->turtleMovementStack.size() == 1 && this->points.size() == 1 )
    {

        if( this->systemAttributes.find( this->startPointKey ) != this->systemAttributes.end() &&
            this->systemAttributes[this->startPointKey] != "" )
        {
            vector<double> tmp = Utility::split<double>( this->systemAttributes[this->startPointKey], success );
            if( !success || tmp.size() != 3 )
            {
                logger.print( "Warning: in LSystemIncrementalInterpreter::grow() failed to parse start point attribute: " + this->systemAttributes[this->startPointKey] );
                logger.print( "Should be in the format: x,y,z" );
            }
            else
            {
                //the initial point is already allocated in the constructor
                this->points[0] = tmp;
                this->turtlePointStack[0] = pair< unsigned int, vector<double> >( 0, tmp ); 
            }
        }
        if( this->systemAttributes.find( this->startMovementVectorKey ) != this->systemAttributes.end() &&
            this->systemAttributes[this->startMovementVectorKey] != "" )
        {
            vector<double> tmp = Utility::split<double>( this->systemAttributes[this->startMovementVectorKey], success );
            if( !success || tmp.size() != 3 )
            {
                logger.print( "Warning: in LSystemIncrementalInterpreter::grow() failed to parse start direction attribute: " + this->systemAttributes[this->startMovementVectorKey] );
                logger.print( "Should be in the format: x,y,z" );
            }
            else 
            {
                this->turtleMovementStack[0] = tmp;
                //fiddle this a little to make things work for now.
                //Otherwise have the user override what they consider the vertical rotate axis (and horizontal).
                //now adjust our cross product stack
                //soo... basically we just randomly choose an axis that lays in the disk of all possible vectors
                //that are at right angles the given movement axis..... Look over there! a pink bunny! 
                //pay no attention to this code.... *ducks away*
                vector<double> jiggle = tmp;
                jiggle[0] += 0.7;
                this->turtleVerticalAxisStack[0] = Matrix::vectorCrossProduct( tmp, jiggle );
                this->turtleHorizontalAxisStack[0] = Matrix::vectorCrossProduct( tmp, this->turtleVerticalAxisStack[0] );
            }
        }
    }
   
    string currentElement = lSystem[this->currentSystemIndex++];
    if( this->turtlePointStack.size() == 0 )
    {
        logger.print( "Error: in LSystemIncrementalInterpreter::grow(), point stack empty" );
        return false;
    }
    if( this->turtleMovementStack.size() == 0 )
    {
        logger.print( "Error: in LSystemIncrementalInterpreter::grow(), turtle movement stack empty" );
        return false;
    }
    if( this->turtleVerticalAxisStack.size() == 0 )
    {
        logger.print( "Error: in LSystemIncrementalInterpreter::grow(), vertical axis stack empty" );
        return false;
    }
    if( this->turtleHorizontalAxisStack.size() == 0 )
    {
        logger.print( "Error: in LSystemIncrementalInterpreter::grow(), horizontal axis stack empty" );
        return false;
    }

    unsigned int topOfPointStack = this->turtlePointStack.size() - 1;
    unsigned int topOfMovementStack = this->turtleMovementStack.size() - 1;
    unsigned int topOfVerticalAxisStack = this->turtleVerticalAxisStack.size() - 1;
    unsigned int topOfHorizontalAxisStack = this->turtleHorizontalAxisStack.size() - 1;

    if( this->turtlePointStack[topOfPointStack].second.size() < 3 )
    {
        logger.print( "Error: turtle point on top of point stack is bad" );
        logger.print( "Point: " + Utility::vectorToString( this->turtlePointStack[topOfPointStack].second, success ) );
        return false;
    }
    if( this->turtleMovementStack[topOfMovementStack].size() < 3 )
    {
        logger.print( "Error: turtle movement vector on top of movement stack is bad" );
        logger.print( Utility::vectorToString( this->turtleMovementStack[topOfMovementStack], success ) );
        return false;
    }

    double movementStep = 0;
    if( this->elementSize.find( currentElement ) == this->elementSize.end() )
    {
        movementStep = Utility::stringTo<double>( this->systemAttributes[this->atomicSizeKey], success );
        if( !success )
        {
            logger.print( "Warning: failed to covert atomic size from attribute: " + this->systemAttributes[this->atomicSizeKey] + " and no size set for element: " + currentElement );
            return false;
        }
    }
    else
    {
        movementStep = this->elementSize[currentElement];
    }

    //we did our job :)
    //also, so we can safely take the substr below.
    //so we can have suffix modifiers to elements that don't affect the interpretation.
    //e.g. U(universal) could mean "up" with a universal joint. Up to the _client_ to interpret
    //these modifiers, so we need to make sure we still map indicies onto the _entire_ current element.
    if( currentElement == "" )
    {
        return false;
    }
    //check substrings to ignore any modifier suffix.
    //(don't index, because code may change to have elements that are more than one character long.
    else if( currentElement.substr( 0, 1 ) == "S" )
    {
        //if we've just turned, don't step forward, just turn.
        //Not quite right. Pythagoras anyone?
        if( this->justTurned )
        {
            //movementStep *= -1;
        }

        for( unsigned int i = 0; i < 3; ++i )
        {
            newPoint.push_back( this->turtlePointStack[topOfPointStack].second[i] + 
                                (this->turtleMovementStack[topOfMovementStack][i] * movementStep )
                              );
        }
        this->justTurned = false;
        this->points.push_back( newPoint );
        index = this->points.size() - 1;
        this->turtlePointStack[topOfPointStack] = pair<unsigned int, vector<double> >( index, newPoint ); 
        this->elements[index] = currentElement;
    }
    else if( currentElement.substr( 0, 1 ) == "L" || currentElement.substr( 0, 1 ) == "R" )
    {
        string angleString = "";
        if( currentElement.substr( 0, 1 ) == "L" )
        {
            angleString = this->systemAttributes[this->leftAngleKey];
        }
        else
        {
            angleString = this->systemAttributes[this->rightAngleKey];
        }

        double angle = Utility::stringTo<double>( angleString, success );
        if( !success )
        {
            logger.print( "Warning: in LSystemIncrementalInterpreter::grow(), bad L/R angle string: " + angleString + " current element is: " + currentElement + " current index is: " + Utility::toString( this->currentSystemIndex )  );
            return false;
        }
        bool isParallel = true;
        this->turtleMovementStack[topOfMovementStack] = Matrix::vector3Rotate( this->turtleMovementStack[topOfMovementStack], 
                                                                               this->turtleHorizontalAxisStack[topOfHorizontalAxisStack], 
                                                                               angle, isParallel );
        this->justTurned = true;                                                                               
        if( isParallel )
        {
            logger.print( "Warning: in LSystemIncrementalInterpreter::grow(), tried to rotate with element: " + currentElement + "  but failed because movement was parallel to the local horizontal axis..." );
        }
        else
        {
            vector<double> tmp = Matrix::vectorCrossProduct( this->turtleMovementStack[topOfMovementStack], 
                                                             this->turtleHorizontalAxisStack[topOfHorizontalAxisStack] );
            this->turtleVerticalAxisStack[topOfVerticalAxisStack] = tmp;
        }
        if( !success )
        {
            logger.print( "Warning: in LSystemIncrementalInterpreter::grow(), bad atomic size string: " + this->systemAttributes[this->atomicSizeKey] );
            return false;
        }

        for( unsigned int i = 0; i < 3; ++i )
        {
            newPoint.push_back( this->turtlePointStack[topOfPointStack].second[i] + 
                                (this->turtleMovementStack[topOfMovementStack][i] * movementStep )
                              );
        }
        this->points.push_back( newPoint );
        index = this->points.size() - 1;
        this->turtlePointStack[topOfPointStack] = pair<unsigned int, vector<double> >( index, newPoint );
        this->elements[index] = currentElement;
    }
    else if( currentElement.substr( 0, 1 ) == "U" || currentElement.substr( 0, 1 ) == "D" )
    {
        string angleString = "";
        if( currentElement.substr( 0, 1 ) == "U" )
        {
            angleString = this->systemAttributes[this->upAngleKey];
        }
        else
        {
            angleString = this->systemAttributes[this->downAngleKey];
        }

        double angle = Utility::stringTo<double>( angleString, success );
        if( !success )
        {
            logger.print( "Warning: in LSystemIncrementalInterpreter::grow(), bad U/D angle string: " + angleString + " current element is: " + currentElement );
            return false;
        }
        bool isParallel = true;
        this->turtleMovementStack[topOfMovementStack] = Matrix::vector3Rotate( this->turtleMovementStack[topOfMovementStack], 
                                                                               this->turtleVerticalAxisStack[topOfVerticalAxisStack],
                                                                               angle, isParallel );
        this->justTurned = true;
        if( isParallel )
        {
            logger.print( "Warning: in LSystemIncrementalInterpreter::grow(), currentElement: " + currentElement + " tried to cause a rotation around a parallel axis" );
        }
        else
        {
            vector<double> newHorizontalAxis = Matrix::vectorCrossProduct( this->turtleMovementStack[topOfMovementStack],
                                                                           this->turtleVerticalAxisStack[topOfVerticalAxisStack] );
            this->turtleHorizontalAxisStack[topOfHorizontalAxisStack] = newHorizontalAxis; 
        }

        for( unsigned int i = 0; i < 3; ++i )
        {
            newPoint.push_back( this->turtlePointStack[topOfPointStack].second[i] + 
                                (this->turtleMovementStack[topOfMovementStack][i] * movementStep )
                              );
        }
        this->points.push_back( newPoint );
        index = this->points.size() - 1;
        this->turtlePointStack[topOfPointStack] = pair< unsigned int, vector<double> >( index, newPoint ); 
        this->elements[index] = currentElement;
    }
    else if( currentElement == "[" )
    {
        this->bracketCount++;
        this->turtlePointStack.push_back( this->turtlePointStack[topOfPointStack] );
        this->turtleVerticalAxisStack.push_back( this->turtleVerticalAxisStack[topOfVerticalAxisStack] );
        this->turtleHorizontalAxisStack.push_back( this->turtleHorizontalAxisStack[topOfHorizontalAxisStack] );
        this->turtleMovementStack.push_back( this->turtleMovementStack[topOfMovementStack] );
        this->justTurned = false;
        index = 0xFFFFFFFE; //unique values that always will return "[" (because they we can't map this to a point - branch markers don't generate one)
    }
    else if( currentElement == "]" )
    {
        //this happens when a system is badly formed, or is mutated
        //into having unbalanced nesting.
        //We need to ignore this in ignore to prevent emptying our stacks too much
        //and crashing :(
        if( this->bracketCount == 0 )
        {
            return false;
        }
        //pop off to restore old values
        this->bracketCount--;
        this->turtlePointStack.pop_back();
        this->turtleMovementStack.pop_back();
        this->turtleVerticalAxisStack.pop_back();
        this->turtleHorizontalAxisStack.pop_back();
        this->justTurned = false;
        index = 0xFFFFFFFF; //see "[" note
    }
    else
    {
        this->logger.print( "Notify: unknown element: " + currentElement + " in LSystemIncremetal::grow()", this->doNotify );
        return false;
    }

    return true;
 }

 bool LSystemIncrementalInterpreter::prev( vector<double>& prev, unsigned int& index )
 {
    logger.print( "Notify: In LSystemIncrementalInterpeter::prev()", this->doNotify );

    if( prev.size() != 0 )
    {
        logger.print( "Warning: in LSystemIncrementalInterpreter::prev, non-empty out argument passed, clearing..", this->doNotify );
        prev.clear();
    }
    if( this->turtlePointStack.size() == 0 )
    {
        logger.print( "Error: in LSystemIncrementalInterpeter::prev bad point stack state" );
        return false;
    }
    prev = this->turtlePointStack[this->turtlePointStack.size() - 1].second;
    index = this->turtlePointStack[this->turtlePointStack.size() - 1].first;
    return true;
 }

 bool LSystemIncrementalInterpreter::getElement( string& element, unsigned int index )
 {
    logger.print ("Notify: In LSystemIncrementalInterpreter::getElement()", this->doNotify );

    if( index == 0xFFFFFFFE )
    {
        element = "[";
        return true;
    }
    else if( index == 0xFFFFFFFF )
    {
        element = "]";
        return true;
    }

    if( this->elements.find( index ) == this->elements.end() )
    {
        logger.print( "Warning: In LSystemIncrementalInterpreter::getElement(), passed invalid index" );
        return false;
    }
    element = this->elements[index];
    return true;
 }

 bool LSystemIncrementalInterpreter::setElementSize( const string& element, double size )
 {
    logger.print( "Notify: In LSystemIncrementalInterpreter::setElementSize()", this->doNotify );
    this->elementSize[element] = size;
    return true;
 }

 bool LSystemIncrementalInterpreter::updatePoint( unsigned int index, const vector<double>& point )
 {
    logger.print( "Notify: In LSystemIncrementalInterpeter::updatePoint()", this->doNotify );

    if( index >= this->points.size() )
    {
        logger.print( "Warning: in LSystemIncrementalInterpreter::setPoint, invalid index: " + Utility::toString( index ) );
        logger.print( "Warning: did you pass an index that matched a ] or [ element?" );
        return false;
    }
    this->points[index] = point;

    //make sure we update any reference to the point in the point stack (for prev() calls)
    //Note that crossProduct and movement stack store vectors, and therefore aren't invalidated by moving
    //the point (because vectors math works like that, too tired too explain right now)
    for( unsigned int i = 0; i < this->turtlePointStack.size(); ++i )
    {
        if( this->turtlePointStack[i].first == index )
        {
            this->turtlePointStack[i].second = point;
        }
    }
    return true;
 }

 vector< vector<double> > LSystemIncrementalInterpreter::getPoints()
 {
    logger.print( "Notify: In LSystemIncrementalInterpeter::getPoints()", this->doNotify );
    return this->points;
 }

 vector<double> LSystemIncrementalInterpreter::getPoint( unsigned int index )
 {
    logger.print( "Notify: In LSystemIncrementalInterpeter::getPoint()", this->doNotify );
    if( index >= this->points.size() )
    {
        logger.print( "Warning: in LSystemIncrementalInterpeter::getPoint, index: " + Utility::toString( index ) + " too big" );
        logger.print( "Warning: did you pass an index that matched a ] or [ element?" );
        vector<double> ret;
        return ret;
    }
    return this->points[index];
 }
 bool LSystemIncrementalInterpreter::getCurrentDirection( vector<double>& direction )
 {
    if( this->turtleMovementStack.size() == 0 )
    {
        logger.print( "Warning: in LSystemIncrementalInterpreter::getCurrentDriection, movement stack is empty..." );
        return false;
    }
    //this is wrong because, before we actually "move" (or generate the next point), we
    //actually alter the movement vector based on the current element we're looking at!
    //so we need to fix this...
    direction = this->turtleMovementStack[this->turtleMovementStack.size() - 1];
    return true;
 }

 void LSystemIncrementalInterpreter::printSystem()
 {
    logger.print( "Notify: In LSystemIncrementalInterpeter::printSystem()", this->doNotify );
    cout << "blah" << endl;
 }


#endif
