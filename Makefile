CC=gcc
OBJECTS= *.c
CFLAGS= -lpng
NAME=gol
gol: $(OBJECTS)
	$(CC) -o $(NAME) $(OBJECTS) $(CFLAGS)
clean:
	rm $(NAME)
