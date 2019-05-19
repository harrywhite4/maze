CXX=g++-8
CPPFLAGS=-std=c++17 -Wall -Wpedantic
BINNAME=maze

OBJECTS=bitmap.o main.o

$(BINNAME): $(OBJECTS)
	$(CXX) $(CPPFLAGS) $(OBJECTS) -o $(BINNAME)

bitmap.o: bitmap.cpp bitmap.hpp
	$(CXX) $(CPPFLAGS) -c bitmap.cpp

main.o: main.cpp bitmap.hpp
	$(CXX) $(CPPFLAGS) -c main.cpp

clean:
	rm *.o $(BINNAME)
