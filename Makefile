CC = gcc
CFLAGS = -O2 -g -Wall -DNDEBUG -DLOG_USE_COLOR -std=c99
LIBS = -lsqlite3 -lcriterion
OBJ = main.o
DEPS = log.o tbsys.o fort.o
TEST = test.o
BIN = tbsys

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJ) $(DEPS)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)

test: $(DEPS) $(TEST)
	sqlite3 test.db ".databases"
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS)
	./test; $(RM) test

clean:
	$(RM) *.o
	$(RM) tbsys
	$(RM) *.orig
