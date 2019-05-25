CXX=g++-8
CPPFLAGS=-std=c++17 -Wall -Wpedantic
BINNAME=maze

SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)

$(BINNAME): $(OBJECTS)
	$(CXX) $(CPPFLAGS) $(OBJECTS) -o $(BINNAME)

main.o: main.cpp bitmap.hpp

bitmap.o: bitmap.cpp bitmap.hpp

.PHONY: clean
clean:
	rm -f *.o

.PHONY: distclean
distclean: clean
	rm -f $(BINNAME)
