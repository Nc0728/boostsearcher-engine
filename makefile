cc=g++
PAR=parser
HTTP_SERVER=http_server
PHONY:all
all: $(PAR) $(HTTP_SERVER)
$(PAR):parser.cpp
	$(cc) $^ -o $@  -lboost_system -lboost_filesystem -std=c++11
$(HTTP_SERVER):http_server.cpp
	$(cc) $^ -o $@  -pthread -ljsoncpp  -std=c++11
PHONY:
clean:
	rm -f $(DUG) $(PAR) 
