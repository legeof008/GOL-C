CC=gcc
INC_DIR = ../
OBJECTS= *.c
CFLAGS= -fcommon
NAME=GoL
GoL: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) -lpng
	mv $(NAME) ../
clean:
	rm $(NAME)
