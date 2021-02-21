OBJS	= GarbageCollector.o test.o
SOURCE	= GarbageCollector.c test.c
HEADER	= GarbageCollector.h
OUT	= ./test
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

GarbageCollector.o: GarbageCollector.c
	$(CC) $(FLAGS) GarbageCollector.c 

test.o: test.c
	$(CC) $(FLAGS) test.c 


clean:
	rm -f $(OBJS) $(OUT)