
PROGS=fifo ip_server pipe rtssignal uclient userver
CC=gcc
#CC=clang
CFLAGS=-Wall -g -Os
all: ${PROGS}

mreceive_posix: mreceive_posix.c
	${CC} ${CFLAGS} ${LDFLAGS} mreceive_posix.c -o mreceive_posix -lrt

msend_posix: msend_posix.c
	${CC} ${CFLAGS} ${LDFLAGS} msend_posix.c -o msend_posix -lrt

clean:
	rm -f ${PROGS} *~
