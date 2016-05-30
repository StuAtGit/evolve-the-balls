bin_PROGRAMS = testOgreGDKCodec
testOgreGDKCodec_SOURCES = testOgreGDKCodec.cc OgreGDKCodec.cc 
testOgreGDKCodec_LDADD = @standard_objs@
AM_CXXFLAGS = @standard_flags@ @standard_incs@ @ogre_flags@ `pkg-config --cflags --libs gtk+-2.0` -DGTK_MULTIHEAD_SAFE=1
