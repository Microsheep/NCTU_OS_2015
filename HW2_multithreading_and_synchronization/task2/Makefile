# It is a makefile for hw2
all:base.cpp pi_free.cpp pi_lock.cpp
	g++ base.cpp -std=c++11 -lpthread -o base.o
	g++ pi_free.cpp -std=c++11 -lpthread -o pi_free.o
	g++ pi_lock.cpp -std=c++11 -lpthread -o pi_lock.o
clean:
	rm -f base.o
	rm -f pi_free.o
	rm -f pi_lock.o
