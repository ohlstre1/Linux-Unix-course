
PROGS=mreceive_posix mreceive_sysv msend_posix msend_sysv shared_sysv shared_posix sema_sysv sema_posix
CC=gcc
#CC=clang
CFLAGS=-Wall -g -Os
all: ${PROGS}

mreceive_posix: mreceive_posix.c
	${CC} ${CFLAGS} ${LDFLAGS} mreceive_posix.c -o mreceive_posix -lrt

msend_posix: msend_posix.c
	${CC} ${CFLAGS} ${LDFLAGS} msend_posix.c -o msend_posix -lrt

shared_posix: shared_posix.c
	${CC} ${CFLAGS} ${LDFLAGS} $< -o $@ -lrt -pthread

sema_posix: sema_posix.c
	${CC} ${CFLAGS} ${LDFLAGS} $< -o $@ -pthread


clean:
	rm -f ${PROGS} *~
