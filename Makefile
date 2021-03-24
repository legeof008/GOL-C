CC=gcc
OBJECTS= *.c
CFLAGS= -g -Og 
NAME=memleak
memleak: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) -lpng
clean:
	rm $(NAME)
