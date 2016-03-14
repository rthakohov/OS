CC = gcc
OBJECTS = cat.o

cat: $(OBJECTS)
	gcc $(OBJECTS) -o $@
%.o: %.c
	gcc -c $< -o $@
clean:
	rm -f cat *.o 