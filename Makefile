PROG = main.exe
CC = gcc
SRC = main.c safeinput.c system.c
CFLAGS = -g 
LIBS = 

all: $(PROG)

$(PROG): $(SRC)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(SRC) $(LIBS)

clean:
	rm -f $(PROG)

.PHONY: all clean