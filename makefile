SOURCEDIR = src
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
INCLUDEDIR = include

EXECUTABLE = $(BUILDDIR)/main
SOURCES = $(wildcard $(SOURCEDIR)/*.cpp)
OBJECTS = $(patsubst $(SOURCEDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

CC = g++
FLAGS = -std=c++20 -g -c -Wall -I./$(INCLUDEDIR)

all: dir $(EXECUTABLE)

dir:
	mkdir -p $(BUILDDIR) $(OBJDIR)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CC) $(FLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o $(EXECUTABLE)
