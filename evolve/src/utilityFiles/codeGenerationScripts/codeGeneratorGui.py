#!/usr/bin/env python

import pygtk
pygtk.require('2.0')
import gtk
import stuClassGenerator

class CodeGeneratorGui:
    headerFiles = []
    includeCheckButtons = dict()
    deleteCheckButtons = dict()
    deleteLabels = dict()
    deleteBoxes = dict()
    checkbuttonHelp = dict()
    methods = []
    addMethodButton = gtk.Button( "Add Method" )
    deleteMethodsButton = gtk.Button("Delete Selected Methods" )

    classNameEntryBox = gtk.Entry()
    developerIdEntryBox = gtk.Entry()
    methodNameEntryBox = gtk.Entry()
    methodReturnTypeEntryBox = gtk.Entry()
    methodArgumentsEntryBox = gtk.Entry()
    methodListingLabel = gtk.Label( "" )

    includeBox = gtk.VBox( True, 2 )
    classDescriptionTable = gtk.Table( 3, 1 )
    methodDisplayContainer = gtk.VBox( True, 2 )

    def writeClass( self, widget ):
        if self.classNameEntryBox.get_text().strip() == "" :
            self.showDialog( "No class name", "Warning: no class name given.\nPlease enter a class name\n and try again" )
            return
        if self.developerIdEntryBox.get_text().strip() == "":
            self.showDialog( "No developer id", "Warning: no developer id given.\nPlease enter a developer id\n and try again")
            return
            
        generator = stuClassGenerator.StuClassGenerator( self.classNameEntryBox.get_text().strip()
                                                         , self.methods, self.developerIdEntryBox.get_text().strip() )

        for headerFile in self.headerFiles:
            if self.includeCheckButtons[headerFile].get_active():
                generator.addHeader( headerFile )

        generator.writeHeaderFile()
        generator.writeImplementationFile()
        generator.writeUnitTest()
        generator.writeMakefile()
        self.showDialog( "Class written", "Wrote out header and implementation files" )
        return
    
    def deleteEvent( self, widget, event, data=None ):
        return False

    def deleteMethodsEvent( self, widget ):
        methodKeys = []
        #delete the button
        for key in self.deleteCheckButtons.keys():
            if self.deleteCheckButtons[key].get_active():
                methodKeys.append( key )
                try:
                    self.deleteCheckButtons[key].destroy()
                    del self.deleteCheckButtons[key]
                except:
                    self.showDialog( "Error Message", "Warning, error deleting check button\n For method: " + key )
                try:
                    self.deleteLabels[key].destroy()
                    del self.deleteLabels[key]
                except:
                    self.showDialog( "Error Message", "Warning, error deleting delete label\n For method: " + key )
                try:
                    self.deleteBoxes[key].destroy()
                    del self.deleteBoxes[key]
                except:
                    self.showDialog( "Error Message", "Warning, error deleting delete display box\n For method:" + key )
                
        #delete the method from the method list
        for i in xrange( len(self.methods) - 1, -1, -1 ):
            for signature in methodKeys:
                if signature == self.getSignature( self.methods[i] ) :
                    try:
                        self.methods.remove( self.methods[i] )
                        break
                    except:
                        self.showDialog( "Error Message", "Warning, error deleting method\n" + key )

        return

    def getSignature( self, method ):
        return  method['returnType'].strip() + " " + \
                          method['name'].strip() + "(" + \
                          ",".join( method['arguments'] ) + ")"
    
    def deleteDialog( self, dialog, event ):
        dialog.destroy()

    def showDialog( self, title, message ):
        dialog = gtk.Dialog( title, self.window,
                                 gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT,
                                 (gtk.STOCK_OK, gtk.RESPONSE_ACCEPT) )
        warningLabel = gtk.Label( message )
        warningLabel.show()
        dialog.vbox.pack_start( warningLabel, False, False )
        dialog.connect( "response", self.deleteDialog )
        dialog.show()

    def methodExists( self, newMethod ):
        for method in self.methods:
            if( method['name'].strip() == newMethod['name'].strip()  and
                ",".join(method['arguments']).strip() == ",".join(newMethod['arguments']).strip()
                ):
                return True

        return False
    
    def addMethodEvent( self, widget, data=None ):
        if( self.classNameEntryBox.get_text().strip() == "" ):
            self.showDialog( "Enter class name", "Please enter a class name before adding any methods" )
            return
        
        if( self.methodNameEntryBox.get_text().strip() == self.classNameEntryBox.get_text().strip()
            and self.methodReturnTypeEntryBox.get_text().strip() != "" ):
            self.showDialog( "Bad constructor", "Constructors do not have return types (leave it blank!)" )
            return
             
        
        methodArguments = [];
        methodArguments = self.methodArgumentsEntryBox.get_text().strip().split( "," )
        
        method = { 'returnType':self.methodReturnTypeEntryBox.get_text().strip(),
                   'name':self.methodNameEntryBox.get_text().strip(),
                   'arguments':methodArguments }

        if( self.methodExists( method ) ):
            self.showDialog( "Duplicate Method", "A method with that signature already exists" )
            return

        self.methods.append( method )
        methodSignature = self.methodReturnTypeEntryBox.get_text().strip() + " " + \
                          self.methodNameEntryBox.get_text().strip() + "(" + \
                          ",".join( methodArguments ) + ")"

        methodDisplayBox = gtk.HBox()
        self.deleteLabels[methodSignature] = gtk.Label( methodSignature )
        self.deleteLabels[methodSignature].show()
        methodDisplayBox.pack_start( self.deleteLabels[methodSignature] )

        self.deleteCheckButtons[methodSignature] = gtk.CheckButton( "delete" )
        self.deleteCheckButtons[methodSignature].show()
        methodDisplayBox.pack_start( self.deleteCheckButtons[methodSignature] )

        methodDisplayBox.show()
        self.deleteBoxes[methodSignature] = methodDisplayBox
        
        self.methodDisplayContainer.pack_start( methodDisplayBox, True, False )
        self.deleteMethodsButton.show()

    
    def destroy( self, widget, data=None ):
        gtk.main_quit()

    def createIncludeCheckboxes( self, headerFiles ):
        checkboxLabel = gtk.Label( "Please select the header files you wish to use:" )
        checkboxLabel.show()
        self.includeBox.pack_start( checkboxLabel, True, True, 2 )
        
        for header in self.headerFiles:
            checkbuttonBox = gtk.HBox( False, 0 )

            self.includeCheckButtons[ str(header) ] = gtk.CheckButton( str(header) )
            self.checkbuttonHelp[ str(header) ] = gtk.Button( str(header) + " Info" )

            checkbuttonBox.pack_start( self.includeCheckButtons[ str(header) ] )
            checkbuttonBox.pack_start( self.checkbuttonHelp[str(header) ] )
            
            self.includeBox.pack_start( checkbuttonBox, True, True, 2 )
            self.includeCheckButtons[ str(header) ].show()
            self.checkbuttonHelp[ str(header) ].show()
            checkbuttonBox.show()

        self.includeBox.show()
        
    def createClassDescriptionBox( self ):
        #dev id 
        developerIdBox = gtk.HBox( False, 0 )
        developerIdLabel = gtk.Label( "Developer Id: " )
        developerIdLabel.show()
        developerIdBox.pack_start( developerIdLabel, True, False, 2 )

        self.developerIdEntryBox.set_max_length( 25 )
        self.developerIdEntryBox.show()
        developerIdBox.pack_start( self.developerIdEntryBox, True, True, 2 )

        developerIdBox.show()

        #class name
        classNameBox = gtk.HBox( False, 0 )
        classNameLabel = gtk.Label( "Name of the class: " )
        classNameLabel.show()
        classNameBox.pack_start( classNameLabel, True, True, 2 )

        self.classNameEntryBox.set_max_length( 25 )
        self.classNameEntryBox.show()
        classNameBox.pack_start( self.classNameEntryBox, True, True, 2 )

        classNameBox.show()

        #add method
        methodFrame = gtk.Frame( "Add Method " )
        methodFrame.show()

        methodTable = gtk.Table( 3, 3 )

        #add method entry boxes
        self.methodReturnTypeEntryBox.set_max_length( 60 )
        self.methodReturnTypeEntryBox.show()
        methodTable.attach( self.methodReturnTypeEntryBox, 0, 1, 1, 2, gtk.EXPAND,
                            gtk.SHRINK, 5, 4 )

        self.methodNameEntryBox.set_max_length( 40 )
        self.methodNameEntryBox.show()
        methodTable.attach( self.methodNameEntryBox, 1, 2, 1, 2, gtk.EXPAND,
                            gtk.SHRINK, 5, 4)

        self.methodArgumentsEntryBox.set_max_length( 300 )
        self.methodArgumentsEntryBox.show()
        methodTable.attach( self.methodArgumentsEntryBox, 2, 3, 1, 2, gtk.EXPAND
                            | gtk.FILL, gtk.EXPAND, 5, 4)

        #add method help boxes
        methodReturnTypeLabel = gtk.Label( "Return Value" )
        methodReturnTypeLabel.show()
        methodTable.attach(  methodReturnTypeLabel, 0, 1, 0, 1, gtk.EXPAND,
                             gtk.SHRINK, 5, 4)
        
        methodNameLabel = gtk.Label( "Method Name" )
        methodNameLabel.show()
        methodTable.attach( methodNameLabel, 1, 2, 0, 1, gtk.EXPAND, gtk.SHRINK,
                            5, 4)

        methodArgumentsHelpString = "Method arguments (as a comma separated list)"
        methodArgumentsHelpString += "\nExample: int arg1, vector<int> arg2, bool arg3"
        methodArgumentsLabel = gtk.Label( methodArgumentsHelpString )
        methodArgumentsLabel.show()
        methodTable.attach( methodArgumentsLabel, 2, 3, 0, 1 )

        #add method button
        methodTable.attach( self.addMethodButton, 2, 3, 2, 3, gtk.EXPAND, gtk.SHRINK,
                            5, 4)        
        self.addMethodButton.show()
        
        methodTable.show()
        
        
        self.classDescriptionTable.attach( developerIdBox, 0, 1, 0, 1,
                                           gtk.EXPAND | gtk.FILL, gtk.SHRINK,
                                           5, 4 )
        self.classDescriptionTable.attach( classNameBox, 0, 1, 1, 2,
                                           gtk.EXPAND | gtk.FILL, gtk.SHRINK,
                                           5, 4 )
        methodFrame.add( methodTable ) 
        self.classDescriptionTable.attach( methodFrame, 0, 1, 2, 3,
                                           gtk.EXPAND | gtk.FILL, gtk.EXPAND,
                                           5, 4 )
        self.classDescriptionTable.show()
        
    def __init__(self):
        #set up window & basic event handlers
        self.window = gtk.Window( gtk.WINDOW_TOPLEVEL )
        self.window.set_title( "Stucode Generator Gui" )
        
        self.window.connect( "delete_event", self.deleteEvent )
        self.window.connect( "destroy", self.destroy )
        self.addMethodButton.connect( "clicked", self.addMethodEvent )
        self.window.set_border_width(0)

        self.globalTable = gtk.Table( 3, 2, False )

        #include checkboxes
        self.headerFiles.append( "<iostream>" )
        self.headerFiles.append( "<vector>" )
        self.headerFiles.append( "<map>" )
        self.headerFiles.append( "<string>" )
        self.headerFiles.append( "<fstream>" )
        self.headerFiles.append( "<sstream>" )
        self.headerFiles.append( "\"LSystem.h\"" )
        self.headerFiles.append( "\"Timer.h\"" )
        self.headerFiles.append( "\"Logger.h\"" )
        self.headerFiles.append( "\"Commands.h\"" )
        self.headerFiles.append( "\"Sockets.h\"" )
        self.createIncludeCheckboxes( self.headerFiles )

        #class description
        self.createClassDescriptionBox()

       
        self.globalTable.attach( self.includeBox, 0, 1, 0, 2,
                                 gtk.EXPAND | gtk.FILL, gtk.EXPAND, 5, 4 )
        self.globalTable.attach( self.classDescriptionTable, 1, 2, 1, 2,
                                 gtk.EXPAND | gtk.FILL, gtk.SHRINK, 5, 4  )
        self.globalTable.show()

        #method display box
        self.methodDisplayContainer.show()
        self.deleteMethodsButton.connect( "clicked", self.deleteMethodsEvent )
        self.methodDisplayContainer.pack_end( self.deleteMethodsButton, True, True ) 
        self.globalTable.attach( self.methodDisplayContainer, 1, 2, 2, 3,
                                 gtk.EXPAND | gtk.FILL, gtk.SHRINK, 5, 4 )
 

        #menu 
        fileMenu = gtk.Menu()
        
        writeClassItem = gtk.MenuItem( "Write Class" )
        writeClassItem.show()
        writeClassItem.connect( "activate", self.writeClass )
        fileMenu.append( writeClassItem )

        fileMenuItem = gtk.MenuItem( "File" )
        fileMenuItem.set_submenu( fileMenu )
        fileMenuItem.show()

        menuBar = gtk.MenuBar()
        menuBar.append( fileMenuItem )
        menuBar.show()

        globalBox = gtk.VBox( False, 0 )
        globalBox.pack_start( menuBar, False, False, 2 )
        globalBox.pack_end( self.globalTable, True, True, 2 )
        globalBox.show()
        
        self.window.add( globalBox )
    
        self.window.show()

    def main(self):
        gtk.main()


print __name__
if __name__ == "__main__":
    codeGeneratorGui = CodeGeneratorGui()
    codeGeneratorGui.main()
        
