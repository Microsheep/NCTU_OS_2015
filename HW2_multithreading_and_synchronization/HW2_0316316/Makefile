# It is a makefile for hw2
all:mutex.cpp sem.cpp spinlock.cpp homemade_spinlock.cpp pi_free.cpp pi_lock.cpp crossroad.cpp
	g++ mutex.cpp -std=c++11 -lpthread -o mutex
	g++ sem.cpp -std=c++11 -lpthread -o sem
	g++ spinlock.cpp -std=c++11 -lpthread -o spinlock
	g++ homemade_spinlock.cpp -std=c++11 -lpthread -o homemade_spinlock
	g++ pi_free.cpp -std=c++11 -lpthread -o pi_free
	g++ pi_lock.cpp -std=c++11 -lpthread -o pi_lock
	g++ crossroad.cpp -std=c++11 -lpthread -o crossroad
clean:
	rm -f mutex
	rm -f sem
	rm -f spinlock
	rm -f homemade_spinlock
	rm -f pi_free
	rm -f pi_lock
	rm -f crossroad

