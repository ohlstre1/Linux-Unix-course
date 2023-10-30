
PROGRAM=as1_mockup
TESTER=as1_testbench


CFLAGS=-O2 -g -Wall -pedantic


all: ${TESTER} ${PROGRAM}

${TESTER}:

.PHONY: test
test: all
	./${TESTER} ${PROGRAM}

.PHONY: clean
clean:
	rm -rf *.o *~ ${TESTER} ${PROGRAM}
