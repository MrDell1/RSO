CFLAGS=-ggdb -Wall -pedantic -pthread

%: %.c
	gcc $(CFLAGS) $< -o $@ -lm

EXECS = server3 client3 

all: $(EXECS)

server3: server3.c

client3: client3.c

.PHONY: clean all

clean:
	rm -f $(EXECS)
