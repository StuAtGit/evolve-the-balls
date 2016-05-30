bin_PROGRAMS = testCommandHandler
lib_LTLIBRARIES = libCommandHandler.la
testCommandHandler_SOURCES = testCommandHandler.cc
testCommandHandler_LDADD = @standard_objs@ $(top_srcdir)@mutex@ $(top_srcdir)@socket_methods@ @read_write_lock@ libCommandHandler.la
libCommandHandler_la_SOURCES = CommandHandler.cc
AM_CXXFLAGS = @standard_flags@ -lpthread -ldl -fPIC -rdynamic @standard_incs@ -I$(top_srcdir)@mutex_inc@ -I$(top_srcdir)@socket_methods_inc@ @read_write_lock_inc@
