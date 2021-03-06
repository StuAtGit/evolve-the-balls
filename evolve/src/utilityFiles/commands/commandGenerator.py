import os, errno, sys, xml.dom.pulldom, xml.dom.minidom

inputFileName = "Commands.xml"
cHeaderOutputFileName = "Commands.h"
cImplOutputFileName = "Commands.cc"
phpOutputFileName = "Commands.php"
javaOutputFileName = "Commands.java"

def writeCFiles( allNodes, cHeaderFile, cImplFile ):
	cHeaderFile.write( "#ifndef COMMANDS_STU_H\n" );
	cHeaderFile.write( "#define COMMANDS_STU_H\n" );
	cHeaderFile.write( "#include <string>\n" );
	cHeaderFile.write( "using namespace std;\n\n" );
	cHeaderFile.write( "/**\n" );
	cHeaderFile.write( "* Singleton class that contains command constants (and maybe one or two others *duck*), auto-generated by commandGenerator.py from Commands.xml\n" );
	cHeaderFile.write( "* Eventually, a hierarchy of contants may be created (but the work is not really justifiable at this point\n" );
	cHeaderFile.write( "* The command contants are the commands used for the various clients & servers to communicate\n" );
	cHeaderFile.write( "*/\n" );
	cHeaderFile.write( "class Commands\n" );
	cHeaderFile.write( "{\n" );
	cHeaderFile.write( "public:\n" );
	cHeaderFile.write( "    static Commands* getInstance();\n" );
	for node in allNodes:
		if( node.nodeName != "EvolveCommands" ):
			cHeaderFile.write( "    string " + node.getAttribute( "id" ).strip() + ";\n" )
	cHeaderFile.write( "protected:\n" );
	#cHeaderFile.write( "Commands::Commands()\n");
	#cHeaderFile.write( "Commands()\n" );

	cHeaderFile.write( "    Commands();\n" );
	cHeaderFile.write( "private:\n" ); 
	cHeaderFile.write( "    static Commands* instance;\n" );
	cHeaderFile.write( "};\n" );
	cHeaderFile.write( "#endif\n" );
	cImplFile.write( "#ifndef COMMANDS_STU_CC\n" );
	cImplFile.write( "#define COMMANDS_STU_CC\n" );
	cImplFile.write( "#include \"Commands.h\"\n\n" );
	cImplFile.write( "Commands* Commands::instance = NULL;\n" );
	cImplFile.write( "Commands::Commands()\n");
	cImplFile.write( "{\n" );
	for node in allNodes:
		if( node.nodeName != "EvolveCommands" ):
			cImplFile.write("    this->" + node.getAttribute( "id" ).strip() + " = \"" + node.firstChild.data.strip() + "\";\n" );
	cImplFile.write( "}\n" );
	cImplFile.write( "Commands* Commands::getInstance()\n{\n" );
	cImplFile.write( "    if( instance == NULL )\n" );
	cImplFile.write( "    {\n" );
	cImplFile.write( "        instance = new Commands();\n");
	cImplFile.write( "    }\n" );
	cImplFile.write( "    return instance;\n" );
	cImplFile.write( "}\n" );
	cImplFile.write( "#endif\n" );

def writePhpFile( allNodes, phpFile ):
	phpFile.write( "<?\n" )
	phpFile.write( "class Commands\n" )
	phpFile.write( "{\n" )
	for node in allNodes:
		if( node.nodeName != "EvolveCommands" ):
			phpFile.write( "    var $" + node.getAttribute( "id" ).strip() + " = \"" + node.firstChild.data.strip() + "\";\n" );
	phpFile.write( "}\n" )
	phpFile.write( "?>\n" )

def writeJavaFile( allNodes, javaFile ):
	javaFile.write( "class Commands\n" )
	javaFile.write( "{\n" )
	for node in allNodes:
		if( node.nodeName != "EvolveCommands" ):
			javaFile.write( "    public static String " + node.getAttribute( "id" ).strip() + " = \"" + node.firstChild.data.strip() + "\";\n" ); 
	javaFile.write( "}\n" )

openSuccess = 1

try: file = open( inputFileName, 'r', -1 )
except IOError:
	print "file not found: " + inputFileName
	print "Exiting"
	openSuccess = 0;

try: cHeaderFile = open( cHeaderOutputFileName, 'w', -1 )
except IOError:
	print "file not found: " + inputFileName
	print "Exiting"
	openSuccess = 0;

try: cImplFile = open( cImplOutputFileName, 'w', -1 )
except IOError:
	print "file not found: " + inputFileName
	print "Exiting"
	openSuccess = 0;

try: phpFile = open( phpOutputFileName, 'w', -1 )
except IOError:
	print "file not found: " + inputFileName
	print "Exiting"
	openSuccess = 0;

try: javaFile = open( javaOutputFileName, 'w', -1 )
except IOError:
	print "file not found: " + inputFileName
	print "Exiting"
	openSuccess = 0;


except IOError:
	print "file not found: " + inputFileName
	print "Exiting"
	openSuccess = 0;

if openSuccess == 1:
	doc = xml.dom.minidom.parse( file )
	lSystemServerCommandNodes = doc.getElementsByTagName( "LSystemServerCommand" )
	lSystemServerRequestNodes = doc.getElementsByTagName( "LSystemRequest" )
	lSystemServerMisc = doc.getElementsByTagName( "LSystemServerMisc" )
	lSystemInterpreterCommandNodes = doc.getElementsByTagName( "LSystemInterpreterCommand" )	
	allNodes = doc.getElementsByTagNameNS( "*", "*" )
	writeCFiles( allNodes, cHeaderFile, cImplFile )
	writePhpFile( allNodes, phpFile )
	writeJavaFile( allNodes, javaFile )
	
