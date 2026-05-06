INCLUDE_DIRS := ./
CC           := g++
CFLAGS       := -I$(INCLUDE_DIRS)

OBJS         := customErrorClass.o avl.o hash.o mainHash.o

.PHONY: clean all

all: a.out

customErrorClass.o: customErrorClass.cpp  customErrorClass.h
	$(CC) -c -o $@ $(CFLAGS) $<

avl.o: AVL/avl.cpp AVL/avl.h
	$(CC) -c -o $@ $(CFLAGS) $<

hash.o: hash.cpp hash.h AVL/avl.h
	$(CC) -c -o $@ $(CFLAGS) $<

mainHash.o: mainHash.cpp hash.h
	$(CC) -c -o $@ $(CFLAGS) $<

a.out: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

run: a.out
	./a.out

clean:
	rm -f $(OBJS) a.out