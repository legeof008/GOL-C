CC=gcc
OBJECTS= *.c
CFLAGS= -g -Og -gnu99
NAME=memleak
memleak: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) -lpng
clean:
	rm $(NAME)
