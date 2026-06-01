SOURCE = $(shell find src -type f -name "*.cpp")
RELOAD_FILES = $(shell find src include shaders -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.fs" -o -name "*.vs" \))


CXXFLAGS = -std=c++17 -Wall

# set platform-specific compile options
ifeq ($(OS),Windows_NT)
	COMPILE_OPTIONS = "no windows support yet"
else ifeq ($(shell uname),Darwin)
	LINK_OPTS = -L/opt/homebrew/lib -lraylib
	INCLUDE_OPTS = -I/opt/homebrew/include -Iinclude \
		-framework OpenGL -framework Cocoa -framework IOKit
else ifeq ($(shell uname),Linux)
	LINK_OPTS = -lraylib -lX11 -lGL -lm -lpthread -ldl
	INCLUDE_OPTS = -Iinclude
endif

compile:
	clang++ $(CXXFLAGS) -o bin/app $(SOURCE) $(LINK_OPTS) $(INCLUDE_OPTS)
	rm -rf bin/shaders
	mkdir -p bin/shaders
	cp -R shaders/ bin/shaders/


run:
	./bin/app

all: compile run

reload:
	ls $(RELOAD_FILES) | entr -r sh -c "make all"
