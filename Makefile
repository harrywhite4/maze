CXX ?= g++
CXXFLAGS = -std=c++17 -Wall -Wpedantic
MKDIR = mkdir -p
RMDIR = rmdir
BINNAME = maze
SRCDIR = src
BINDIR = bin

BINPATH = $(BINDIR)/$(BINNAME)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)
DEPS = $(SOURCES:%.cpp=%.d)

# Link objects
$(BINPATH): $(BINDIR) $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $(BINPATH)

# Create dir
$(BINDIR):
	$(MKDIR) $(BINDIR)

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
	$(RM) $(BINPATH)
	$(RMDIR) $(BINDIR)
