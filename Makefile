CC=gcc
OBJECTS= *.c
CFLAGS= -lpng
NAME=gol
gol: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS)
clean:
	rm $(NAME)
