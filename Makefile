# ---------- Variables ----------
CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wpedantic -Werror
MKDIR = mkdir -p
LINTER ?= cpplint
BINNAME = maze
SRCDIR = src
BUILDDIR = build
BINDIR = bin

GTEST_DIR ?= /usr/src/googletest/googletest
TESTDIR = test

BINPATH = $(BINDIR)/$(BINNAME)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.o)
DEPS = $(SOURCES:$(SRCDIR)/%.cpp=$(BUILDDIR)/%.d)

# ---------- Main build ----------

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
	$(CXX) $(CXXFLAGS) -MP -MMD -c $< -o $@

# ---------- Testing ----------

# Gtest object
$(BUILDDIR)/gtest-all.o: | $(BUILDDIR)
	$(CXX) -std=c++11 -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -pthread -c $(GTEST_DIR)/src/gtest-all.cc -o $@

# Gtest archive
$(BUILDDIR)/libgtest.a: $(BUILDDIR)/gtest-all.o
	ar -rv $@ $<

$(BINDIR)/test: $(TESTDIR)/test.cpp $(BUILDDIR)/libgtest.a $(BUILDDIR)/grid.o
	$(CXX) $(CXXFLAGS) -I$(SRCDIR) -pthread $^ -o $@

# ---------- Phony ----------

.PHONY: test
test: $(BINDIR)/test
	$<

.PHONY: lint
lint:
	$(LINTER) --recursive ./src

.PHONY: clean
clean:
	$(RM) $(BUILDDIR)/*.o $(BUILDDIR)/*.d

.PHONY: distclean
distclean: clean
	$(RM) $(BINPATH)
