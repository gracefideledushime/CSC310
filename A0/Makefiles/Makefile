INCLUDE_DIRS := Includes
CC 			 := g++
CFLAGS 		 := -I$(INCLUDE_DIRS)

OBJS 		 := customErrorClass.o customErrorHandling.o
DEPS 		 := ${INCLUDE_DIRS}/customErrorClass.h

.PHONY: clean all

all: customErrorClass.o customErrorHandling.o errorTutorial

clean:
	rm $(OBJS) errorTutorial

customErrorClass.o: customErrorClass.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

customErrorHandling.o: customErrorHandling.cpp ${DEPS}
	$(CC) -c -o $@ $(CFLAGS) $<

errorTutorial: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)