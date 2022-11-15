
#define BLOCKSIZE 4096

#define ALPHABETLENGTH 37



// helper function for encoding text
// input is char and output is the equal string in morse with a empyt character at the end
char* charToMorse(char character);

// Encodes text and copyes it into buf
// takes in buffer and encodes it to morse
void morseEncode( char *out, char *buf);

// helper function to MorseDecode
// input a string that is one letter in morsecode - output
char morseToChar(char string[]);

// Decodes text outputs it in output
void morseDecode(char *output, char *buf);