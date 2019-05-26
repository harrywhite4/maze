CXX ?= g++
CPPFLAGS = -std=c++17 -Wall -Wpedantic
BINNAME = maze

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)
DEPS = $(SOURCES:%.cpp=%.d)

# Link objects
$(BINNAME): $(OBJECTS)
	$(CXX) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) -o $(BINNAME)

# Include generated deps rules
-include $(DEPS)

# Object files
# after first compilation will be joined with generated deps rules
%.o: %.cpp
	$(CXX) $(CPPFLAGS) -MP -MMD -c $< -o $@

.PHONY: clean
clean:
	$(RM) *.o *.d

.PHONY: distclean
distclean: clean
	$(RM) $(BINNAME)
