#1) if no file given as argument, ask for class name & method signatures 
#auto-generate .c, .h, files, add printDebug args. Add logger class
#automatically to members. Set #define DO_DEBUG. auto-write constructor.

#TODO:
# code writeMakefile
# start gui framework for code generation: code viewing probably will not be done
# ,at least anytime soon
# make unit test

#2) convert existing debug messages in existing code to use logger.print messages!
#   since this is a temp hack, that shouldn't be needed (after #1)
#   you can have the user enter the macro used for output? Or maybe 
#   recognize #define outputMacro cout/cerr

#3) process existing files to add printDebug where none exists. 
#and appr. error messages. (scan .h and .c files with given name).

#4) auto-generate makefiles given the root evolve directory??

import os, sys, errno, stuClassGenerator

if( len(sys.argv) < 2 or sys.argv[1] == "" ):
    classNamePrompt = "No classname given, what is the name of the class you wish to create? "
    className = raw_input( classNamePrompt )
else:
    className = sys.argv[1]


developerId = ""

if( len( sys.argv ) < 3 ):
    methdodNamePrompt = "No method names given, enter your method names one at \
                         a time, enter q to quit (note you cannot have a method \
                         named 'q'"
    methodName = ""
    methodReturnType = ""
    methods = []
    while( methodName != "q" ):
        method = dict();
        methodName = raw_input( "Enter method name: " )
        if( methodName == "q" ):
            continue
        
        if( methodName != className ):
            methodReturnType = raw_input( "Enter method return type (and/or specify virtual): " )
        methodArguments = [];
        methodArgumentString = "";

        if( methodName != className ):
            methodArgumentString = raw_input( "Enter the methods arguments: " )
            methodArguments = methodArgumentString.split( "," )
        
        for arg in methodArguments:
            print arg
            
        method = { 'returnType':methodReturnType, 'name':methodName, 'arguments':methodArguments };
        if methodName != "q":
            methods.append( method );

#else:
#    for i in range( 2, len( sys.argv ) ):
 #       if argv[i] != "--dev-id":
  #          methods[i] = argv[i];
 #       else:
 #           if len( sys.argv ) > i:
 #               developerId = argv[ i + 1 ];

if developerId == "":
    developerId = raw_input( "Enter a developerId to use: " )

generator = stuClassGenerator.StuClassGenerator( className, methods, developerId )
generator.initStandardHeaders()

print "Creating a class with name: " + className
print "And methods: "
for i in range( 0, len( methods ) ):
    print methods[i]['returnType'] + " " + methods[i]['name'];

generator.writeHeaderFile()
generator.writeImplementationFile()
    
