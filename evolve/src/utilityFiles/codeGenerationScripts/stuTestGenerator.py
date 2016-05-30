#class to generate unit test file

import os, sys

class StuTestGenerator:
    testClassName = ""
    headers = []

    def __init__( this, className ):
        this.testClassName = str(className)
        return

    def addHeaders( this, headers ):
        for header in headers:
            this.headers.append( header )
        return

    def addHeader( this, header ):
        this.headers.append( header )
        return

    def writeUnitTest( this ):
        print "In StuTestGenerator.writeUnitTest\n"
        try:
            filename = "test" + str( this.testClassName ) + ".cc"
            unitTestFile = open( filename, "wt" )
        except IOError, e:
            print "Couldn't open file: " + filename + " for writing:  \n"
            print e
            return

        for header in this.headers:
            unitTestFile.write( "#include " + header + "\n" )

        unitTestFile.write( "\n" )
        unitTestFile.write( "using namespace std;\n" )
        unitTestFile.write( "using namespace boost;\n" )
        unitTestFile.write( "\n" )
        unitTestFile.write( "int main()\n" )
        unitTestFile.write( "{\n" )
        unitTestFile.write( "    " + str( this.testClassName ) + " test;\n" )
        unitTestFile.write( "}\n" );
        return
