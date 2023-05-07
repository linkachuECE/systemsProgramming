threads?=1
total?=10000

all:
	gcc -g -Wall t-test1.c -DN_THREADS=$(threads) -DN_TOTAL=$(total) -o bin/test$(threads)threads$(total)total -lpthread

clean:
	rm bin/*
