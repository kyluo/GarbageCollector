OBJS	= GarbageCollector.o test.o
SOURCE	= GarbageCollector.c test.c
HEADER	= GarbageCollector.h
OUT	= test
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)


test_stack_ref: test_stack_ref.o GarbageCollector.o
	$(CC) -g test_stack_ref.o GarbageCollector.o -o test_stack_ref $(LFLAGS)

test_stack_ref.o: test_stack_ref.c
	gcc $(FLAGS) test_stack_ref.c

test_heap_ref: test_heap_ref.o GarbageCollector.o
	$(CC) -g test_heap_ref.o GarbageCollector.o -o test_heap_ref $(LFLAGS)

test_heap_ref.o: test_heap_ref.c 
	gcc $(FLAGS) test_heap_ref.c
GarbageCollector.o: GarbageCollector.c
	$(CC) $(FLAGS) GarbageCollector.c 

demo.o: demo.c
	$(CC) $(FLAGS) test.c 

clean:
	rm -f $(OBJS) test_stack_ref.o $(OUT)