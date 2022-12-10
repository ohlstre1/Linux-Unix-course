
PROGS=libevent_example omp_hello omp_for omp_fib eventfd signalfd epoll-example
CFLAGS=-Wall -g
all: ${PROGS}

libevent_example: libevent_example.c
	${CC} ${CFLAGS} ${LDFLAGS} libevent_example.c -o libevent_example -levent

omp_hello: omp_hello.c
	${CC} ${CFLAGS} -fopenmp ${LDFLAGS} omp_hello.c -o omp_hello

omp_for: omp_for.c
	${CC} ${CFLAGS} -std=c99 -fopenmp ${LDFLAGS} omp_for.c -o omp_for

omp_fib: omp_fib.c
	${CC} ${CFLAGS} -fopenmp ${LDFLAGS} omp_fib.c -o omp_fib
	
clean:
	rm -f ${PROGS} *~
