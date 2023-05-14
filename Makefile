CC = gcc
CFLAGS = -DLOG_USE_COLOR -std=c99
CLIB = -lsqlite3
OBJ = main.o log.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CLIB) $(CFLAGS)
