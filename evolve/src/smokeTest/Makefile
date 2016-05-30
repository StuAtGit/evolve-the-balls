include ../GlobalMakeSettings.mk

TEST=smokeTest
TEST_CC=smokeTest.cc

CCFLAGS += $(GdkFlags)

#also depends on libRender from utility files
TEST_DEPENDS = $(LOGGER) $(TIMER) $(SOCKETMETHODS) $(COMMANDS) \
			   $(COMMONASPECT) $(Ogre) $(ODE) $(Lib_Ois)\
			   $(PhysicsEngine) $(WorldObject)\
			   $(StaticWorldObject) $(LibOgreRenderer)\
			   $(LSYSTEMGENERIC) $(LSYSTEMGENERICINTERPRETER)\
			   $(StaticSphereObject)

LIB = -L./ -lode -lOgreRenderer 

all:$(TEST)

$(TEST):$(TEST_CC) $(TEST_DEPENDS)
	$(CC) $(CCFLAGS) $(INCLUDE) $(TEST_DEPENDS) $(LIB) $(TEST_CC) -o $(TEST)

include ../GlobalMakeTargets.mk

.PHONY:clean
.PHONY:clean_all

clean:
	rm -f $(TEST)
	rm -f *.o
	rm -f *.so

clean_all:
	rm -f $(TEST)
	rm -f $(TEST_DEPENDS)
	rm -f *.o
	rm -f *~
