AC_INIT([evolve], [1.0], [stu26code@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CXX
AC_PROG_LIBTOOL
##Makefiles
AC_CONFIG_FILES([Makefile src/Makefile src/utilityFiles/logger/Makefile src/commonAspect/Makefile src/utilityFiles/timer/Makefile src/utilityFiles/matrix/Makefile src/utilityFiles/readWriteLock/Makefile src/utilityFiles/mutex/Makefile src/system/lSystem/Makefile src/utilityFiles/commands/Makefile src/utilityFiles/socketMethods/Makefile src/server/commandHandler/Makefile src/server/systemCommandHandler/Makefile src/interpreter/lSystemInterpreter/Makefile src/server/interpreterCommandHandler/Makefile src/server/Makefile src/physicsEngine/Makefile src/worldObject/Makefile src/worldObject/staticSphereObject/Makefile src/worldObject/staticWorldObject/Makefile src/worldObject/lSystemWorldObject/Makefile src/ogreRenderer/ogreGDKCodec/Makefile src/ogreRenderer/Makefile])

#Utility Files
AC_SUBST(logger,[/src/utilityFiles/logger/Logger.o])
AC_SUBST(logger_obj,[[\$\(top_srcdir\)/src/utilityFiles/logger/Logger.o]])
AC_SUBST(logger_inc,[/src/utilityFiles/logger/])
AC_SUBST(common_aspect_obj,[[\$\(top_srcdir\)/src/commonAspect/CommonAspect.o]])
AC_SUBST(common_aspect_incs,[[-I\$\(top_srcdir\)/src/commonAspect]])
AC_SUBST(ogre_inc,[/src/ogrenew/OgreMain/include])
AC_SUBST(utility_inc,[/src/utilityFiles/])
AC_SUBST(commands,[/src/utilityFiles/commands/Commands.o])
AC_SUBST(commands_inc,[/src/utilityFiles/commands/])
AC_SUBST(timer,[/src/utilityFiles/timer/Timer.o])
AC_SUBST(timer_inc,[/src/utilityFiles/timer/])
AC_SUBST(mutex,[/src/utilityFiles/mutex/Mutex.o])
AC_SUBST(mutex_inc,[/src/utilityFiles/mutex/])
AC_SUBST(socket_methods,[/src/utilityFiles/socketMethods/SocketMethods.o])
AC_SUBST(socket_methods_inc,[/src/utilityFiles/socketMethods/])
AC_SUBST(read_write_lock,[[\$\(top_srcdir\)/src/utilityFiles/readWriteLock/ReadWriteLock.o]])
AC_SUBST(read_write_lock_inc,[[-I\$\(top_srcdir\)/src/utilityFiles/readWriteLock/]])
AC_SUBST(matrix_inc,[[-I\$\(top_srcdir\)/src/utilityFiles/matrix/]])

#lsystem files
AC_SUBST(lsystem_generic_obj,[[\$\(top_srcdir\)/src/system/lSystem/LSystemGeneric.o]])
AC_SUBST(lsystem_incs,[[-I\$\(top_srcdir\)/src/system/\ -I\$\(top_srcdir\)/src/system/lSystem]])
AC_SUBST(lsystem_generic_interpreter_obj,[[\$\(top_srcdir\)/src/interpreter/lSystemInterpreter/LSystemGenericInterpreter.o]])
AC_SUBST(lsystem_incremental_interpreter_obj,[[\$\(top_srcdir\)/src/interpreter/lSystemInterpreter/LSystemIncrementalInterpreter.o]])
AC_SUBST(lsystem_interpreter_incs,[[-I\$\(top_srcdir\)/src/interpreter\ -I\$\(top_srcdir\)/src/interpreter/lSystemInterpreter/]])
AC_SUBST(interpreter_incs,[[-I\$\(top_srcdir\)/src/interpreter]])

#server command handlers (server is built, but not exposed to other components via one of these variables)
AC_SUBST(command_handler_obj,[[\$\(top_srcdir\)/src/server/commandHandler/CommandHandler.o]])
AC_SUBST(command_handler_incs,[[-I\$\(top_srcdir\)/src/server/commandHandler/]])
AC_SUBST(system_command_handler_obj,[[\$\(top_srcdir\)/src/server/systemCommandHandler/SystemCommandHandler.o]])
AC_SUBST(system_command_handler_incs,[[-I\$\(top_srcdir\)/src/server/systemCommandHandler/]])
AC_SUBST(interpreter_command_handler_obj,[[\$\(top_srcdir\)/src/server/interpreterCommandHandler/InterpreterCommandHandler.o]])
AC_SUBST(interpreter_command_handler_incs,[[-I\$\(top_srcdir\)/src/server/intepreterCommandHandler/]])

#physics engine
AC_SUBST(physics_engine_obj,[[\$\(top_srcdir\)/src/physicsEngine/PhysicsEngine.o]])
AC_SUBST(physics_engine_incs,[[-I\$\(top_srcdir\)/src/physicsEngine/]])

#world objects
AC_SUBST(world_object_obj,[[\$\(top_srcdir\)/src/worldObject/WorldObject.o]])
AC_SUBST(world_object_incs,[[-I\$\(top_srcdir\)/src/worldObject/]])
AC_SUBST(static_world_object_obj,[[\$\(top_srcdir\)/src/worldObject/staticWorldObject/StaticWorldObject.o]])
AC_SUBST(static_world_object_incs,[[-I\$\(top_srcdir\)/src/worldObject/staticWorldObject]])
AC_SUBST(static_sphere_object_obj,[[\$\(top_srcdir\)/src/worldObject/staticSphereObject/StaticSphereObject.o]])
AC_SUBST(static_sphere_object_incs,[[-I\$\(top_srcdir\)/src/worldObject/staticSphereObject]])
AC_SUBST(lsystem_world_object_obj,[[\$\(top_srcdir\)/src/worldObject/lSystemWorldObject/LSystemWorldObject.o]])
AC_SUBST(lsystem_world_object_incs,[[-I\$\(top_srcdir\)/src/worldObject/lSystemWorldObject]])


#Rendering objects
AC_SUBST(ogre_gdk_codec_obj,[[\$\(top_srcdir\)/src/ogreRenderer/ogreGDKCodec/OgreGDKCodec.o]])
AC_SUBST(ogre_gdk_codec_incs,[[-I\$\(top_srcdir\)/src/ogreRenderer/ogreGDKCodec/]])

#common includes and external libraries
AC_SUBST(standard_objs,[[\$\(top_srcdir\)/src/utilityFiles/logger/Logger.o\ \$\(top_srcdir\)/src/utilityFiles/timer/Timer.o\ \$\(top_srcdir\)/src/commonAspect/CommonAspect.o\ \$\(top_srcdir\)/src/utilityFiles/commands/Commands.o]])
AC_SUBST(standard_flags,[[-Wall\ -g\ -fPIC]])
AC_SUBST(standard_incs,[[-I\$\(top_srcdir\)/src/utilityFiles/logger/\ -I\$\(top_srcdir\)/src/commonAspect/\ -I\$\(top_srcdir\)/src/utilityFiles/timer/\ -I\$\(top_srcdir\)/src/utilityFiles/commands/\ -I\$\(top_srcdir\)/src/utilityFiles/\ -I/usr/local/include/OGRE/\ -I/usr/local/include/OGRE/GLX/\ -I/usr/local/include/OIS/]])
AC_SUBST(ode_flags,[[-lode]])
AC_SUBST(ogre_flags,[[-L/usr/local/lib/OGRE/\ -lOgreMain]])
AC_SUBST(ois_flags,[[-lOIS]])


#****************** Autoscan checks ************************************
# Checks for header files.
AC_PATH_X
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stdlib.h string.h sys/socket.h sys/time.h unistd.h OGRE/Ogre.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_TYPE_SIGNAL
AC_CHECK_FUNCS([gettimeofday memset socket sqrt strerror])

AC_OUTPUT
