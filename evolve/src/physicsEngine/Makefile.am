bin_PROGRAMS = testPhysicsEngine

testPhysicsEngine_SOURCES = testPhysicsEngine.cc PhysicsEngine.cc
testPhysicsEngine_LDADD = @standard_objs@ @read_write_lock@ -ldl -lpthread

AM_CXXFLAGS = @standard_flags@ @standard_incs@ @ode_flags@ @matrix_inc@ @read_write_lock_inc@
