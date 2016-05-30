#class to write makefiles for the evolve project
import os, sys

class makefileGenerator:
    projectHome = ""
    testName = ""
    testFilename = ""
    testDepedencies = []
    depLocations = []
    compiler = "g++"
    compilerFlags = "-Wall -O2 -g"
    includes = ""
    objectName = ""
    objCFilename = ""
    objHFilename = ""

    def __init__(this, objectName, projectHome ):
        this.objectName = objectName
        this.objCFilename = str(objectName) + ".cc"
        this.objHFilename = str(objectName) + ".h"
        this.testName = "test" + str(objectName)
        this.testFilename = "test" + str(objectName) + ".cc"
        this.projectHome = projectHome

    # configuration file contains project location, directory structure,
    # list of objects (in their locations)
    def readConfigFile(this, filename):
        return
    
    def writeMakefile( this, location ):
        return

    def setProjectHome( this, projectHome ):
        this.projectHome = projectHome
        return
    
    def setTestName( this, testName ):
        this.testName = testName
        return
    
    def setTestFilename( this, testFilename ):
        this.testFileName = testFilename
        return
    
    def setDependencies( this, dependencies, depLocations ):
        this.testDependencies = dependencies
        this.depLocations = depLocations
        return

    def setCompiler( this, compilerName ):
        this.compiler = compilerName
        return

    def setCompilerFlags( this, compilerFlags ):
        this.compilerFlags = compilerFlags
        return

    def setIncludes( this, includes ):
        this.includes = includes
        return

    def setObjectName( this, objectName ):
        this.objectName = objectName
        return

    def setObjCFilename( this, objCFilename ):
        this.objCFilename = objCFilename
        return

    def setObjHFilename( this, objHFilename ):
        this.objHFilename = objHFilename
        return

    
    
