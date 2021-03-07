CC=gcc
OBJECTS= *.c
CFLAGS=
NAME=gol
gol: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS)
clean:
	rm $(NAME)