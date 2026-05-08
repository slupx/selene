FILES = $(shell find . -type f -name "*.cpp")

compile:
	clang++ $(FILES) -o bin/app -Iinclude -lraylib -lX11 -lGL -lm -lpthread -ldl -lrt

run:
	./bin/app

all: compile run

reload:
	ls $(FILES) | entr -r sh -c "make all"
