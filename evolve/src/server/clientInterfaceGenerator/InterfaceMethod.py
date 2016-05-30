import os, sys, re

#err... I don't like the names here
class InterfaceMethod:
    command = ""
    interfaceName = ""
    requests = []
    replyType = dict();
    replyOrder = dict();

    def __init__ ( this ):
        this.command = ""
        this.requests = []
        this.replyType = dict()
        this.interfaceName = ""
        return

    def setInterfaceName( this, interfaceName ):
        this.interfaceName = interfaceName
        return
    
    def setCommand( this, command ):
        this.command = command
        return

    def addRequest( this, request ):
        this.requests.append( request )
        return

    def addReplyType( this, reply, reply_type ):
        this.replyType[reply] = reply_type
        return;

    def addReplyOrder( this, reply, reply_order ):
        this.replyOrder[reply] = reply_order
        return

    def getBaseType( this,  annotated_type ):
        tokens = re.split( r'[\s]', annotated_type )
        base_type = ""
        for token in tokens:
            token = token.strip();
            #I used to have unsigned in here, but we don't want it (it's important for getting the templated types right!)
            if token == "const":
                continue
            else:
                if token.endswith( "&"):
                    if len(base_type) > 0:
                        base_type += " "
                    base_type +=  str(token)[0:len(token) - 1]
                else:
                    if len(base_type) > 0:
                        base_type += " "
                    base_type += token;

        return base_type
    
    #basically types that the lexical_cast boost method can handle - by no means an exhaustive list
    #(due to laziness)
    def isSimpleType( this, reply_type ):
        reply_type = reply_type.strip()
        base_type = this.getBaseType( reply_type )
        if( base_type == "char" ):
            return True
        elif( base_type == "double" ):
            return True
        elif( base_type == "float" ):
            return True
        elif( base_type ==  "int" ):
            return True
        elif( base_type ==  "unsigned int" ):
            return True
        elif( base_type == "long" ):
            return True
        elif( base_type == "long long" ):
            return True
        elif( base_type == "unsigned long" ):
            return True
        elif( base_type == "string" ):
            return True
        elif( base_type == "char[]" ):
            return True
        elif( base_type == "char*" ):
            return True
        elif( base_type == "unsigned char*" ):
            return True
        else:
            return False

    def parseSimpleMapType( this, reply_type, types ):
#        del types[:]
        reply_type = this.getBaseType( reply_type.strip() )
        tokens  = re.split( r'[<>,]', reply_type )
        #trim each token (to get rid of tokens of just spaces vs tokens with spaces inbetween like unsigned int)
        for token in tokens:
            token = token.strip()
        #remove empty tokens
        for i in xrange( len(tokens) ):
            try:
                tokens.remove( "" )
            except:
                continue
        for token in tokens:
            print token
        if len( tokens ) == 3:
            if tokens[0] == "map" :
                if this.isSimpleType( tokens[1] ) and this.isSimpleType( tokens[2] ) :
                    types.append( tokens[1].strip() )
                    types.append( tokens[2].strip() )
                    return True
                else:
                    print "Warning: InterfaceMethod.isSimpleMapType, can only currently handle maps templated with basic types, not: " + tokens[1].strip() + " or " + tokens[2].strip()
                    return False
            else:
                print "not a map type"
                return False
            return False
        return False
    
    def parseSimpleVectorType( this, reply_type, types ):
        reply_type = this.getBaseType( reply_type.strip() )
        tokens = re.split( r'[<>]', reply_type )
        for token in tokens:
            token = token.strip()
        #remove any empty tokens
        for i in xrange( len(tokens) ):
            try:
                tokens.remove( "" )
            except:
                continue
        if len( tokens ) == 2:
            if tokens[0] == "vector":
                if this.isSimpleType( tokens[1] ) :
                    types.append( tokens[1].strip() )
                    return True
                else:
                    print "Warning: InterfaceMethod.parseSimpleVectorType, can only currently handle vectors templated with basic types"
                    return False
            else:
                print "not a vector type"
                return False
            return False
        return False
        
    def writeMethod( this, file_handle ):
        functionName = "handle_" + str(this.command)
        # generate argument list: need an argument for every reply we need to send (along with the type)
        argumentList = ""
        for key in this.replyType.keys():
            argumentList += this.replyType[key] + " " + key + ","
        file_handle.write( "    bool " + functionName + " ( int sock, const string& id," + argumentList + "  vector<string>& replies, bool printDebug )\n" )
        file_handle.write( "    {\n" )
        file_handle.write( "        this->logger.print( \"Notify: entered " + this.interfaceName + "::" + functionName + " \");\n" )
        file_handle.write( "        vector<string> commandSequence;\n" );
        file_handle.write( "        commandSequence.push_back( \"" + this.command + "?\" + id );\n" )
       

        #we cache the code, because the dom nodes don't necessarily reflect the sequential order in the file
        #Thus, we generate the code, then spit it out in the order specified by the order attribute
        replyCode = dict()
        for key in this.replyType.keys():
            types = []
            if this.isSimpleType( this.replyType[key] ):
                replyCode[ this.replyOrder[key] ] = "        commandSequence.push_back( Utility::toString<" + this.getBaseType( this.replyType[key] ) + ">( " + key + ", success, printDebug ) );\n" ;
            elif this.parseSimpleMapType( this.replyType[key], types ) :
                if len(types) != 2 :
                    print "maps have two types, not: " + str( len( types ) )
                    continue
                replyCode[ this.replyOrder[key] ] = "        commandSequence.push_back( Utility::mapToString<" + this.getBaseType( types[0] ) + "," + this.getBaseType( types[1] ) + ">( " + key + ", success ) );\n";
            elif this.parseSimpleVectorType( this.replyType[key], types ) :
                if len(types) != 1:
                    print "vectors have one templated type, not: " + str( len(types) )
                replyCode[ this.replyOrder[key] ] = "        commandSequence.push_back( Utility::vectorToString<" + this.getBaseType( types[0] )+ ">( " + key + " , success ) );\n";
            else:
                print "Warning: in InterfaceMethod::writeMethod, could not handle type: " + this.replyType[key]+ " for variable: " + key

        orderedKeys = []
        #keys() doesn't put stuff in order
        for key in replyCode.keys():
            orderedKeys.append( key )
        orderedKeys.sort()

        if len(orderedKeys) > 0:
            file_handle.write( "        bool success = false;\n" )
            
        for key in orderedKeys:
            print "order is: " + str(key)
            file_handle.write( replyCode[key] )
        
        file_handle.write( "        return this->socketMethods.sendCommandSequence( sock, commandSequence, replies, printDebug );\n" );
        file_handle.write( "    }\n" )
        return
