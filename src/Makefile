CXX=g++
CFLAGS=`pkg-config --cflags gtk+-2.0`
LIBS=`pkg-config --libs gtk+-2.0`


all: konane


%.o : %.cpp
	$(CXX) -c $(CFLAGS) $<  -o $@


konane: konane.o interfaccia.o main.cpp
	$(CXX) main.cpp konane.o interfaccia.o $(CFLAGS) $(LIBS) -o konane


clean:
	rm *.o *.gch
