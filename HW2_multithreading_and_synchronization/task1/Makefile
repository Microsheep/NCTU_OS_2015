# It is a makefile for hw2
all:base.cpp mutex.cpp sem.cpp spinlock.cpp
	g++ base.cpp -std=c++11 -lpthread -o base.o
	g++ mutex.cpp -std=c++11 -lpthread -o mutex.o
	g++ sem.cpp -std=c++11 -lpthread -o sem.o
	g++ spinlock.cpp -std=c++11 -lpthread -o spinlock.o
clean:
	rm -f base.o
	rm -f mutex.o
	rm -f sem.o
	rm -f spinlock.o
