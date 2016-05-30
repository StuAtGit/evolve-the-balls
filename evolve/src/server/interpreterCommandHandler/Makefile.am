bin_PROGRAMS = testInterpreterCommandHandler
lib_LTLIBRARIES = libInterpreterCommandHandler.la
testInterpreterCommandHandler_SOURCES = testInterpreterCommandHandler.cc
testInterpreterCommandHandler_LDADD = @standard_objs@ $(top_srcdir)@mutex@ $(top_srcdir)@socket_methods@ @read_write_lock@ @lsystem_generic_interpreter_obj@ @command_handler_obj@ libInterpreterCommandHandler.la
libInterpreterCommandHandler_la_SOURCES = InterpreterCommandHandler.cc
AM_CXXFLAGS = @standard_flags@ -lpthread -ldl -fPIC -rdynamic @standard_incs@ -I$(top_srcdir)@mutex_inc@ -I$(top_srcdir)@socket_methods_inc@ @read_write_lock_inc@ @lsystem_incs@ @lsystem_interpreter_incs@ @command_handler_incs@ @matrix_inc@
