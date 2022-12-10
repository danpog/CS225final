CXX=clang++
CXX_FLAGS=-std=c++20 -Iincludes -g -fstandalone-debug -O0 -Wall -Wextra -Werror -ljsoncpp
FILES= ./entry/parser.cpp ./lib/playlist.cpp ./lib/song.cpp ./lib/node.cpp ./src/graph.cpp

exec: bin/exec
tests: bin/tests

bin/exec: $(FILES)
	$(CXX) $(CXX_FLAGS) $^ -o $@

bin/tests: ./tests/tests.cpp
	$(CXX) $(CXX_FLAGS) $^ -o $@

.DEFAULT_GOAL := exec
.PHONY: exec tests clean

clean:
	rm -f bin/*
