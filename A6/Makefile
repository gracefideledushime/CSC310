INCLUDE_DIRS := ./
CC 		 	:= g++
CFLAGS 		 := -I$(INCLUDE_DIRS)

# Add dsw.o so the final executable links all needed object files
OBJS 		 := customErrorClass.o mainSamples.o geneBank.o
DEPS 		 := geneBank.cpp \
				customErrorClass.cpp

.PHONY: clean all

all: mainSamples.o geneBank.o customErrorClass.o

customErrorClass.o: customErrorClass.cpp
	$(CC) -c -o $@ $(CFLAGS) $<

mainSamples.o: mainSamples.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

geneBank.o: geneBank.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

a.out: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

# Rule to run the program
run: a.out
	./a.out samples.bin

clean:
	rm $(OBJS) a.out