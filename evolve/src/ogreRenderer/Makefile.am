bin_PROGRAMS = testRenderer
testRenderer_SOURCES = testRenderer.cc OgreRenderer.cc OISFrame.cc StandardCamera.cc 
testRenderer_LDADD = @standard_objs@ @physics_engine_obj@ @world_object_obj@ @ogre_gdk_codec_obj@ @static_world_object_obj@ @static_sphere_object_obj@ @lsystem_world_object_obj@ @lsystem_generic_obj@ @lsystem_generic_interpreter_obj@ @lsystem_incremental_interpreter_obj@ @read_write_lock@ -ldl -lpthread

AM_CXXFLAGS = @standard_flags@ @standard_incs@ @physics_engine_incs@ @ogre_flags@ @ode_flags@ @ois_flags@ @world_object_incs@ @ogre_gdk_codec_incs@ @static_world_object_incs@ @static_sphere_object_incs@ @interpreter_incs@ @lsystem_incs@ @matrix_inc@ @lsystem_interpreter_incs@ @lsystem_world_object_incs@ @read_write_lock_inc@ `pkg-config --cflags --libs gtk+-2.0` -DGTK_MULTIHEAD_SAFE=1

