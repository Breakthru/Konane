#!/bin/bash

# compile 3 files and then
g++ -o konane.o konane.cpp `pkg-config --cflags gtk+-2.0`

g++ -o interfaccia.o interfaccia.cpp `pkg-config --cflags gtk+-2.0`

g++ -o main.o main.cpp `pkg-config --cflags gtk+-2.0`

# link 3 objects into 1 executable
g++ -o play_konane main.o interfaccia.o konane.o `pkg-config --libs gtk+-2.0`

