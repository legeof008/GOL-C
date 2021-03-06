CC = gcc
OBJECTS = image_creator.c cell.c source.c
NAME = output
output: $(OBJECTS)
	$(CC) -o $(NAME) $(OBJECTS) 
clean:
	rm $(NAME)