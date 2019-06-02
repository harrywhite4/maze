CXX ?= g++
CXXFLAGS = -std=c++17 -Wall -Wpedantic -Werror
MKDIR = mkdir -p
BINNAME = maze
SRCDIR = src
BUILDDIR = build
BINDIR = bin

BINPATH = $(BINDIR)/$(BINNAME)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
DEPS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.d)

# Link objects
$(BINPATH): $(OBJECTS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $(BINPATH)

# Create bin dir
$(BINDIR):
	$(MKDIR) $(BINDIR)

# Create build dir
$(BUILDDIR):
	$(MKDIR) $(BUILDDIR)

# Include generated deps rules
-include $(DEPS)

# Object files
# after first compilation will be joined with generated deps rules
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CPPFLAGS) -MP -MMD -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(BUILDDIR)/*.o $(BUILDDIR)/*.d

.PHONY: distclean
distclean: clean
	$(RM) $(BINPATH)
