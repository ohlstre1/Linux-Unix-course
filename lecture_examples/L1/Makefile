
CFLAGS=-Wall -pedantic

all: hello

hello: hello.o library.o
	gcc $^ -o $@	

.PHONY: clean

clean:
	rm -rf *.o *~

