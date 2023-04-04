FLAGS = -Wall -g
SRCDIR = src
BINDIR = bin
INCDIR = inc

all:
	gcc $(SRCDIR)/server.c -I$(INCDIR) $(SRCDIR)/linkedList.c $(FLAGS) -o $(BINDIR)/server
	gcc $(SRCDIR)/client.c -I$(INCDIR) $(FLAGS) -o $(BINDIR)/client

clean:
	rm -r bin/*