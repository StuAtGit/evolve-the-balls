bin_PROGRAMS = testLSystemGenericInterpreter testLSystemIncrementalInterpreter

testLSystemGenericInterpreter_SOURCES = LSystemGenericInterpreter.cc testLSystemGenericInterpreter.cc
testLSystemGenericInterpreter_LDADD = @standard_objs@ @lsystem_generic_obj@

testLSystemIncrementalInterpreter_SOURCES = LSystemIncrementalInterpreter.cc testLSystemIncrementalInterpreter.cc
testLSystemIncrementalInterpreter_LDADD = @standard_objs@ @lsystem_generic_obj@ @lsystem_generic_interpreter_obj@

AM_CXXFLAGS = @standard_flags@ @standard_incs@ @lsystem_incs@ @interpreter_incs@ @matrix_inc@
