CC = g++
OBJECTS = midterm.o

midterm: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@
%.o: %.c
	$(CC) -c $< -o $@
clean:
	rm -f midterm *.o 