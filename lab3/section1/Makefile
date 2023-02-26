CC = gcc

INCDIR := inc
SRCDIR := src
OBJDIR := obj
BINDIR := bin

CFLAGS = -g
CFLAGS += -Wall
CFLAGS += -I$(INCDIR)

# Define queue file
QUEUESRC = $(SRCDIR)/queue.c

# Define scheduler policies here
SCHEDULERS := mfq sjf rr fcfs
SCHEDEXES := $(patsubst %, $(BINDIR)/%, $(SCHEDULERS))
SCHEDSRCS := $(patsubst %, $(SRCDIR)/%.c, $(SCHEDULERS))

# Define child processes to run
CHILDPROCESS := p
CHILDSRC := $(SRCDIR)/$(CHILDPROCESS).c
CHILDEXE := $(BINDIR)/$(CHILDPROCESS)
CHILDEXES := $(CHILDEXE)1 $(CHILDEXE)2 $(CHILDEXE)3 $(CHILDEXE)4 $(CHILDEXE)5

all: $(SCHEDEXES) $(CHILDEXES)

# Generate scheduler executables
define generateExe =
	$(shell $(CC) $(CFLAGS) $(QUEUESRC) $(1) -o $(2))
endef

$(SCHEDEXES): $(SCHEDSRCS) $(QUEUESRC)
	$(foreach SRC, $(SCHEDSRCS), $(call generateExe, $(SRC), \
	$(patsubst $(SRCDIR)/%.c, $(BINDIR)/%, $(SRC))))

# Generate child process executables
$(CHILDEXES): $(CHILDSRC)
	for exe in $(CHILDEXES); do \
		$(CC) $(CFLAGS) $(CHILDSRC) -o $$exe; \
	done

clean:
	rm $(BINDIR)/*