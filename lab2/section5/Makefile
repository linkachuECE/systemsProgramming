CC=gcc

FLAGS = -g
FLAGS += -Wall
FLAGS += -std=c99

BINDIR = bin
SRCDIR = src

PROG=multiprocess

$(BINDIR)/$(PROG): $(SRCDIR)/$(PROG).o
	$(CC) $^ -o $@

$(SRCDIR)/$(PROG).o: $(SRCDIR)/$(PROG).c
	$(CC) $^ $(FLAGS) -c -o $@

clean:
	rm $(BINDIR)/*