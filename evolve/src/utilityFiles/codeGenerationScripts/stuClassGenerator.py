#class to generate C++ classes in the "stu" style

import os, sys
import stuTestGenerator

class StuClassGenerator:
    className = ""
    developerId = ""
    methods = []
    doNotify = False
    headers = []
    
    
    def __init__( this, className, methods, developerId ):
        this.className = className
        this.methods = methods
        this.developerId = developerId

    def addHeaders( this, headers ):
        for header in headers:
            this.headers.append( header )

    def addHeader( this, header ):
        this.headers.append( header )

    def initStandardHeaders( this ):
        this.headers.append( "<iostream>" )
        this.headers.append( "<vector>" )
        this.headers.append( "<map>" )
        this.headers.append( "<sstream>" )
        this.headers.append( "<string>" )
        this.headers.append( "<fstream>" )
        
    def writeHeaderFile( this, printDebug = True ):
        if this.doNotify and printDebug:
            print "Notify: entered writeHeaderFile"
            
        try:
            headerFile = open( this.className + ".h", "wt" )
        except IOError:
            if printDebug:
                print "Error: couldn't open header file: " + this.className + ".h for writing " + IOError + "\n"
            return False;


        headerFile.write( "#ifndef " + this.developerId + "_" + this.className + "_H\n" )
        headerFile.write( "#define " + this.developerId + "_" + this.className + "_H\n" )
        headerFile.write( "\n" )
        for header in this.headers:
            headerFile.write( "#include " + header + "\n" )
        
        headerFile.write( "\n" )
        headerFile.write( "#define DO_DEBUG true\n" )
        headerFile.write( "\n" )
        headerFile.write( "using namespace std;\n" )
        headerFile.write( "using namespace boost;\n" )
        headerFile.write( "\n" )
        headerFile.write( "/**TODO: insert class comments*/\n" )
        headerFile.write( "class " + this.className + "\n" )
        headerFile.write( "{\n" )
        headerFile.write( "public:\n" )
        for method in this.methods :
            if( method['name'] != this.className ):
                headerFile.write( "    /**\n" )
                headerFile.write( "    *\n" );
                headerFile.write( "    */\n" )
                method['arguments'].append( "bool printDebug = DO_DEBUG" )
                headerFile.write( "    " + method['returnType'] + " " + method['name'] + "( " + this.makeArgumentList( method['arguments'] ) + " )" )
                method['arguments'] = method['arguments'][0:len(method['arguments']) - 1]
                if( method['returnType'].startswith( "virtual" ) ):
                    headerFile.write( " = 0" );
                headerFile.write( ";\n" )
            else:
                headerFile.write( "    " + method['name'] + "( " + this.makeArgumentList( method['arguments'] ) + " );\n" )
                
        headerFile.write( "private:\n" )
        headerFile.write( "};\n" )
        headerFile.write( "\n" )
        headerFile.write( "#endif" )
        headerFile.close()
        return True

    def writeImplementationFile( this, printDebug = True ):
        if this.doNotify and printDebug:
            print "Notify: entered writeImplementationFile\n"
        try:
            ImplementationFile = open( this.className + ".cc", "wt" )
        except IOError, e:
            print "Error: failed to open file: " + this.className + ".cc "
            print e
            return False
        ImplementationFile.write( "#ifndef " + this.developerId + "_" + this.className + "_CC\n" )
        ImplementationFile.write( "#define " + this.developerId + "_" + this.className + "_CC\n" )
        ImplementationFile.write( "\n" );
        ImplementationFile.write( "#include \"" + this.className + ".h\"\n" );
        ImplementationFile.write( "\n" );
        for method in this.methods:
            if method['name'] != this.className and (not method['returnType'].startswith( "virtual" )):
                method['arguments'].append( " bool printDebug" )
                ImplementationFile.write( method['returnType'] + " " + this.className + "::" +\
                                          method['name'] + "( " + this.makeArgumentList( method['arguments'] ) + " )\n" )
                ImplementationFile.write( "{\n" );
                ImplementationFile.write( "    logger.print( \"Notify: entered method: " + \
                                                   method['returnType'] + " " + method['name'] +\
                                                   "(" + this.makeArgumentList( method['arguments'] ) +\
                                                   ")  of class " + this.className + "\", this->doNotify && printDebug );\n" )
                if method['returnType'].strip() != "void" :
                    ImplementationFile.write("    " + method['returnType'] + " ret;\n" )
                    ImplementationFile.write( "    return ret;\n" )
                else:
                    ImplementationFile.write( "    return;\n" )
                ImplementationFile.write( "}\n\n" )
            else:
                if method['name'] == this.className :
                    ImplementationFile.write( this.className + "::" + method['name'] + "( " + this.makeArgumentList( method['arguments'] ) + " )\n" )
                    ImplementationFile.write( "{\n" )
                    ImplementationFile.write( "    return;\n" )
                    ImplementationFile.write( "}\n\n" )
        ImplementationFile.write( "#endif" );
        return True

    def writeMakefile( this, printDebug = True ):
        return

    def writeUnitTest( this ):
        print "Writing unit test\n"
        testGen = stuTestGenerator.StuTestGenerator( this.className )
        testGen.addHeader( "<iostream>" )
        testGen.addHeader( "<string>" )
        testGen.addHeader( "<boost/lexical_cast.hpp>" )
        testGen.addHeader( "\"" + str(this.className) + ".h\"" )
        testGen.writeUnitTest()
        return
        
    # a simple join screws up with blank arguments
    def makeArgumentList( this, argumentList, printDebug = True ):
        if printDebug and this.doNotify:
            print "Notify: entered makeArgumentList"
        argumentString = ""
        for argument in argumentList:
            if( len( argument.strip() ) > 0 ):
                argumentString += str(argument) + ","

        if printDebug and this.doNotify:
            print "Notify: makeArgumentList returning: " + argumentString[0:len(argumentString) - 1]
            
        return argumentString[0:len(argumentString) - 1]
                
    def getClassName( this, printDebug = True ):
        return this.className

    def getClassMethods( this, printDebug = True ):
        return this.methods

    def getHeaders( this, printDebug = True ):
        return this.headers

    def getClassName( this, printDebug = True ):
        return this.className
