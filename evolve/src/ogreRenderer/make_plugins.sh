#!/bin/sh

OGRE_DIRECTORY=/usr/local/lib/OGRE
ln -s $OGRE_DIRECTORY/Plugin_BSPSceneManager.so
ln -s $OGRE_DIRECTORY/Plugin_OctreeSceneManager.so
ln -s $OGRE_DIRECTORY/Plugin_ParticleFX.so
ln -s $OGRE_DIRECTORY/RenderSystem_GL.so

OGRE_SRC=$HOME/ogrenew/

ln -s $OGRE_SRC/Samples/Media
