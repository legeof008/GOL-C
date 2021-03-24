CC=gcc
OBJECTS= *.c
CFLAGS= 
NAME=GoL
GoL: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) -lpng
clean:
	rm $(NAME)
