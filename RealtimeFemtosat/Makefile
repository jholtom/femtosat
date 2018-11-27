CC=gcc
CFLAGS=-I.
DEPS = femto.h
OBJ = femto_stream.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

beacon: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
