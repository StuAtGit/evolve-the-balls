#ifndef STU_L_SYSTEM_GENERIC_CC
#define STU_L_SYSTEM_GENERIC_CC

#include "LSystemGeneric.h"
#include "helperFuncs.h"

//constructors/destructors
LSystemGeneric::LSystemGeneric()
{
     currentGene = 0;
     minSize = 0;
     maxSize = 0;
     //state of LSystem with respect to stepping: whether the grow() method
     //expects a response to a query the next time it is called
     expectingQueryResponse = false;
     //this could overflow?
     numSteps = 0;
    //stats collected by system
    //case 1
     numSwaps = 0;
     swappedLeft = 0;
     swappedRight = 0;
    //case 2
     numToggled = 0;
    //case 3
     numDeleted = 0;
    //case 4
     numDoubled = 0;
     isInitialized = false;
     doNotify = false;
    //Names of config file sections
    //attributes of the system
     max_size_system="maxSize";
     min_size_system="minSize";
     mutation_options="options";
     seed_system="system";
     growth_rules="growthRules";
     mutation_rules="mutationRules";

    //stats of the system
     num_steps="numSteps";
     num_swaps="numSwaps";
     swapped_left="swappedLeft";
     swapped_right="swappedRight";
     num_toggled="numToggled";
     num_deleted="numDeleted";
     num_doubled="numDoubled";
     current_gene="currentGene";

    //comment mark (any line beginning with this mark is ignored)
     comment_mark="#";
}

//when adding new keywords, be sure to add them to the method above
bool LSystemGeneric::initialize( string file, bool debug )
{
    logger.print( "Notify: entered initialze via filename", debug && this->doNotify );

    ifstream fin;

    try
    {
        fin.open( file.c_str(), ios::in | ios::binary );
    }
    catch( ... )
    {
        return false;
    }
    
    if( !fin.good() )
    {
        if( debug )
        {
            ostringstream ostream;
            ostream <<  "Error: intialize( filename, debug ) can't open file: " << file;
    	    logger.print( ostream.str() );
	    }
        return false;
    }

    return this->initialize( fin, debug );
}    

bool LSystemGeneric::initialize( ifstream& fin, bool printDebug )
{

    logger.print( "Notify: entering initialize via ifstream", printDebug && this->doNotify );
   
    if( !fin.good() )
    {
        if( printDebug )
        {
            logger.print( "Warning: bad input stream" );
        }
        return false;
    }
   
    fin.seekg( 0, ios::end );
    int length = fin.tellg();
    fin.seekg( 0, ios::beg );
    //we're changing this, and hope this wasn't done on purpose..
    //char* buffer = new char[length];
    char* buffer = new char[length + 1];
    //why is this one less?? this should 
    //fin.read( buffer, length - 1 );
    fin.read( buffer, length );
    buffer[length] = '\0';

    string fileContents = buffer;
    delete [] buffer;
    bool success = this->initializeFromFileContents( fileContents, printDebug );
    fin.close();

    this->isInitialized = success;
    return success;
}

bool LSystemGeneric::checkWord( string word, bool printDebug )
{
    logger.print( "Notify: checkWord() ", printDebug && this->doNotify );

    return word != seed_system && 
           word != mutation_rules && 
           word != mutation_options && 
           word != max_size_system && 
           word != min_size_system &&
           word != num_steps &&
           word != num_swaps &&
           word != swapped_left &&
           word != swapped_right &&
           word != num_toggled &&
           word != num_deleted &&
           word != num_doubled &&
           word != growth_rules &&
           word != current_gene;
}

bool LSystemGeneric::initialize( map< string, string >& input, bool debug )
{
    logger.print( "Notify: entered initialize map< string, string>", debug && this->doNotify );
       
    for( unsigned int i = 0; i < input[mutation_options].length(); ++i )
    {
        string option;
        while( i < input[mutation_options].length() && input[mutation_options][i] != ',' )
        {
            option += input[mutation_options][i];
            ++i;
        }
        this->options.push_back( option );
    }

    this->system.clear();
        
    for( unsigned int i = 0; i < input[seed_system].length(); ++i )
    {
        string word;
        while( i < input[seed_system].length() && input[seed_system][i] != ',' )
        {
            word += input[seed_system][i];
            ++i;
        }
        this->system.push_back( word );
    }
    
        
    for( vector<string>::iterator i = this->options.begin(); i != this->options.end(); ++i)
    {
       toggledStats[ *i ] = 0; 
    }

    logger.print( "Notify: reading in growth rules", debug && this->doNotify );

    for( unsigned int i = 0; i < input[growth_rules].length(); ++i )
    {
        string rule;
        while( i < input[growth_rules].length() && input[growth_rules][i] != '.' )
        {
            rule += input[growth_rules][i];
            ++i;
        }
        
        unsigned long colonIndex = rule.find( ':' );
        if( colonIndex != string::npos )
        {
            if( debug && this->doNotify )
            {
                ostringstream ostream;
                ostream << "Notify: colonIndex for rule: " << rule << " is: " << colonIndex;
                logger.print( ostream.str() );
            }
            string startSymbol = rule.substr( 0, colonIndex );
            string endSymbol = "";
            //A start symbol with no substiution rule!
            if( colonIndex + 1 >= rule.length() )
            {
                if( debug )
                {
                    logger.print( "Warning: got a start symbol with not substitution rule" );
                }
            }
            else
            {
                endSymbol = rule.substr( colonIndex + 1, rule.length() );
            }

            if( debug && this->doNotify )
            {
                ostringstream ostream;
                ostream << "Notify: inserting start symbol: " << startSymbol << " and endSymbol: " << endSymbol;
                logger.print( ostream.str() );
            }
            growthRules[startSymbol] = endSymbol;
        }
    }

    bool success = false;
    this->mutationProbabilities = Utility::parseMap<string,int>( input[mutation_rules], success, debug );
    this->minSize = Utility::stringTo<int>( input[min_size_system], success, debug );
    this->maxSize = Utility::stringTo<int>( input[max_size_system], success, debug );
    this->numSteps = Utility::stringTo<long long>( input[num_steps], success, debug );
    this->numSwaps = Utility::stringTo<long long>( input[num_swaps], success, debug );
    this->swappedLeft = Utility::stringTo<long long>( input[swapped_left], success, debug );
    this->swappedRight = Utility::stringTo<long long>( input[swapped_right], success, debug );
    this->numToggled = Utility::stringTo<long long>( input[num_toggled], success, debug );
    this->numDeleted = Utility::stringTo<long long>( input[num_deleted], success, debug );
    this->numDoubled = Utility::stringTo<long long>( input[num_doubled], success, debug );
    this->currentGene = Utility::stringTo<long long>( input[current_gene], success, debug );

    logger.print( "Notify: exiting initialize map< string, string > ", debug && this->doNotify );

    this->isInitialized = true;
    return true;
}

string stripComments( string fileContents )
{
    string stripped = "";
    for( unsigned int i = 0; i < fileContents.size(); ++i )
    {
        if( fileContents[i] == '#' )
        {
            while( i < fileContents.size() && fileContents[i] != '\n' )
            {
                ++i;            
            }
        }
        else
        {
            stripped += fileContents[i];
        }
    }
    return stripped;
}

bool LSystemGeneric::initializeFromFileContents( string fileContents, bool printDebug )
{
    logger.print( "Notify: entered initializeFromFileContents", this->doNotify && printDebug );

    map<string,string> keyValuePairs;
    string fileStripped = stripComments( fileContents );
    char_separator<char> sep( " \n\t");
    tokenizer< char_separator<char> > tokens(fileStripped,sep);
    tokenizer< char_separator<char> >::iterator i = tokens.begin();
    while( i != tokens.end() )
    {
        string currentSection = *i;
        if( checkWord( currentSection, printDebug ) )
        {
            logger.print( "Warning: in LSystemGeneric, expected a section name, but got: " + currentSection + " instead", printDebug );
            ++i;
            continue;
        }
       
        string currentValue = "";
        while( ++i != tokens.end() && checkWord( *i, printDebug ) )
        {
            currentValue += *i;
        }

        keyValuePairs[currentSection] = currentValue;
    }

    return initialize( keyValuePairs, printDebug );
}

//saves the system to a simple key space value format
bool LSystemGeneric::save( string filename, bool printDebug)
{
    logger.print( "Notify: entered saveSystem() ", printDebug && this->doNotify );

    ofstream fout;
    fout.open( filename.c_str(), std::ios_base::out );

    if( !fout.good() )
    {
        logger.print( "Error, bad file name or unable to open file"  );
        return false;
    }

    fout << this->max_size_system << " " << this->maxSize << endl;
    fout << min_size_system << " " << this->minSize << endl;
    fout << mutation_options << " " << getOptionsString( printDebug ) << endl;
    fout << seed_system << " ";
    for( unsigned int i = 0; i < this->system.size(); ++i )
    {
        if( i != 0 )
            fout << "," + this->system[i];
        else
            fout << this->system[i];
    }
    fout  << endl;
    
    fout << mutation_rules << " ";
    for( map<string,int>::iterator j = this->mutationProbabilities.begin(); j != this->mutationProbabilities.end(); ++j )
    {
        fout << "(" << (*j).first << "," << (*j).second << ")";
    }
    
    fout  << endl;
    fout << growth_rules << " ";
    for( map<string,string>::iterator i = this->growthRules.begin(); i != this->growthRules.end(); ++i )
    {
        if( i != this->growthRules.begin() )
        {
            fout << "." << (*i).first << ":" << (*i).second;
        }
        else
        {
            fout << (*i).first << ":" << (*i).second;
        }
    }
    fout << endl;
    fout << num_steps << " " << this->numSteps << endl;
    fout << num_swaps << " " << this->numSwaps << endl;
    fout << swapped_left << " " << this->swappedLeft << endl;
    fout << swapped_right << " " << this->swappedRight << endl;
    fout << num_toggled << " " << this->numToggled << endl;
    fout << num_deleted << " " << this->numDeleted << endl;
    fout << num_doubled << " " << this->numDoubled << endl;
    fout << current_gene << " " << this->currentGene << endl;
    fout.close();
 
    return true;
}

bool LSystemGeneric::run( const string& mode, string& query, bool printDebug )
{

    logger.print( "Notify: in LSystemGeneric::run", printDebug && this->doNotify );

    if( mode == "mutate" )
    {
        return mutate( printDebug );
    }
    else if( mode == "grow" )
    {
        return grow( query, printDebug );
    }

    logger.print( "Warning: in LSystemGeneric::run, unrecognized mode: " + mode + " given", printDebug );
    return false;
}

bool LSystemGeneric::checkQueryResponse( const string query, bool printDebug )
{
    return true;
}

bool LSystemGeneric::grow( string& query, bool printDebug )
{

    if( printDebug && this->doNotify )
    {
        ostringstream ostream;
        ostream << "Notify: in grow() system initially: " << this->getSystem();
        logger.print( ostream.str() );
    }

    if( this->system.size() >= this->maxSize )
    {
        if( printDebug && this->doNotify )
        {
            logger.print( "Notify: in grow(), system already at max size" );
            return true;
        }
    }
    if( expectingQueryResponse && !checkQueryResponse( query, printDebug ) )
    {
        if( printDebug )
        {
            logger.print( "Warning: grow() currently expecting a query response but no correct response recieved" );
            ostringstream ostream;
            ostream << "Warning: query recieved was: " << query;
            logger.print( ostream.str() );
        }
        //here we attempt to re-start and re-issue query if possible, else, just return -1 (error)
        return false;
    }

    this->expectingQueryResponse = false;
    //make sure the query is blanked after handling it
    query = "";
    
    for( unsigned int i = 0; i < this->system.size() && i < this->maxSize && this->system.size() < this->maxSize; ++i )
    {
        if( printDebug && this->doNotify )
        {
            ostringstream ostream;
            ostream << "Notify: first for loop: i : " << i << " size: " << this->system.size();
            logger.print( ostream.str() );
        }
        
        for( map<string,string>::iterator iter = growthRules.begin(); iter != growthRules.end() && this->system.size() < this->maxSize; ++iter )
        {
            if( printDebug && this->doNotify )
            {
                ostringstream ostream;
                ostream << "Notify: second for loop at rule for: " << (*iter).first;
                logger.print( ostream.str() );
            }

            if( (*iter).first == system[i] )
            {
                if( printDebug && this->doNotify )
                {
                    ostringstream ostream;
                    ostream << "Notify: grow() found match for replacement: " << system[i] << " with " << (*iter).second;
                    logger.print( ostream.str() );
                }
                string newRules = (*iter).second;
                vector<string> tokens;
                //tokenize the replacement
                for( unsigned int j = 0; j < newRules.length(); ++j )
                {
                    string newToken = "";
                    for( ; j < newRules.length() && newRules[j] != ','; ++j )
                    {
                       newToken += newRules[j]; 
                    }
                    tokens.push_back( newToken );
                }

                logger.print( "Notify: grow() tokenized replacement", printDebug && this->doNotify);
              
                int numInserted = 0;
                for( int l = tokens.size() - 2; l >= 0; --l )
                {
                    vector<string>::iterator systemIter = system.begin();
                    systemIter += i; 
                    if( system.size() < this->maxSize )
                    {
                        if( printDebug && this->doNotify )
                        {
                            ostringstream ostream;
                            ostream << "Notify: grow() inserting new element: " << tokens[l];
                            logger.print( ostream.str() );
                        }
 
                        system.insert( systemIter, tokens[l] );
                        numInserted++;
                    }
                    else
                    {
                        break;
                    }
                }

                if( printDebug && this->doNotify )
                {
                    logger.print( "Notify: grow() inserted new elements" );
                    ostringstream ostream;
                    ostream << "Notify: jumping i by: " << numInserted;
                    logger.print( ostream.str() );
                }
                //get the index back pointing at the same element
                i += numInserted;
                if( printDebug && this->doNotify )
                {
                    ostringstream ostream;
                    ostream << "Notify: jumping i to: " << i << "\n";
                    ostream << "Notify: system size is: " << system.size();
                    logger.print( ostream.str() );
                }

                //now just replace the original element with the last element in the end production
                if( numInserted >= 0 )
                {
                    system[i] = tokens[numInserted];
                }
                break;
            }
        }
    }

    if( printDebug && this->doNotify )
    {
        ostringstream ostream;
        ostream << "Notify: in grow(), after replacement, system now: " << this->getSystem();
        logger.print( ostream.str() );
    }
    return true;
}

bool LSystemGeneric::mutate( bool printDebug )
{
    logger.print( "Notify: entered lsystem step()", printDebug && this->doNotify );

    if( !this->isInitialized )
    {
        if( printDebug )
            logger.print( "Warning: attempt to run an lsystem that is not initialized"  );
        return false;
    }

    unsigned int j = 0;
    numSteps++;
    if( this->mutationProbabilities.empty() )
    {
        logger.print( "Warning, no mutation types in mutationProbability table", printDebug  );
        
        return false;
    }
    if( this->currentGene >= this->system.size() )
    {
        this->currentGene = 0;
    }

    j = this->currentGene;
    string type;
    int probability = 0;
    vector<string> keys = Utility::getMapKeys( this->mutationProbabilities );
    getRandomKey( type, probability, keys, printDebug );
    char type_tmp = type[0];
    timeval t;
    gettimeofday( &t, NULL ); 
	unsigned int seed = t.tv_usec;
    int threshold = ( 1 + (rand_r( &seed ) % 10000) );
    int neighbor;
    int index;
    string entry;
    vector<string>::iterator iterErase = this->system.begin();

    if( printDebug && this->doNotify )
    {
        ostringstream ostream;
        ostream << "Notify: step(), type: " << type << "\n";
        ostream << "Notify: step(), probability: " << probability << " mutation: " << mutationProbabilities[type] << endl;
        ostream << "Notify: step(), threshold: " << threshold;
        logger.print( ostream.str() );
    }
        //rand has even distribution (I think), so if our probability 
        //was specified as 5000, we should have a 50% chance of exceeding
        //threshold given (or is it 49.99?). No worries.
        if( probability > threshold )
        {
            switch( type_tmp )
            {
                case '1':
                    //obviously, can't do swaps here ;)
                    if( this->system.size() <= 1 )
                        break;
                    this->numSwaps++;
                    if( printDebug && this->doNotify )
                    {
                        ostringstream ostream;
                        ostream << "Notify: step(), case 1, j is: " << j;
                        logger.print( ostream.str() );
                    }
                    if( j == 0 )
                    {
                        this->swappedRight++;
                        //swap with ++j
                        if( printDebug && this->doNotify )
                        {
                            ostringstream ostream;
                            ostream << "Notify: step(), case 1, beginning swap right (bounding case)";
                            logger.print( ostream.str() );
                        }
                        string temp = this->system[j];
                        this->system[j] = this->system.at(j + 1);
                        this->system[j + 1] = temp;
                        break;
                    }
                    
                    if( j == this->system.size() - 1 )
                    {
                        //swap with --j
                        logger.print( "Notify: step(), case 1, beginning swap left (bounding case)", printDebug && this->doNotify );
                        this->swappedLeft++;
                        string temp = this->system[j];
                        this->system[j] = this->system[j - 1];
                        this->system[j - 1] = temp; 
                        break;
                    }

                    neighbor = 1 + (rand_r( &seed ) % 2);
                   
                    if( neighbor == 1 )
                    {
                        //swap with ++j
                        logger.print( "Notify: step(), case 1, beginning swap right", printDebug && this->doNotify );

                        this->swappedRight++;
                        string temp = this->system[j];
                        this->system[j] = this->system.at(j + 1);
                        this->system[j + 1] = temp; 
                        break;
                    }

                    if( neighbor == 2 )
                    {
                        //swap with --j
                        logger.print( "Notify: step(), case 1, beginning swap left", printDebug && this->doNotify );
                        this->swappedLeft++;
                        string temp = this->system[j];
                        this->system[j] = this->system[j - 1];
                        this->system[j - 1] = temp; 
                        break;
                    }
                break;
                case '2':
                    if( this->system.size() < 1 )
                        break;
                    logger.print( "Notify: step(), case 2", printDebug && this->doNotify );
                    this->numToggled++;
                    index = rand_r( &seed ) % this->options.size();
                    this->toggledStats[ this->options[index] ]++;
                    this->system[j] = this->options[index];
                break;
                case '3':
                    if( this->system.size() < 1 )
                        break;
                    if( printDebug && this->doNotify )
                    {
                        ostringstream ostream;
                        ostream << "Notify: step(), case 3, j is: " << j;
                        logger.print( ostream.str() );
                    }
                    if( this->system.size() <= this->minSize )
                       break;
                    this->numDeleted++;
                    iterErase += j;
                    this->system.erase( iterErase );
                break;
                case '4':
                    if( this->system.size() < 1 )
                        break;
                    if( printDebug && this->doNotify )
                    {
                        ostringstream ostream;
                        ostream << "Notify: step(), case 4, j is: " << j;
                        logger.print( ostream.str() );
                    }
                    if( this->maxSize >= 0 && this->system.size() >= this->maxSize )
                        break;
                    this->numDoubled++;
                    vector<string>::iterator iter = this->system.begin();
                    iter += j;
                    string doubled = this->system[j];
                    this->system.insert( iter, doubled );
                break;
            }
        }
        
        ++this->currentGene;
        logger.print( "Notify: step(), returning", printDebug && this->doNotify );

    return true;
}

//gets the current L-system state (as comma-delimited string)
const string LSystemGeneric::getSystem( bool printDebug )
{
    logger.print( "Notify: In LSystemGeneric getSystem", printDebug && this->doNotify );

    string systemString = "";
    for( unsigned int i = 0; i < this->system.size(); ++i )
    {
        if( i != 0 )
            systemString += "," + this->system[i];
        else
            systemString += this->system[i];
    }
    return systemString;
}

//gets the system as vector
vector<string> LSystemGeneric::getSystemVector( bool printDebug )
{
    logger.print( "Notify: entered getSystemVector", printDebug && this->doNotify );
    return this->system;
}

//get the minimum size of the system
int LSystemGeneric::getMinSize(bool printDebug)
{
    logger.print( "Notify: entered getMinSize() ", printDebug && this->doNotify );
    return this->minSize;
}
//gets the maximum size this string is allowed to reach
int LSystemGeneric::getMaxSize(bool printDebug)
{
    logger.print( "Notify: entered getMaxSize() ", printDebug && this->doNotify );
    return this->maxSize;
}
//gets the options available for toggling
vector<string> LSystemGeneric::getOptions(bool printDebug)
{
    logger.print( "Notify: entered getOptions() ", printDebug && this->doNotify );
    return this->options;
}   
//gets the integer defining the mutationProbabilitieset for this system
map<string,int> LSystemGeneric::getMutationProbabilities(bool printDebug)
{
    logger.print( "Notify: entered getMutationProbabilities() ", printDebug && this->doNotify );
    return this->mutationProbabilities;
}
//gets whether or not this system has been initialized
bool LSystemGeneric::getIsInitialized(bool printDebug)
{
    logger.print( "Notify: entered getIsIntialized() ", printDebug && this->doNotify );
    return this->isInitialized;
}

string LSystemGeneric::getGrowthRules( bool printDebug )
{
    logger.print( "Notify: entered getGrowthRules", printDebug && this->doNotify );

    string growthRulesString = "";
    for( map<string,string>::iterator iter = this->growthRules.begin(); iter != this->growthRules.end(); ++iter )
    {
        if( iter == this->growthRules.begin() )
        {
            growthRulesString += (*iter).first + "." + (*iter).second ;
        }
        else
        {
            growthRulesString += ":" + (*iter).first + "." + (*iter).second ;
        }
    }

    return growthRulesString;
}

map<string,string> LSystemGeneric::getGrowthRuleMap( bool printDebug )
{
    logger.print( "Notify: entered getGrowthRuleMap", printDebug && this->doNotify );
    return this->growthRules;
}

string LSystemGeneric::getMutationTypeDescription( string type, bool printDebug  )
{
    logger.print( "Notify: entered getMutationTypeDescription", printDebug && this->doNotify );
    char type_tmp = type[0]; 
    switch( type_tmp )
    {
        case '1':
            return "The current word is swapped with one of it's neighbors. Unless at a end of the system, it has an equal chance of getting swapped left or right";
        case '2':
            return "The current word is toggled to one of the possible options available. Each option has equal probability";
        case '3':
            return "The current word is deleted";
        case '4':
            return "The current word is doubled";
        default:
            return "Undefined mutation type for this lSystem";
    }
}

const string LSystemGeneric::getId( bool printDebug )
{
    logger.print( "Notify: entered LSystemGeneric::getId", printDebug && this->doNotify );
    return this->id;
}

const bool LSystemGeneric::getCopy( System*& copy, bool printDebug )
{
    logger.print( "Notify: entered method LSystemGeneric::getCopy ", this->doNotify && printDebug );

    if( copy != NULL )
    {
        delete copy;
    }
    LSystemGeneric* tmp = new LSystemGeneric();
    *tmp = *this;
    copy = tmp;
    return true;
}

bool LSystemGeneric::setId( string id, bool printDebug )
{
    logger.print( "Notify: entered LSystemGeneric::setId", printDebug && this->doNotify );
    this->id = id;
    return true;
}

//set the minimum size the string can reach
bool LSystemGeneric::setMinSize( int minSize, bool printDebug)
{
    logger.print( "Notify: entered setMinSize() ", printDebug && this->doNotify );

    if( minSize < 0 )
        minSize = 0;
    this->minSize = minSize;
    return true;
}

//set the maximum size the string can reach
//-1 means it will not check for max size
bool LSystemGeneric::setMaxSize( int maxSize, bool printDebug)
{
    logger.print( "Notify: entered setMaxSize() ", printDebug && this->doNotify );
    this->maxSize = maxSize;
    return true;
}

//set the system 
bool LSystemGeneric::setSystem( vector<string> system, bool printDebug)
{
    logger.print( "Notify: entered setSystem() ", printDebug && this->doNotify );
    this->system = system;
    return true;
}

//adds/modifies a growth rule with start symbol start
bool LSystemGeneric::setGrowthRule( string start, string end, bool printDebug )
{
    if( printDebug && this->doNotify )
    {
        ostringstream ostream;
        ostream << "Notify: entered setGrowthRule, key(start): " << start << " value(production): " << end;
        logger.print( ostream.str() );
    }
    growthRules[start] = end;

    return true;
}

//deletes a growth rule with matching start symbol
bool LSystemGeneric::deleteGrowthRule( string start, bool printDebug )
{
    if( printDebug && this->doNotify )
    {
        ostringstream ostream;
        ostream << "Notify: entered deleteGrowthRules for rule starting with: " << start;
        logger.print( ostream.str() );
    }

    for( map<string,string>::iterator iter = growthRules.begin(); iter != growthRules.end(); ++iter )
    {
        if( (*iter).first == start )
        {
            growthRules.erase( iter );
            //iterator now invalidated! Need to break.
            return true;
        }
    }

    return false;
}

//sets a mutation probability
//type is 1 2 3 4
//probability is the probability that the mutation will happen if the type is choosen in 
//step()
void LSystemGeneric::setMutationProbability( string mutationType, int probability, bool printDebug )
{
    if( printDebug && this->doNotify )
    {
        ostringstream ostream;
        ostream << "Notify: entered setProbability for mutation type: " << mutationType;
        logger.print( ostream.str() );
    }

    this->mutationProbabilities[mutationType] = probability;
   
}

//adds an replacement option for mutations
void LSystemGeneric::addOption( string option, bool printDebug )
{
    if( printDebug && this->doNotify )
    {
        ostringstream ostream;
        ostream << "Notify: entered addOption for option: " << option;
        logger.print( ostream.str() );
    }
    
    this->options.push_back( option ); 
}

//delete the option given
void LSystemGeneric::deleteOption( string option, bool printDebug )
{
    if( printDebug && this->doNotify )
    {
        ostringstream ostream;
        ostream << "Notify:: entered deleteOption for option: " << option;
        logger.print( ostream.str() );
    }

    for( vector<string>::iterator iter = this->options.begin(); iter != this->options.end(); ++iter )
    {
        if( *iter == option )
        {
            this->options.erase( iter );
            //gotta break, just invalidated the iterator
            break;
        }
    }
}

string LSystemGeneric::getOptionsString(bool printDebug)
{
    logger.print( "Notify: entered getOptionsString() ", printDebug && this->doNotify );

    string options = "";
    for( unsigned int i = 0; i < this->options.size(); ++i )
    {
        if( i != 0 )
            options += "," + this->options[i];
        else
            options += this->options[i];
    }
    return options;
}


//so similar to getIndices.... maybe pass function pointer??? signatures different.... err...
template <class T>
bool LSystemGeneric::getKeys( string key, string& key1, string& key2, 
                              const map<string,T>& container1, const map<string,T>& container2, 
                              bool printDebug )
{
    logger.print( "Notify: entered LSystemGeneric::getKeys<T>( string, string&, string& ... ) ", this->doNotify && printDebug );
    unsigned long colonIndex = key.find( ":" );
    if( colonIndex == string::npos || colonIndex == 0 || colonIndex == key.length() - 1 )
    {
        if( printDebug )
        {
            string errorMsg;
            errorMsg = "Warning: in LSystemGeneric::getMutationIndices key: " + key + " does not have a colon";
            errorMsg += " or the colon is at the beginning or end of the string";
            logger.print( errorMsg );
        }

        return false;
    }

    if( container1.size() == 0 || container2.size() == 0 )
    {
        logger.print( "Warning: in LSystemGeneric::getKeys(), can't get keys of empty container!" );
        return false;
    }

    if( !getKey( key.substr( 0, colonIndex ), key1, Utility::getMapKeys( container1 ) ) )
    {
        logger.print( "Warning: in LSystemGeneric::getIndices, failed to get index 1", printDebug );
        return false;
    } 

    if( !getKey( key.substr( colonIndex + 1, key.length() ), key2, Utility::getMapKeys( container2 ) ) )
    {
        logger.print( "Warning: in LSystemGeneric::getIndices, failed to get index 2", printDebug );
        return false;
    }
    
    return true;    
}

bool LSystemGeneric::getKey( string indexString, string& key, const vector<string>& keys, bool printDebug )
{
    logger.print( "Notify: entered LSystemGeneric::getKey( string, string&, const vector<string>& ... )", this->doNotify && printDebug );
    if( indexString.find( "random" ) != string::npos )
    {
        int probability = 0;
        if( !getRandomKey( key, probability, keys, printDebug ) )
        {
            return false;
        }
    }
    else
    {
        key = indexString;
    }

    return true;
}

bool LSystemGeneric::getRandomKey( string& type, int& probability, const vector<string>& keys, bool printDebug )
{
    logger.print( "Notify: entered LSystemGeneric::getRandomKey()", this->doNotify &&& printDebug );
    if( keys.size() == 0 )
    {
        logger.print( "Warning: in lSystemGeneric::getRandomKey() tried to get the random key of an empty map" );
        return false;
    }
    int index = Utility::getRandomInteger( 0, keys.size(), printDebug );
    type= keys[index];
    probability = this->mutationProbabilities[keys[index]];

    return true;
}

template <class T>
bool LSystemGeneric::crossMaps( map<string,T>& map1, map<string,T>& map2, const map<string,string>& crossMap, bool printDebug  )
{   
    logger.print( "Notify: entered LSystemGeneric::crossMaps()", this->doNotify && printDebug );
   
    if( map1.size() == 0 || map2.size() == 0 )
    {
        logger.print( "Warning: in LSystemGeneric::crossMaps(), can't cross empty maps!" );
        return false;
    }
    
    for( map<string,string>::const_iterator i = crossMap.begin(); i != crossMap.end(); ++i )
    {
        string key = (*i).first;
        string index1;
        string index2;

        if( !getKeys( key, index1, index2, map1, 
                      map2, printDebug 
                    ) 
          )
        {
            return false;
        }

        //prevent from assigning from positions that don't exist.
        //(otherwise just overwrites/swaps with "undefined" - usually NULL entry)
        if( map1.find( index1 ) == map1.end() || map2.find( index2 ) == map2.end() )
        {
            continue;
        }

        if( (*i).second == "1" )
        {
            map2[index2] = map1[index1]; 
        }
        else if( (*i).second == "2" )
        {
            map1[index1] = map2[index2];
        }
        else if( (*i).second == "3" )
        {
            /*
            ostringstream ostream1;
            ostringstream ostream2;
            ostream1 << map1[index1];
            ostream2 << map2[index2];
            cout << "swapping: " << ostream1.str() << " with " << ostream2.str() << endl;
            */
            T temp = map1[index1];
            map1[index1] = map2[index2];
            map2[index2] = temp;
        }
    }

    return true;

}

//crosses two systems
bool LSystemGeneric::cross( System* lsystem2, const map<string,string>& systemCrossMap, const map<string,string>& optionsCrossMap,
            const map<string,string>& mutationProbabilitiesCrossMap, const map<string,string>& growthRulesCrossMap, bool printDebug )
{
    logger.print( "Notify: entered cross in LSystemGeneric", printDebug && this->doNotify );

//    cout << "lsystem2 at start: " << Utility::assembleVector( lsystem2->getSystemVector() ) << endl;
    LSystemGeneric* lSystemGeneric2 = dynamic_cast<LSystemGeneric*>( lsystem2 );
    if( lSystemGeneric2 == NULL )
    {
        logger.print( "Notify: in LSystemGeneric::cross, passed incorrect type of System* (should be lSystemGeneric)" );
        return false;
    }
    
    if( this->system.size() > 0 && lSystemGeneric2->system.size() > 0 &&
        !crossVectors( this->system, lSystemGeneric2->system, systemCrossMap,
                       this->system.size(), lSystemGeneric2->system.size(), printDebug )
      )
    {
        logger.print( "Warning: cross of the lsystems failed", printDebug );
        return false;
    }
    
    if( this->options.size() > 0 && lSystemGeneric2->options.size() > 0 && 
        !crossVectors( this->options, lSystemGeneric2->options, optionsCrossMap,
                       this->options.size(), lSystemGeneric2->options.size(), printDebug ) 
      ) 
    {
        logger.print( "Warning: cross of the options failed", printDebug );
        return false;
    }
    
    if( this->mutationProbabilities.size() > 0 && lSystemGeneric2->mutationProbabilities.size() > 0 &&
        !crossMaps( this->mutationProbabilities, lSystemGeneric2->mutationProbabilities, 
                    mutationProbabilitiesCrossMap, printDebug ) )
    {
        logger.print( "Warning: cross of the options failed", printDebug );
        return false;
    }

    if( this->growthRules.size() > 0 && lSystemGeneric2->growthRules.size() > 0 
        && !crossMaps( this->growthRules, lSystemGeneric2->growthRules, 
                    growthRulesCrossMap, printDebug ) )
    {
        logger.print( "Warning: cross of the options failed", printDebug );
        return false;
    }
    return true;
}
 
bool LSystemGeneric::crossVectors( vector<string>& parent1, vector<string>& parent2, const map<string,string>& crossMap,
                            unsigned int parent1size, unsigned int parent2size, bool printDebug )
{
    logger.print( "Notify: entered LSystemGeneric::crossVectors", this->doNotify && printDebug );
    if( parent1size == 0 || parent2size == 0 )
    {
        logger.print( "Warning: in LSystemGeneric::crossVectors, can't cross vectors with size <= 0" );
        return false;
    }
    for( map<string,string>::const_iterator i = crossMap.begin(); i != crossMap.end(); ++i )
    {
        string key = (*i).first;
        unsigned int index1 = 0;
        unsigned int index2 = 0;

        if( !getIndices( key, index1, index2, parent1size, parent2size, printDebug ) )
        {
            return false;
        }

        if( index1 < 0 || index1 >= parent1size )
        {
            if( printDebug )
            {
                ostringstream ostream;
                ostream << "Warning: in LSystemGeneric::cross, index1: " << index1 << " out of range";
                bool success = false;
                ostream << " Parent size is: " << parent1size << " parent is: " << Utility::vectorToString<string>( parent1, success ) << endl;
                logger.print( ostream.str() );
            }

            return false;
        }

        if( index2 < 0 || index2 >= parent2size )
        {
            if( printDebug )
            {
                ostringstream ostream;
                ostream << "Warning: in LSystemGeneric::cross, index2: " << index2 << " out of range";
                bool success = false;
                ostream << " Parent size is: " << parent2size << " parent is: " << Utility::vectorToString<string>( parent2, success ) << endl;
                logger.print( ostream.str() );
            }

            return false;
        }

        cout << "operation is: " << (*i).second << endl;
        if( (*i).second == "1" )
        {
            parent2[index2] = parent1[index1]; 
        }
        else if( (*i).second == "2" )
        {
            parent1[index1] = parent2[index2];
        }
        else if( (*i).second == "3" )
        {
            //cout << "crossing vector items: " << parent1[index1] << " and " << parent2[index2] << endl;
            string temp = parent1[index1];
            parent1[index1] = parent2[index2];
            parent2[index2] = temp;
        }
    }

    return true;

}

bool LSystemGeneric::getIndices( string key, unsigned int& index1, unsigned int& index2, 
                                 unsigned int parent1size, unsigned int parent2size, bool printDebug )
{

    if( parent1size == 0 || parent2size == 0 )
    {
        logger.print( "Warning: in LSystemGeneric::getIndices, can't get indices for systems of size <= 0" );
        return false;
    }
    unsigned long colonIndex = key.find( ":" );
    if( colonIndex == string::npos || colonIndex == 0 || colonIndex == key.length() - 1 )
    {
        if( printDebug )
        {
            string errorMsg;
            errorMsg = "Warning: in LSystemGeneric::getIndices, key: " + key + " does not have a colon";
            errorMsg += " or the colon is at the beginning or end of the string";
            logger.print( errorMsg );
        }
        return false;
    }

    if( !getIndex( key.substr( 0, colonIndex ), index1, parent1size, printDebug ) )
    {
        logger.print( "Warning: in LSystemGeneric::getIndices, failed to get index 1", printDebug );
        return false;
    } 

    if( !getIndex( key.substr( colonIndex + 1, key.length() ), index2, parent2size, printDebug ) )
    {
        logger.print( "Warning: in LSystemGeneric::getIndices, failed to get index 2", printDebug );
        return false;
    }

    return true;
}

bool LSystemGeneric::getIndex( string indexString, unsigned int& index, unsigned int size,
                               bool printDebug )
{
    logger.print( "Notify: in LSystemGeneric::getIndex( string, unsigned int&, unsigned int... ) ", this->doNotify && printDebug );
    if( size == 0 )
    {
        logger.print( "Warning: in LSystemGeneric::getIndex() can't get index for size == 0 " );
        return false;
    }
    if( indexString.find( "random" ) != string::npos )
    {
        index = Utility::getRandomInteger( 0, size, printDebug ); 
        if( index == size )
        {
            logger.print( "Error: In getRandomInteger implementation, size == index" );
            return false;
        }
    }
    else
    {
        bool success = false;
        index = Utility::stringTo<int>( indexString, success, printDebug );
        if( !success )
        {
            logger.print( "Warning: in LSystemGeneric::cross, couldn't covert string: " + indexString + " to an index", printDebug );
            return false;
        }
    }

    return true;

}

bool LSystemGeneric::checkMutationTypeSupported( string type, bool printDebug )
{
    logger.print( "Notify: entered checkMutationType supported", printDebug && this->doNotify );
    return type == "1" || type == "2" || type == "3" || type == "4";
}

void LSystemGeneric::printStats(bool printDebug)
{
    cout << "***************************** System stats **********************************" << endl;
    cout << "final system: " << this->getSystem()  << endl;
    cout << "mutationProbabilities for this systems where: " << endl;
    cout << "size of mutationProbabilities: " << this->mutationProbabilities.size() << endl;
    if( this->mutationProbabilities.size() == 0 )
    {
        cout << "Error: mutationProbability of vector was of size 0"  << endl;
    }
    else
    {
        cout << "case 1(swap) had a prob of: " << this->getMutationProbabilities()["1"] / 100.0 << "%"  << endl;
        cout << "case 2(toggle) had a prob of: " << this->getMutationProbabilities()["2"] / 100.0 << "%"  << endl;
        cout << "case 3(delete) had a prob of: " << this->getMutationProbabilities()["3"] / 100.0 << "%"  << endl;
        cout << "case 4(double) had a prob of: " << this->getMutationProbabilities()["4"] / 100.0 << "%"  << endl;
    }
    cout << "The options for possible toggle values were: "  << endl;
    for( unsigned int i = 0; i < this->options.size(); ++i )
    {
        cout << this->options[i]  << endl;
    }

    cout << "Production rules are: "  << endl;
    cout << getGrowthRules()  << endl;
    cout << "The maximum length of this system was: " << this->maxSize  << endl;
    cout << "The minimum length of this system was: " << this->minSize  << endl;
    cout << "The current length of this system was: " << this->system.size()  << endl;
    
    cout << "***************swap stats************************"  << endl;
    cout << "Number of Swaps performed: " << this->numSwaps  << endl;
    cout << "Number swapped left: " << this->swappedLeft  << endl;
    cout << "Number swapped right: " << this->swappedRight  << endl;
    cout << "****************toggled stats*********************"  << endl;
    cout << "Number of toggles performed: " << this->numToggled  << endl;
    for( map<string,int>::iterator i = toggledStats.begin(); i != toggledStats.end(); ++i )
    {
        cout << "option: " << i->first << " chosen: " << i->second << " times "  << endl;
    }
    cout << "****************Deleted stats**********************"  << endl;
    cout << "numDeleted: " << numDeleted  << endl;
    cout << "****************Doubled stats**********************"  << endl;
    cout << "numDoubled: " << numDoubled  << endl;
    cout << "*************************** end system stats *********************"  << endl;
}

#endif
