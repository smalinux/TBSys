CC = gcc
CFLAGS = -std=c99
CLIB = -lsqlite3
OBJ = main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CLIB) $(CFLAGS)
