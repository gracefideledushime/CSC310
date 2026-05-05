INCLUDE_DIRS := ./
CC			 := g++
CFLAGS 		 := -I$(INCLUDE_DIRS)

# Add dsw.o so the final executable links all needed object files
OBJS 		 := customErrorClass.o p_bst_1.o
DEPS 		 := customErrorClass.h

.PHONY: clean all

all: customErrorClass.o p_bst_1.o a.out

customErrorClass.o: ${INCLUDE_DIRS}/customErrorClass.cpp
	$(CC) -c -o $@ $(CFLAGS) $<

p_bst_1.o: p_bst_1.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

a.out: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

# Rule to run the program
run: a.out
	./a.out

clean:
	rm $(OBJS) a.out