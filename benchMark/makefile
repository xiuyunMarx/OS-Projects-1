CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = benchmark
all: clean $(TARGET) 


$(TARGET1): benchmark.c
	$(CC) $(CFLAGS) -o $(TARGET1) benchmark.c

clean:
	rm -f $(TARGET) 

.PHONY: all clean
