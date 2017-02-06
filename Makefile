CC = gcc
WARNINGS =
CFLAGS = -std=c99 -g -I. $(WARNINGS)
DEPS = heap.h
OBJ = heap.o test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	-rm test $(OBJ)
