
PROGS=lock mpx-blocks  mpx-nonblock  mpx-select mpx-poll mpx-aio mpx-aiosig mpx-fork writer writersync
CC=gcc
CFLAGS=-Os -Wall -g
all: ${PROGS}

mpx-aio: mpx-aio.c
	${CC} ${CFLAGS} ${LDFLAGS} mpx-aio.c -o mpx-aio -lrt

mpx-aiosig: mpx-aiosig.c
	${CC} ${CFLAGS} ${LDFLAGS} mpx-aiosig.c -o mpx-aiosig -lrt

clean:
	rm -f ${PROGS} *~
