CC = gcc
CFLAGS = -O2 -g -Wall -DNDEBUG -DLOG_USE_COLOR -std=c99
LIBS = -lsqlite3
OBJ = main.o log.o tbsys.o fort.o
BIN = tbsys

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

clean:
	$(RM) *.o
	$(RM) tbsys
