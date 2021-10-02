CC = gcc

INC = list.h hash.h bitmap.h debug.h limits.h round.h hex_dump.h
CFLAGS = -g
TARGET = testlib
OBJECTS = main.o list.o hash.o hex_dump.o

$(TARGET) : $(OBJECTS)
			  $(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

main.o : $(INC) main.c
				$(CC) $(CFLAGS) -c -o main.o main.c

list.o : $(INC) list.c
			  $(CC) $(CFLAGS) -c -o list.o list.c

hash.o : $(INC) hash.c
			  $(CC) $(CFLAGS) -c -o hash.o hash.c

clean:
	-rm -f $(OBJECTS) $(TARGET)