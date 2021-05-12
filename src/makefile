OBJS	= GarbageCollector.o 
TESTS = mark_and_sweep test_stack_ref test_heap_ref test_data_seg test_P
CLEAN_TARGETS = *.o $(TESTS)
#GarbageCollector.o test_stack_ref.o test_heap_ref.o test_data_seg.o GarbageCollector_P.o test_P.o callbacks.o vector.o
SOURCE	= GarbageCollector.c test_data_seg.c
HEADER	= GarbageCollector.h
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 
DIR = tests
INCLUDES = includes

tests: $(TESTS)

mark_and_sweep: GarbageCollector.o mark_and_sweep_demo.o 
	$(CC) -g  mark_and_sweep_demo.o GarbageCollector.o -o mark_and_sweep $(LFLAGS)

test_stack_ref:  GarbageCollector.o test_stack_ref.o
	$(CC) -g test_stack_ref.o GarbageCollector.o -o test_stack_ref $(LFLAGS)

test_heap_ref: GarbageCollector.o test_heap_ref.o
	$(CC) -g test_heap_ref.o GarbageCollector.o -o test_heap_ref $(LFLAGS)

test_data_seg: GarbageCollector.o test_data_seg.o 
	$(CC) -g test_data_seg.o GarbageCollector.o -o test_data_seg $(LFLAGS)

test_P:  test_P.o vector.o callbacks.o GarbageCollector_P.o
	$(CC) -g test_P.o vector.o callbacks.o GarbageCollector_P.o -o  test_P $(LFLAGS)

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

mark-and-sweep.o:
	$(CC) $(FLAGS) mark_and_sweep_demo.c 

test_P.o: $(DIR)/test_P.c
	$(CC) $(FLAGS) $(DIR)/test_P.c 

vector.o: $(INCLUDES)/vector.c
	$(CC) $(FLAGS) $(INCLUDES)/vector.c 

callbacks.o: $(INCLUDES)/callbacks.c
	$(CC) $(FLAGS) $(INCLUDES)/callbacks.c 

GarbageCollector_P.o: GarbageCollector_P.c GarbageCollector_P.h
	$(CC) $(FLAGS) GarbageCollector_P.c 

