This is an export from some truly very old code I wrote and put up on google code, back before github.

My old notes:

Overview
=========

Hmm... not so much anymore. Moved onto relearning that whole web thing:

http://www.shareplaylearn.net/
(Update - that was actually my old site, nowadays it's https://www.shareplaylearn.com )

Been a few years.. (5-6).. odd what's changed and what hasn't..

This is now mainly offsite storage of the site's code. Nothing exciting, just learning AppEngine? and GWT (which is exciting, if my code is not :P). Note that the website code is NOT covered by the same license as the project code.

Keeping this stuff just coz:

Create a 3D world of evolving creatures. (yet another one)

Runs evolver, interpreter, physics as separate processes that talk to each other via sockets. Client does rendering, and possibly physics. Hope to factor out physics to separate process.

Currently the system evolved is an lsystem, and it is interpreted as a sequence of points in 3D space.

Note that the Source tab is now irrelevant: all code drops are done via downloads, and the tree is no longer updated. This is due to the fact that I use git on my local machine for version control, and it is rather awkward to try to use two different version control systems for the same code.

Works on linux with Ogre, ODE, OIS, and boost.

For more info see:

[evolve blog intro](http://evolve-the-balls.blogspot.com/2007/06/intro.html) [most recent blog](http://evolve-the-balls.blogspot.com/)

gmail

    -stu26code -not checked terribly often 

Building
=========

You should have: Boost, ODE, OGRE, OIS, and GDK (gdk dev headers) installed.

You need to download, build, and install OGRE and OIS. I use Eihort (Ogre 1.4.5). Earlier versions won't work. At the very least, you should install OGRE, OIS, ode, and they're respective development headers (the -dev packages). Note the build system currently assumes ogre is installed using the default /usr/local/ install prefix not /usr or /. Once I learn more about autools, I may be able to work around that. Until then, you can update the configure.ac ogre_inc variable if you have it in different places. Note that Ogre should be configured with freeimage disabled.

./configure --disable-freeimage.

OGRE/OIS should be installed in usr/include (or update configure.ac accordingly). The renderer needs plugins in the local directory. There is a script make_plugins.sh in /src/ogreRenderer Edit this script to have it point at the correct locations (currently it assumes ogre libs installed in /usr/local, and source tarball in $HOME)

Then do a

./configure && make

from the evolve directory.

cd src/ogreRenderer

./makePlugins.sh

./testOgreRenderer

(to see ?pretty? graphics)

cd src/server

./mommaServer

Runs the server (currently not hooked into ogreRenderer). Smoke test used to hook into the server, but now this is being phased into the ogreRender test.

This project will probably eventually use QT as well.

You may want to install: nvidia-cg.

You do not want: freeimage (I supply my own codec handler in OGRE using GDK)

nvidia-cg is for OGRE, and OGRE can be configured to not use it with --disable-cg. I don't use cg in my code, so it should be fine to not have it (I didn't have it for awhile), as long as you get OGRE to build without it.

For more about the dependencies of my dependencies, see the respective websites for those projects.
Links
-------
Basics:

 - [boost home page](http://www.boost.org/)
 - [boost download page](http://sourceforge.net/project/showfiles.php?group_id=7586)
 - cygwin - never tried it with this, but a better place to start than Visual Studio ;) 

Client stuff:

  - [Ogre](http://www.ogre3d.org/)
  - [nvidia-cg](http://developer.nvidia.com/object/cg_toolkit.html#downloads) - (optional)
  - [OIS](http://sourceforge.net/project/showfiles.php?group_id=149835)
  - [ODE](http://www.ode.org/)
  - [GTK](http://www.gimp.org/~tml/gimp/win32/downloads.html) 

And someday, we may just need this to get gtk for the client:

  - [GTK](http://gladewin32.sourceforge.net/modules/wfdownloads/viewcat.php?cid=14) 

All this stuff should be available via a decent [package manager](http://gentoo-wiki.com/MAN_emerge). So you really only need to download & install this stuff if you intend build on windows. (You brave person you)

OGRE and OIS should NOT be installed via your package manager. I don't configure OGRE to use freeimage library for one, and for two, it keeps things clean. That said, you can do it if you want, and if you set it up right, everything should work.

Misc:
[ODE tutorial](http://gpwiki.org/index.php/ODE_Object_tutorial) - seems decent from glancing at it. Especially since I added some stuff :) 

Version control
===============
I use git locally, and upload tarballs periodically. Makes life much nicer. I can't check-in my git repository (too big). So you need to make your own. Which is how it's intended to be anyways
