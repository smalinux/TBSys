CC = gcc
CFLAGS = -O2 -DNDEBUG -DLOG_USE_COLOR -std=c99
CLIB = -lsqlite3
OBJ = main.o log.o tbsys.o fort.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tbsys: $(OBJ)
	$(CC) -o $@ $^ $(CLIB) $(CFLAGS)

clean:
	rm *.o
	rm tbsys
