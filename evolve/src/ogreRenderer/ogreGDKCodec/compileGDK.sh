#!/bin/sh

echo g++ `pkg-config --cflags --libs gtk+-2.0` -g -DGTK_MULTIHEAD_SAFE=1 testGDK.cc -o testGDK
g++ `pkg-config --cflags --libs gtk+-2.0` -g  -DGTK_MULTIHEAD_SAFE=1 testGDK.cc -o testGDK
