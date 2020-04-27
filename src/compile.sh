#!/bin/bash

g++ -o konane.o konane.cpp `pkg-config --cflags gtk+-2.0`
g++ -o interfaccia.o interfaccia.cpp `pkg-config --cflags gtk+-2.0`


