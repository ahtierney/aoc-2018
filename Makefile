
CFLAGS= -Wall -Iutils -g
LDFLAGS=
# FIXME: this doesn't omit utils and still tries to build an exececutable from it
DAYS := $(basename $(filter-out utils/*, $(wildcard */*.c)))
UTILS = utils/BSTNode.o utils/Grid.o utils/HashTable.o utils/printhelpers.o

all: $(DAYS)

$(DAYS): $(UTILS)

.PHONY: clean
clean:
	-rm $(wildcard */*.o)
	-rm $(basename $(wildcard */*.c))
