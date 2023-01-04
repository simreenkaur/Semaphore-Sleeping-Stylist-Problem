CC = gcc
FLAGS = -lpthread
INCLUDES = -lm

sleepingStylistSem: sleepingStylistMon.c monitor.o
	$(CC) $(FLAGS) sleepingStylistMon.c monitor.o -o sleepingStylistMon
monitor.o: monitor.c
	$(CC) -c monitor.c -o monitor.o
clean:
	rm -f *~ *.o
