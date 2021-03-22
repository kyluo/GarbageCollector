# OBJS	= GarbageCollector_P.o test_P.o
# SOURCE	= GarbageCollector_P.c test_P.c
# HEADER	= GarbageCollector_P.h
# OUT	= ./test
# CC	 = gcc
# FLAGS	 = -g -c -Wall
# LFLAGS	 = 

# all: $(OBJS)
# 	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

# GarbageCollector_P.o: GarbageCollector_P.c
# 	$(CC) $(FLAGS) GarbageCollector_P.c 

# test.o: test.c
# 	$(CC) $(FLAGS) test.c 


# clean:
# 	rm -f $(OBJS) $(OUT)

OBJS	= test_P.o vector.o callbacks.o GarbageCollector_P.o
SOURCE	= test_P.c vector.c callbacks.c GarbageCollector_P.c
HEADER	= vector.h callbacks.h GarbageCollector_P.h
OUT	= main
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

test_P.o: test_P.c
	$(CC) $(FLAGS) test_P.c 

vector.o: vector.c
	$(CC) $(FLAGS) vector.c 

callbacks.o: callbacks.c
	$(CC) $(FLAGS) callbacks.c 

GarbageCollector_P.o: GarbageCollector_P.c
	$(CC) $(FLAGS) GarbageCollector_P.c 


clean:
	rm -f $(OBJS) $(OUT)