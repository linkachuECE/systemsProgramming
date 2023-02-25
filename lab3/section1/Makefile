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
SCHEDULERS := mfq sjf rr
SCHEDEXES := $(patsubst %, $(BINDIR)/%, $(SCHEDULERS))
SCHEDSRCS := $(patsubst %, $(SRCDIR)/%.c, $(SCHEDULERS))

# Define child processes to run
CHILDPROCESS := p
CHILDSRC := $(SRCDIR)/$(CHILDPROCESS).c
CHILDEXES := $(BINDIR)/$(CHILDPROCESS)

all: $(SCHEDEXES) $(CHILDEXES)

# Generate scheduler executables
define generateExe =
	$(shell $(CC) $(CFLAGS) $(QUEUESRC) $(1) -o $(2))
endef

$(SCHEDEXES): $(SCHEDSRCS) $(QUEUESRC)
	$(foreach SRC, $(SCHEDSRCS), $(call generateExe, $(SRC), \
	$(patsubst $(SRCDIR)/%.c, $(BINDIR)/%, $(SRC))))

# Generate child process executables

clean:
	rm $(BINDIR)/*