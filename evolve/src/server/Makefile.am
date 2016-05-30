bin_PROGRAMS = testServer mommaServer

testServer_SOURCES = Server.cc testServer.cc
testServer_LDADD = @standard_objs@ $(top_srcdir)@mutex@ $(top_srcdir)@socket_methods@ @read_write_lock@ @command_handler_obj@ -ldl -lpthread

mommaServer_SOURCES = Server.cc MommaServer.cc
mommaServer_LDADD = @standard_objs@ $(top_srcdir)@mutex@ $(top_srcdir)@socket_methods@ @read_write_lock@ @lsystem_generic_obj@ @lsystem_generic_interpreter_obj@ @command_handler_obj@ -ldl -lpthread

AM_CXXFLAGS = @standard_flags@ @standard_incs@ @command_handler_incs@ -I$(top_srcdir)@mutex_inc@ -I$(top_srcdir)@socket_methods_inc@ @read_write_lock_inc@ @lsystem_incs@ @lsystem_interpreter_incs@ @interpreter_command_handler_incs@ @system_command_handler_incs@ -rdynamic
