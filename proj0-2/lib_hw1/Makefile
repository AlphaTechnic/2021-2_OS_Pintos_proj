CC = gcc
CFLAGS = -W -Wall
TARGET = testlib.out
OBJECTS = main.o bitmap.o debug.o hash.o hex_dump.o list.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean :
	rm *.o
	rm *.out