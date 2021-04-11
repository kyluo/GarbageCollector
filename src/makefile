OBJS	= GarbageCollector.o 
# TESTS = test_stack_ref test_heap_ref test_data_seg
TESTS = test_stack_ref test_heap_ref test_data_seg
CLEAN_TARGETS = GarbageCollector.o test_stack_ref.o test_heap_ref.o test_data_seg.o
SOURCE	= GarbageCollector.c test_data_seg.c
HEADER	= GarbageCollector.h
OUT	= test_stack_ref test_heap_ref test_data_seg 
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 
DIR = tests

tests: $(TESTS)

test_stack_ref:  GarbageCollector.o test_stack_ref.o
	$(CC) -g test_stack_ref.o GarbageCollector.o -o test_stack_ref $(LFLAGS)

test_heap_ref: GarbageCollector.o test_heap_ref.o
	$(CC) -g test_heap_ref.o GarbageCollector.o -o test_heap_ref $(LFLAGS)

test_data_seg: GarbageCollector.o test_data_seg.o 
	$(CC) -g test_data_seg.o GarbageCollector.o -o test_data_seg $(LFLAGS)

test_stack_ref.o: $(DIR)/test_stack_ref.c GarbageCollector.h
	$(CC) $(FLAGS) $(DIR)/test_stack_ref.c

test_heap_ref.o: $(DIR)/test_heap_ref.c GarbageCollector.h
	$(CC) $(FLAGS) $(DIR)/test_heap_ref.c

test_data_seg.o: $(DIR)/test_data_seg.c GarbageCollector.h
	$(CC) $(FLAGS) $(DIR)/test_data_seg.c

GarbageCollector.o: GarbageCollector.c GarbageCollector.h 
	$(CC) $(FLAGS) GarbageCollector.c 

helloworld : helloworld.o 
	$(CC) -g helloworld.o -o helloworld

helloworld.o: $(DIR)/helloworld.c 
	gcc $(FLAGS) $(DIR)/helloworld.c

clean:
	rm -f $(CLEAN_TARGETS) $(OUT)