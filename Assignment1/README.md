
Testbench and a mockup implementation for Assignment #1
-------------------------------------------------------

Mockup (as1_mockup.c) is a simple program that parses command line and just
copies the input to output instead of transmitting it using signals.

Testbench is a helper program that generates random gibberish in Morse
combatible alphabet and runs it through the tested program ($PROGRAM in
Makefile).  It tests short and medium length files (16 and 1024 bytes
respectively).  If you want to repeat a particular random input, you can
specify the seed of that run with command line argument "-s <num>".

If you give "-l" argument, very long test is also run with 1MB input size.


## To make program run

$ make

##to test sending information run

$ ./as1_mockup input.txt output.tx

## to Run testbed

$ ./as1_testbench -n 1 -s 1234 ./as1_mockup
