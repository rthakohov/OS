CC = gcc
OBJECTS = hello_world.o

hello_world: $(OBJECTS)
	gcc $(OBJECTS) -o $@
%.o: %.c
	gcc -c $< -o $@
clean:
	rm -f hello_world *.o 