bin_PROGRAMS = testCommonAspect
testCommonAspect_SOURCES = CommonAspect.cc testCommonAspect.cc
#You can't have the dollar in AC_SUBST, or the whole variable gets lost
#proving, with finality, what a POS language M4 is. Or maybe just autotools...
testCommonAspect_LDADD = @logger_obj@ 
AM_CXXFLAGS = -Wall -g -I$(top_srcdir)@logger_inc@ 
