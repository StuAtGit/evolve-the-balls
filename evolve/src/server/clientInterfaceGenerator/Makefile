include ../../GlobalMakeSettings.mk

TEST=testLSystemInterfaceHandlers
TEST_CC=testLSystemInterfaceHandlers.cc
TEST_DEPENDS=$(LOGGER) $(COMMONASPECT) $(TIMER) $(SOCKETMETHODS) $(COMMANDS) $(LSYSTEMINTERFACEHANDLERS_HEADER)

$(TEST):$(TEST_CC) $(TEST_DEPENDS)  
	$(CC) $(CCFLAGS) $(TEST_CC) -lpthread $(TEST_DEPENDS) -o $(TEST)

include ../../GlobalMakeTargets.mk

clean:
	rm -f $(TEST) 
	rm -f $(OBJ)
	rm -f $(TEST_DEPENDS)
