CC = clang++

FLAGS = -Wall -Wextra -pedantic -ferror-limit=1 -std=c++11 -O0 -g

CFLAGS = `pkg-config --cflags libbsoncxx`
LFLAGS = `pkg-config --libs libbsoncxx`

NM = nm

NMFLAGS = -C -l -p -S

CAT = less

HPP_COMMON = $(wildcard reflection++/*.hpp)
HPP_VISITOR = $(wildcard reflection++/visitor/*.hpp)
HPP_TESTS = tests/common.hpp
CPP_MAIN = tests/main.cpp
CPP_TESTS = $(wildcard tests/*.cpp)

default:
	echo "make test\nmake pp\nmake nm\nmake clean"

tests.out: $(HPP_COMMON) $(HPP_VISITOR) $(HPP_TESTS) $(CPP_TESTS)
	$(CC) $(FLAGS) $(CFLAGS) $(LFLAGS) $(CPP_TESTS) -o $@

main.cpp.out:
	$(CC) -E $(FLAGS) $(CFLAGS) $(CPP_MAIN) -o $@

names.nm.out: tests.out
	$(NM) $(NMFLAGS) $< > $@

test: tests.out
	./$<

pp: main.cpp.out
	$(CAT) $<

nm: names.nm.out
	$(CAT) $<

clean:
	touch rm_placeholder.out
	rm *.out
