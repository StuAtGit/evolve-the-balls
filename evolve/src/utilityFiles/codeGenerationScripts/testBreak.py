
list = []
list.append( 1 )
list.append( 2 )
list.append( 3 )
for i in xrange( 4, 0, -1 ):
    print "i: " + str(i)
    for j in list:
        print "j: " + str(j)
        if( j == 2 ):
            try:
                break
            except:
                pass

print "hello"
