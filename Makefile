SRC = src
EVENTS = $(SRC)/events
HANDLERS = $(SRC)/handlers
OBJS = $(SRC)/main.o $(SRC)/events.o $(SRC)/db.o $(EVENTS)/ready.o $(EVENTS)/message_reaction_add.o $(HANDLERS)/reaction_role.o

PROGRAM = bot

CC   ?= cc
CCLD ?= $(CC)

CFLAGS ?= -O2
CFLAGS += -std=c99

LDFLAGS += -L/usr/local/lib -lcurl -ldiscord -lsqlite3

.PHONY: clean all $(PROGRAM)
.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(PROGRAM): $(OBJS)
	$(CCLD) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

all: $(PROGRAM)

clean:
	rm -vf $(OBJS) $(PROGRAM)
