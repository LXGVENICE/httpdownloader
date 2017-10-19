all: downloader

downloader: main.cpp
	g++  -g -std=c++11 main.cpp -o downloader -lpthread -lboost_filesystem -lboost_system -lboost_thread

clean:
	rm downloader

restart:
	rm *.meta

