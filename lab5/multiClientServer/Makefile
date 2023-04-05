FLAGS = -Wall -g
SRCDIR = src
BINDIR = bin
INCDIR = inc

all:
	gcc $(SRCDIR)/server.c $(SRCDIR)/serverChild.c $(SRCDIR)/serverParent.c $(SRCDIR)/linkedList.c -I$(INCDIR) $(FLAGS) -o $(BINDIR)/server
	gcc $(SRCDIR)/client.c -I$(INCDIR) $(FLAGS) -o $(BINDIR)/client

clean:
	rm -r bin/*