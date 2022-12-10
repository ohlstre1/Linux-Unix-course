
CFLAGS=-Os -g -Wall `pkg-config --cflags gtk+-3.0`
LDLIBS=`pkg-config --libs gtk+-3.0`
BINS = example-1 example-2 example-4

all: ${BINS}

clean:
	rm -rf *~ ${BINS}

