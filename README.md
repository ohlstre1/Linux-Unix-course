#### compile program
$ make

#### clean random files
$ make clean


#### Run program
$ ./exe_morse input.txt output.txt

# Firstly we read data from input.txt 
# Execute fork() which makes a parent and child process of the same program. 
# The parent process reads the data and encodes it into morse code and sends it trough a pipe.
# The child process receives the encoded message and decodes it.
# After this child process writes the message into output.txt 

