#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <assert.h>
#include <ctype.h>
#include "morse.h"

#define BLOCKSIZE 4096

#define ALPHABETLENGTH 37

char *morseAlphabet[] = {
    ".- ",   //A
    "-... ", //B
    "-.-. ", //C
    "-.. ",  //D
    ". ",    //E
    "..-. ", //F
    "--. ",  //G
    ".... ", //H
    ".. ",   //I
    ".--- ", //J
    "-.- ",  //K
    ".-.. ", //L
    "-- ",   //M
    "-. ",   //N
    "--- ",  //O
    ".--. ", //P
    "--.- ", //Q
    ".-. ",  //R
    "... ",  //S
    "- ",    //T
    "..- ",  //U
    "...- ", //V
    ".-- ",  //W
    "-..- ", //X
    "-.-- ", //Y
    "--.. ", //Z
    ".---- ", //1
    "..--- ", //2
    "...-- ", //3
    "....- ", //4
    "..... ", //5
    "-.... ", //6
    "--... ", //7
    "---.. ", //8
    "----. ", //9
    "----- ", //0
};

const char alphabet[] = {
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0', 
};



// helper function for encoding text
// input is char and output is the equal string in morse with a empyt character at the end
char* charToMorse(char character){
    char ch = toupper(character);
    for( int i = 0; i < ALPHABETLENGTH; i++){
        if( alphabet[i] == ch){
            return morseAlphabet[i];
        }
    }
    return "error";
}

// Encodes text and copyes it into buf
// takes in buffer and encodes it to morse
void morseEncode( char *out, char *buf){
    int len = strlen(buf);
    unsigned int count = 0;
    char *subbuff = malloc(10*sizeof(char));
    int idx_output = 0;
    
    char *ch;
    char *ch_new_word; 

    for( int i = 0; i < len; i++){
        if(buf[i] == ' ' || i == len-1){
            memcpy(subbuff, &buf[(i-count)], count);

            for( int j = 0; j < count; j++){
                ch = charToMorse(subbuff[j]);
                memcpy(&out[idx_output], ch, strlen(ch));
                idx_output += (strlen(ch));
            }

            ch_new_word = "/ ";
            memcpy(&out[idx_output], ch_new_word, strlen(ch_new_word));
            idx_output += (strlen(ch_new_word));
            count = 0;

        }else if(buf[i] == '\n'){
            memcpy(subbuff, &buf[(i-count)], count);

            for( int j = 0; j < count; j++){
                ch = charToMorse(subbuff[j]);
                memcpy(&out[idx_output], ch, strlen(ch));
                idx_output += (strlen(ch));
            }
            
            ch_new_word = "\n";
            memcpy(&out[idx_output], ch_new_word, strlen(ch_new_word));
            idx_output += (strlen(ch_new_word));
            count = 0;
        }else{
            count++;
        }
        
    }
    free(subbuff);
    memcpy(buf, out, BLOCKSIZE);
    
    
}

// helper function to MorseDecode
// input a string that is one letter in morsecode - output
char morseToChar(char string[]){
    for( int i = 0; i < ALPHABETLENGTH; i++){
        //printf("%sx%s\n", string, morseAlphabet[i]); 
        if(strcmp(string, morseAlphabet[i]) == 0){
            return tolower(alphabet[i]);
        }        
    }

    return '!';
}

// Decodes text outputs it in output
void morseDecode(char *output, char *buf){
    int len = strlen(buf);
    unsigned int count = 0;
    char *subbuff = malloc(10*sizeof(char));
    int idx_output = 0;


    char ch;
    char *ch_new_word;
    for( int i = 0; i < len; i++){
        if(buf[i] == ' ' || i == len-1){
            subbuff = calloc(*subbuff,(count+1)*sizeof(char));
            memcpy(subbuff, &buf[(i-count)], count+1);
            ch = morseToChar(subbuff);
            memcpy(&output[idx_output], &ch, 1);
            idx_output ++;
            count = 0;
        }else if(buf[i] == '\n'){
            ch_new_word = "\n";
            memcpy(&output[idx_output], ch_new_word, 1);
            idx_output += 1;
            count = 0;
        }
        else{
            count++;
        }
        
    }
    for( int i = 0; i < len; i++){
        if( output[i] == '!')
        output[i] = ' ';
    }

    free(subbuff);

}