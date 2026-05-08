FILES = $(shell find . -type f -name "*.cpp")

# set platform-specific compile options
ifeq ($(OS),Windows_NT)
	COMPILE_OPTIONS = "no windows support for now"
else ifeq ($(shell uname),Darwin)
	LINK_OPTS = -L/opt/homebrew/lib -lraylib
	INCLUDE_OPTS = -I/opt/homebrew/include -Iinclude \
		-framework OpenGL -framework Cocoa -framework IOKit
else ifeq ($(shell uname),Linux)
	LINK_OPTS = -lraylib -lX11 -lGL -lm -lpthread -ldl
	INCLUDE_OPTS = -Iinclude
endif

compile:
	clang++ -o bin/app $(FILES) $(LINK_OPTS) $(INCLUDE_OPTS)

run:
	./bin/app

all: compile run

reload:
	ls $(FILES) | entr -r sh -c "make all"
