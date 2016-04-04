CC = gcc
OBJECTS = sigusr.o

sigusr: $(OBJECTS)
	gcc $(OBJECTS) -o $@
%.o: %.c
	gcc -c $< -o $@
clean:
	rm -f sigusr *.o 