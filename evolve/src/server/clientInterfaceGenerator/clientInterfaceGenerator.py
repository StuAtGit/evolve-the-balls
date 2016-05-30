import os, errno, sys, xml.dom.minidom

try:
    import InterfaceMethod
except Exception, e:
    print "Importing InterfaceMethod failed: " + str( e )
    
commandSequenceFilename="commandSequencing.xml"
commandSequenceFilename=sys.argv[1]
interfaceFilename="LSystemInterfaceHandlers.h"
interfaceFilename=sys.argv[2]
testFilename="testLSystemInterfaceHandlers.cc"
programmerId = "stu"
interfaceName = "SystemInterfaceHandlers"
interfaceName=sys.argv[3]

def openFile( filename, mode ):
    try :
        file_handle = open( filename, mode, -1 )
    except IOError, e:
        print  "Error opening file: " + str( filename )
        return False
    return file_handle

def parseDocument( xmlDocument, interfaceFile ):
    for child in xmlDocument.childNodes:
        if child.nodeName == "commandSequences":
            parseCommandSequences( child, interfaceFile )
        else:
            print "parser cannot currently handler nodes of type: " + str( child.nodeName )
    return

def parseCommandSequences( commandSequences, interfaceFile ):
    for child in commandSequences.childNodes:
        if child.nodeName == "commandSequence":
            #print "command sequence: "
            parseCommandSequence( child, interfaceFile )
        else:
            continue
            #print "parseCommandSequences does not know what to do with, skipping node  " + str( child.nodeName ) + str( child.nodeValue.strip() )
    return

def parseCommandSequence( commandSequence, interfaceFile ):
    interfaceMethod = InterfaceMethod.InterfaceMethod();
    print "New Command Sequence"
    for child in commandSequence.childNodes:
        if child.nodeName == "command":
            if child.hasChildNodes() :
                interfaceMethod.setCommand( child.firstChild.nodeValue.strip() )
                print "    Command: " + child.firstChild.nodeValue.strip()
            else :
                print "Warning: found command node without command"
        elif child.nodeName == "request":
            if child.hasChildNodes() :
                interfaceMethod.addRequest( child.firstChild.nodeValue.strip() )
                print "    Request: " + child.firstChild.nodeValue.strip()
            else :
                print "Warning: found request node without request"
        elif child.nodeName == "reply":
            if child.hasChildNodes() :
                interfaceMethod.addReplyType( child.firstChild.nodeValue.strip(), child.getAttribute( "type" ) )
                print  "    Reply: " + child.getAttribute( "type" ) + " " + child.firstChild.nodeValue.strip()
                interfaceMethod.addReplyOrder( child.firstChild.nodeValue.strip(), child.getAttribute( "order" ) )
                print  "    Reply: " + child.getAttribute( "type" ) + " " + child.firstChild.nodeValue.strip()
            else :
                print "Warning: found reply node without reply"
        elif child.nodeName == "commandSequence":
            parseCommandSequence( child, interfaceFile )
    interfaceMethod.setInterfaceName( interfaceName )
    interfaceMethod.writeMethod( interfaceFile )
    return

def printDoc( xmlDocument ):
    for child in xmlDocument.childNodes:
        if child.nodeName == "#text" and child.nodeValue.strip() != "" :
            print( str( child.parentNode.nodeName ) + ": " + str(child.nodeValue).strip() )
        if child.hasChildNodes:
            printDoc( child )
        else:
            for attr in child.attributes:
                print "attr:" + str(attr)
    return

def main():
    commandSeqFile = openFile( commandSequenceFilename, "r" ) 
    if not( commandSeqFile ):
        return
    interfaceFile = openFile( interfaceFilename, "w+" )
    if not(  interfaceFile  ) :
        return
    testFile = openFile( testFilename, "w+" )
    if not(  testFile  ) :
        return
    testFile.write( "#include \"" + interfaceFilename + "\"\n" )
    testFile.write( "\n" )
    testFile.write( "int main()\n" )
    testFile.write( "{\n" )
    testFile.write( "    " + interfaceName + " testObj;\n" )
    testFile.write( "}\n" )
    commandSeqDoc = xml.dom.minidom.parse( commandSeqFile )
    #printDoc( commandSeqDoc )
    interfaceFile.write( "#ifndef " + str(programmerId) + "_" +  interfaceName + "\n" )
    interfaceFile.write( "#define " + str(programmerId) + "_" + interfaceName + "\n" )
    interfaceFile.write( "\n" )
    interfaceFile.write( "#include <string>\n" )
    interfaceFile.write( "#include <vector>\n" )
    interfaceFile.write( "#include <map>\n" )
    interfaceFile.write( "\n" )
    interfaceFile.write( "#include <boost/lexical_cast.hpp>\n" )
    interfaceFile.write( "\n" )
    interfaceFile.write( "#include \"Logger.h\"\n" );
    interfaceFile.write( "#include \"Utility.h\"\n" );
    interfaceFile.write( "#include \"CommonAspect.h\"\n" );
    interfaceFile.write( "#include \"SocketMethods.h\"\n" );
    interfaceFile.write( "\n" )
    interfaceFile.write( "using namespace std;\n" )
    interfaceFile.write( "using namespace boost;\n")
    interfaceFile.write( "using namespace evolve;\n")
    interfaceFile.write( "\n" )
    interfaceFile.write( "class " + str( interfaceName ) + " : public CommonAspect\n" )
    interfaceFile.write( "{\n" )
    interfaceFile.write( "public:\n" )
    parseDocument( commandSeqDoc, interfaceFile )
    interfaceFile.write( "private:\n" )
    interfaceFile.write( "    SocketMethods socketMethods;\n" )
    interfaceFile.write( "};\n" )
    interfaceFile.write( "#endif\n" )
    return

main()
