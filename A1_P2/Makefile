INCLUDE_DIRS := ./Splay
CC			 := g++
CFLAGS 		 := -I$(INCLUDE_DIRS)

# Add dsw.o so the final executable links all needed object files
OBJS 		 := customErrorClass.o splay.o mainsplay.o
DEPS 		 := customErrorClass.h

.PHONY: clean all

all: customErrorClass.o splay.o a.out

customErrorClass.o: customErrorClass.cpp
	$(CC) -c -o $@ $(CFLAGS) $<

splay.o: ${INCLUDE_DIRS}/splay.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

mainsplay.o: ${INCLUDE_DIRS}/mainsplay.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

a.out: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

# Rule to run the program
run: a.out
	./a.out

clean:
	rm $(OBJS) a.out