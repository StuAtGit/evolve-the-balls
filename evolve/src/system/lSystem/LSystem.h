#ifndef STU_L_SYSTEM_H
#define STU_L_SYSTEM_H

#include <fstream>
#include <map>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sys/time.h>
#include "CommonAspect.h"
#include "System.h"

using namespace std;

#define DO_DEBUG true
namespace evolve
{
    /**
     * LSystem class reads in a file containing a string 
     * an information on how to evolve that string, namely
     * probablities of change, values and positions.
     *
     * Rules for Mutations
     * 
     * Mutation Probabilites for evolving are as follows:
     * <ul>
     * <li>
     * The probabilities consists of type/probability pairs 
     * </li>
     * <li>
     * The pairs determine the evolution as follows:
     * </li>
     * the probability is represented as an integer with the smallest
     * increment being 1/100 of a percent. (the increment size may vary from
     * implementation to implementation) 
     * </li>
     * <li>
     * the value stored * 0.0001 gives the percentage as a decimal or the percent *.01
     * that the second entry (the type) will have any affect. The second entry determines the type of mutation operation to perform.
     * </li>
     * </ul>
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
     * and your system will probably grow much quicker than you expect! <br/><br/>
     * <b>Growth Rules should not contain the following characters: '[',']',',',':'</b><br/><br/>
     * Unless otherwise noted, methods which return bool return true on success, false on failure.\n
     * Unless otherwise noted, the debug or printDebug argument to each method indicated 
     * whether or to display debug messages. 
     */
    class LSystem : public System
    {
    
     public:
        virtual ~LSystem() {};
        /**
        Initialize the system from a file stream.
        The filestream is expected to point at a file in the lSystem save file format.
        @param file the filestream to be read from
        */
        virtual bool initialize( ifstream& file, bool debug = DO_DEBUG ) = 0;
        /**
        Initialize the system from a file.
        The filename is to the name of a file in the lSystem save file format.
        @param file the name of the file to read from
        */
        virtual bool initialize( string file, bool debug = DO_DEBUG ) = 0;
        /**
        Intializes the lsystem to the given arguments, setting 
        defaults for all other values. 
        virtual bool initialize( const vector<string>& system, const map<string,int>& mutationProb, 
                                 const vector<string>& options,  bool printDebug = DO_DEBUG ) = 0; 
        * Initializes from a set of key-value pairs (e.g. system agtct, options (1,500),(2,300), etc.) 
        * @param the key value pairs (note the passed argument could possibly be altered...)
        virtual bool initialize(  map< string, string >& lsystem, bool debug = DO_DEBUG ) = 0;
        */
    
        /**
        * Initializes the lSystem from a string containing the the contents of
        * an lSystem save file
        * @param fileContents the contents of an lsystem save file
        */   
        virtual bool initializeFromFileContents( string fileContents, bool printDebug = DO_DEBUG ) = 0;
        /**
         * Save the lsystems state to a file
         * @param filename the name of the file to save to
         */
        virtual bool save( string filename, bool printDebug = DO_DEBUG ) = 0;
        /**
        runs one step of evolution.
        @param mode a string with the following effect:
        "mutate" mutate
        "grow" grow (according to production rules)
        @param query a response parameter that is used to make environmental queries
        */
        virtual bool run( const string& mode, string& query, bool printDebug = DO_DEBUG ) = 0; 
        /**
        @return the current L-system state
        */
        const virtual string getSystem( bool printDebug = DO_DEBUG ) = 0;
        /**
        @return the current L-system state as a vector
        */
        virtual vector<string> getSystemVector( bool printDebug = DO_DEBUG ) = 0;
        /**
        @return the minimum size of the system
        */
        virtual int getMinSize( bool printDebug = DO_DEBUG ) = 0;
        /**
        @return the maximum size this string is allowed to reach
        */
        virtual int getMaxSize( bool printDebug = DO_DEBUG ) = 0;
        /**
        @return the options available for toggling
        */
        virtual vector<string> getOptions( bool printDebug = DO_DEBUG ) = 0;
        /**
        @return the integer defining the ruleset for this system
        */
        virtual map<string,int> getMutationProbabilities( bool printDebug = DO_DEBUG ) = 0;
        /**
        @return the current growth rules in the string format they will appear in in the save file
        */
        virtual string getGrowthRules( bool printDebug = DO_DEBUG ) = 0;
        /**
        @return the current growth rules as a map<string,string>
        */
        virtual map<string,string> getGrowthRuleMap( bool printDebug = DO_DEBUG ) = 0;
        /**
        @return a human-friendly description of each mutation type
        */
        virtual string getMutationTypeDescription( string type,  bool printDebug = DO_DEBUG ) = 0;
        /**
         * @return the id of this lsystem as a string
         * */
        const virtual string getId( bool printDebug = DO_DEBUG ) = 0;
        /**
         * Returns a copy of this lsystem. Not implemented as
         * copy constructor to allow for different behavior in case
         * of LSystemInterface, etc, where you don't want a copy of 
         * the object itself, rather it creates a remote copy on the
         * server and returns an object controlling that remote copy.
         * It returns a pointer so any LSystem type can be returned
         *
         * @param a pointer pointing at a <b>copy</b> of this lsystem.
         *        if this pointer is not set to null delete will be
         *        called on the pointer, and any info pointed at will
         *        be lost. In addtion, if it is not set to null, and
         *        points at nothing, undefined behavior may result.
         *        <b>Initialize your pointers!</b>
         * 
         * @return true on success, false on failure      
         * * */
        const virtual bool getCopy( System*& copy, bool printDebug = DO_DEBUG ) = 0;
        /**
         * @param id the id to set this class to
         * @return if the id was set, true, false otherwise
         * */
        virtual bool setId( string id, bool printDebug = DO_DEBUG ) = 0;
        /**
         Set the minimum size the string can reach
         @param minSize the minimum size desired, negative integers will be set to 0
        */
        virtual bool setMinSize( int minSize, bool printDebug = DO_DEBUG ) = 0;
        /**
        Set the maximum size the string can reach
        @param maxSize the maximum size desired, 
               negative integers means it will not check for max size when growing.
        */
        virtual bool setMaxSize( int maxSize, bool printDebug = DO_DEBUG ) = 0;
        /**
        Set the system to something else
        @param system a vector of strings that make up the system
        */
        virtual bool setSystem( vector<string> system, bool printDebug = DO_DEBUG ) = 0;
        /**
        Set a growth rule
        @param start the left side of the production rule (what will be replaced)
        @param end the right side of the production rule (what will be substituted in for start)
        */
        virtual bool setGrowthRule( string start, string end, bool printDebug = DO_DEBUG ) = 0;
        /**
        Deletes a growth rule
        @param start the left side of the rule to be deleted 
        @return true if an entry matching start was found and deleted, false otherwise
        */
        virtual bool deleteGrowthRule( string start, bool printDebug = DO_DEBUG ) = 0;
        /**
        Sets the probabilities for the mutation options
        @param mutationType the type of mutation you wish to adjust the probability for
        @param probability the probability to set for the mutation, given in hundreths of a percent
        */
        virtual void setMutationProbability( string mutationType, int probability, bool printDebug = DO_DEBUG ) = 0;
        /**
        Adds an option to the existing options string
        @param option the new option to be made available for mutation
        */
        virtual void addOption( string option, bool printDebug = DO_DEBUG ) = 0;
        /**
        Deletes an option from the existing options string
        @param option the option you wish to remove from being used in mutation 
        */
        virtual void deleteOption( string option, bool printDebug = DO_DEBUG ) = 0;
        /**
         parses the mutation probabilties (passed in LSystem save file format)
         into an appropriately structured hashtable of ints 
         @param mutationProbabilities a string representing the mutation probabilities
                                      given as pairs contained in ()'s
                                      each element in the pair separated by a ,
                                      the first number in the pair is the probability,
                                      the second the type of mutation to apply the 
                                      probability to
        virtual map<string,int> parseMutationProbabilities( string mutationProbabilties, bool printDebug = DO_DEBUG ) = 0;
        */
        /**
        Checks if the mutation type is supported
        @param type the type of mutation to check
        @return true of the mutation type is supported, false otherwise
        */
        virtual bool checkMutationTypeSupported( string type, bool printDebug = DO_DEBUG ) = 0;
    
        /**
        * Crosses this lsystem with another lsystem.
        *
        * The system cross maps and the options crossMap  are 
        * structured as follows:\n
        *        The key can be random:[number], [number]:random, random:random or [number]:[number], \n
        *        where the random keyword specifies the index to use should be choosen at random. \n
        *        Otherwise, the key is assumed to be a number that indicates the index to use. 
        *        The operation to the left of the colon always refers to this object,\n 
        *        while the operation on the right refers to the LSystem object being passed to the method. \n
        *        The value is an integer 1-3, that specifies which operation to use, as follows:\n
        *        1) move the element at the index in this system to the index in the other system (1->2)\n
        *        2) move the element at the index in the other system to this system (2->1) \n
        *        3) swap elements at this index\n
        *
        * @param lsystem the lsystem to cross this system with
        * @param systemCrossMap rules for crossing the existing system (see method dec.) 
        * @param optionsCrossMap rules for crossing the options (see method dec.)
        * @mutationProbabilitiesCrossMap The mutation probabilities cross map is similar, except [number] is the number indicating
        * the type of mutation to alter the probability on.\n
        * Operations are specified identically 
        *
        * @param growthRulesCrossMap The growth crossMap is keyed similarly, but like so: random:[start], etc, where 
        * [start] is the start of the production.\n
        * Operations are specified identically.
        *
        * Note to extenders of this class... yes the interface isn't perfect, not too general, but
        * remember, you just need to respect the argument *types*, and you can change the 
        * semantics all you want.
        * Just be sure to doc your stuff..
        **/
        virtual bool cross( System* lsystem, const map<string,string>& systemCrossMap, const map<string,string>& optionsCrossMap,
                            const map<string,string>& mutationProbabilitiesCrossMap, const map<string,string>& growthRulesCrossMap, bool printDebug = DO_DEBUG ) = 0;
    
    };

};
#endif
