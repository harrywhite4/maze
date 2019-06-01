CXX ?= g++
CPPFLAGS = -std=c++17 -Wall -Wpedantic
BINNAME = maze
SRCDIR = src
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)
DEPS = $(SOURCES:%.cpp=%.d)

# Link objects
$(BINNAME): $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CXX) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) -o $(BINDIR)/$(BINNAME)

# Include generated deps rules
-include $(DEPS)

# Object files
# after first compilation will be joined with generated deps rules
%.o: %.cpp
	$(CXX) $(CPPFLAGS) -MP -MMD -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(SRCDIR)/*.o $(SRCDIR)/*.d

.PHONY: distclean
distclean: clean
	$(RM) $(BINDIR)/$(BINNAME)
