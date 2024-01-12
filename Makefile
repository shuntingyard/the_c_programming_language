CFLAGS = "-ansi"
SRC = $(wildcard *.c)
EXE = $(patsubst %.c, %, $(SRC))

all: $(EXE)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm $(EXE) *.o
