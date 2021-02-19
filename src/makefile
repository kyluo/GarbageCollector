OBJS	= GarbageCollector.o
SOURCE	= GarbageCollector.c
HEADER	= GarbageCollector.h
OUT	= gc
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

GarbageCollector.o: GarbageCollector.c
	$(CC) $(FLAGS) GarbageCollector.c 


clean:
	rm -f $(OBJS) $(OUT)