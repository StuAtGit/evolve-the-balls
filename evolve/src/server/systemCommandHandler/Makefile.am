bin_PROGRAMS = testSystemCommandHandler
lib_LTLIBRARIES = libSystemCommandHandler.la
testSystemCommandHandler_SOURCES = testSystemCommandHandler.cc
testSystemCommandHandler_LDADD = @standard_objs@ $(top_srcdir)@mutex@ $(top_srcdir)@socket_methods@ @read_write_lock@ @lsystem_generic_obj@ @command_handler_obj@ libSystemCommandHandler.la
libSystemCommandHandler_la_SOURCES = SystemCommandHandler.cc
AM_CXXFLAGS = @standard_flags@ -lpthread -ldl -fPIC -rdynamic @standard_incs@ -I$(top_srcdir)@mutex_inc@ -I$(top_srcdir)@socket_methods_inc@ @read_write_lock_inc@ @lsystem_incs@ @command_handler_incs@
