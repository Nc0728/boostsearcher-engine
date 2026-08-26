cc=g++
SS=server
PAR=parser
PHONY:all
all:$(SS) $(PAR)
$(SS):server.cpp
	$(cc) $^ -o $@ -ljsoncpp   -std=c++11
$(PAR):parser.cpp
	$(cc) $^ -o $@  -lboost_system -lboost_filesystem -std=c++11
PHONY:
clean:
	rm -f parser server
