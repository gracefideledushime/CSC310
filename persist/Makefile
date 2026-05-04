INCLUDE_DIRS := ./Includes
CC 		 	:= g++
CFLAGS 		 := -I$(INCLUDE_DIRS)

# Add dsw.o so the final executable links all needed object files
OBJS 		 := customErrorClass.o main.o mst.o binaryHeap.o binomialHeap.o
DEPS 		 := binaryHeap.cpp \
      			mst.cpp \
        		binomialHeap.cpp \
				${INCLUDE_DIRS}/customErrorClass.cpp

.PHONY: clean all

all: customErrorClass.o main.o mst.o binaryHeap.o binomialHeap.o customErrorClass.o

customErrorClass.o: ${INCLUDE_DIRS}/customErrorClass.cpp
	$(CC) -c -o $@ $(CFLAGS) $<

main.o: main.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

mst.o: mst.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

binomialHeap.o: binomialHeap.cpp
	$(CC) -c -o $@ $(CFLAGS) $<

binaryHeap.o: binaryHeap.cpp
	$(CC) -c -o $@ $(CFLAGS) $<

a.out: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

# Rule to run the program
run: a.out
	./a.out

clean:
	rm $(OBJS) a.out