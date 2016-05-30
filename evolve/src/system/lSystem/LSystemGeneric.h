#ifndef STU_L_SYSTEM_GENERIC_H
#define STU_L_SYSTEM_GENERIC_H

#include <fstream>
#include <map>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sys/time.h>
#include <boost/tokenizer.hpp>
#include "Utility.h"
#include "LSystem.h"
#include "Logger.h"

using namespace std;
using namespace boost;
using namespace evolve;
using namespace Utility;

#define DO_DEBUG true 

namespace evolve
{
/**
 * LSystemGeneric class reads in a file containing a string 
 * an information on how to evolve that string, namely
 * probablities of change, and values and positions to chance.
 * Can also create it's own default string (11111111), and 
 * rules for Mutations
 * 
 * Mutation Probabilites for evolving are as follows:
 * The probabilities are held in a vector consists of a set of 4 pairs of integers, with each pair corresponding to th
 * with a key for each type listed below 
 * The pairs determine the evolution as follows:
 * the first entry is the probability (the entry * 0.0001 or the percent *.01)
 * that the second will have any
 * affect. The second entry affects it's corresponding entry in the system
 * like so:
 *
 * 1) The entry is swapped with one of it's neighbors
 *
 * Future:
 * the probability 
 * of the neighbor being picked decreasing with it's distance. (using gaussian
 * distribution)
 *
 * 2) The entry is toggled to one of the possible values for each entry, 
 * given by the user. 
 *
 * Future: a probablity may be given with each of the possible values made
 * available.
 *
 * 3) The entry is deleted.
 *
 * 4) The entry is doubled. (i.e., is inserted next to itself in the string).
 *
 * Options <br/>
 * Each option is one word that <b>may not contain commas</b>. These are used for the replacement 
 * mutation noted above. If replacement happens, each options has an equally likely chance of 
 * happening.
 *
 * Growth Rules <br/>
 * Replacement that happens in a deterministic fashion
 * Currently the string is scanned every step, and each entry
 * checked for eligibility against all the production rules. 
 * So setting a growth rule of A->BA, means every A will be replaced with BA every turn,
 * and your system will probably grow much quicker than you expect!<br/><br/>
 * <b>Growth Rules should not contain the following characters: '[',']',',',':'</b>
 *
 */
class LSystemGeneric : public LSystem
{

 public:
    
    //constructors/destructors
    LSystemGeneric();
    virtual ~LSystemGeneric() {};
    bool initialize( ifstream& file, bool debug = DO_DEBUG );
    bool initialize( string file, bool debug = DO_DEBUG );
    bool initialize( map< string, string >& lsystem, bool debug = DO_DEBUG );
    bool initializeFromFileContents( string fileContents, bool debug = DO_DEBUG );
    bool save( string filename, bool printDebug = DO_DEBUG );

    //accessors
    const string getSystem( bool printDebug = DO_DEBUG );
    vector<string> getSystemVector( bool printDebug = DO_DEBUG );
    int getMinSize( bool printDebug = DO_DEBUG );
    int getMaxSize( bool printDebug = DO_DEBUG );
    vector<string> getOptions( bool printDebug = DO_DEBUG );
    map<string,int> getMutationProbabilities( bool printDebug = DO_DEBUG );
    bool getIsInitialized( bool printDebug = DO_DEBUG );
    string getGrowthRules( bool printDebug = DO_DEBUG );
    map<string,string> getGrowthRuleMap( bool printDebug = DO_DEBUG );
    string getMutationTypeDescription( string type,  bool printDebug = DO_DEBUG );
    const string getId( bool printDebug = DO_DEBUG );
    const bool getCopy( System*& copy, bool printDebug = DO_DEBUG );

    //mutators (mostly)
    bool setId( string id, bool printDebug = DO_DEBUG );
    bool run( const string& mode, string& query, bool printDebug = DO_DEBUG ); 
    bool setMinSize( int minSize, bool printDebug = DO_DEBUG );
    bool setMaxSize( int maxSize, bool printDebug = DO_DEBUG );
    bool setSystem( vector<string> system, bool printDebug = DO_DEBUG );
    bool setGrowthRule( string start, string end, bool printDebug = DO_DEBUG );
    bool deleteGrowthRule( string start, bool printDebug = DO_DEBUG );
    void setMutationProbability( string mutationType, int probability, bool printDebug = DO_DEBUG );
    void addOption( string option, bool printDebug = DO_DEBUG );
    void deleteOption( string option, bool printDebug = DO_DEBUG );
    bool checkMutationTypeSupported( string type, bool printDebug = DO_DEBUG );
    void printStats( bool printDebug = DO_DEBUG );
    bool cross( System* lsystem, const map<string,string>& systemCrossMap, const map<string,string>& optionsCrossMap,
                const map<string,string>& mutationProbabilitiesCrossMap, const map<string,string>& growthRulesCrossMap
                ,bool printDebug = DO_DEBUG ); 
    
 private:
    //private methods
    int getReps( string::iterator& pos, string::iterator end, bool printDebug );
    template <class T>
    bool getKeys( string key, string& key1, string& key2, const map<string,T>& container1,
                  const map<string,T>& container2, bool printDebug );
    bool getKey( string indexString, string& key, const vector<string>& keys,
                 bool printDebug = DO_DEBUG );
    bool getRandomKey( string& type, int& probability, const vector<string>& keys, bool printDebug );
    template <class T>
    bool crossMaps( map<string, T>& map1, map<string,T>& map2, const map<string,string>& crossMap, bool printDebug );
    bool crossVectors( vector<string>& parent1, vector<string>& parent2, const map<string,string>& crossMap,
                unsigned int parent1size, unsigned int parent2size, bool printDebug );
    bool getIndices( string key, unsigned int& index1, unsigned int& index2, 
                     unsigned int parent1size, unsigned int parent2size,
                     bool printDebug );
    bool getIndex( string indexString, unsigned int& index, unsigned int size,
                   bool printDebug );
 
    bool parseMutationProbability( string::iterator& pos, string::iterator end, map<string,int>& ret, bool printDebug );
    string getOptionsString( bool printDebug );
    bool checkWord( string word, bool printDebug );
    /**
     * If the grow() method is expecting a query response, this
     * checks if the response is valid
     */
    bool checkQueryResponse( string query, bool printDebug );
    bool grow( string& query, bool printDebug );
    bool mutate( bool printDebug );
 
    /***************private members************************/
    vector<string> system;
    map<string,int> mutationProbabilities;
    //options for replacement
    vector<string> options;
    map<string,string> growthRules;
    bool expectingQueryResponse;
    bool isInitialized;
    unsigned int currentGene;
    unsigned int minSize;
    unsigned int maxSize;
    //stats collected by system
    //general stats
    long long numSteps;
    //case 1
    long long numSwaps;
    long long swappedLeft;
    long long swappedRight;
    //case 2
    long long numToggled;
    map< string, int > toggledStats;
    //case 3
    long long numDeleted;
    //case 4
    long long numDoubled;
    string id;

    //Names of config file sections
    //attributes of the system
    string max_size_system;;
    string min_size_system;;
    string mutation_options;;
    string seed_system;;
    string growth_rules;;
    string mutation_rules;;

    //stats of the system
    string num_steps;;
    string num_swaps;;
    string swapped_left;;
    string swapped_right;;
    string num_toggled;;
    string num_deleted;;
    string num_doubled;;
    string current_gene;;

    //comment mark (any line beginning with this mark is ignored)
    string comment_mark;;
};
};
#endif
