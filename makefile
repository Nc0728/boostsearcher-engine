cc=g++

parser:parser.cpp
	$(cc) $^ -o $@  -lboost_system -lboost_filesystem -std=c++11
PHONY:
clean:
	rm -f parser
