CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET1 = MyCopy
TARGET2 = ForkCopy
all: $(TARGET1) $(TARGET2)

$(TARGET1): MyCopy.c
	$(CC) $(CFLAGS) -o $(TARGET1) MyCopy.c

clean:
	rm -f $(TARGET1)

.PHONY: all clean
