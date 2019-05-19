CXX=g++-8
CPPFLAGS=-std=c++17 -Wall -Wpedantic
BINNAME=maze

OBJECTS=bitmap.o main.o

$(BINNAME): $(OBJECTS)
	$(CXX) $(CPPFLAGS) $(OBJECTS) -o $(BINNAME)

.PHONY: clean
clean:
	rm -f *.o

.PHONY: distclean
distclean: clean
	rm -f $(BINNAME)
