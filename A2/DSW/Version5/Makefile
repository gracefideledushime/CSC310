INCLUDE_DIRS := ../ExcepHandling
CC 		 	:= g++
CFLAGS 		 := -I$(INCLUDE_DIRS)

# Add dsw.o so the final executable links all needed object files
OBJS 		 := customErrorClass.o main.o dsw.o
DEPS 		 := ${INCLUDE_DIRS}/customErrorClass.h

.PHONY: clean all

all: customErrorClass.o main.o dsw.o a.out

customErrorClass.o: ${INCLUDE_DIRS}/customErrorClass.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

main.o: main.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

dsw.o: dsw.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

a.out: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

# Rule to run the program
run: a.out
	./a.out

clean:
	rm $(OBJS) a.out