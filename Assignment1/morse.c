#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char character;
    char *code;
} MorseCode;

MorseCode morseTable[] = {
    {'A', ".-"},
    {'B', "-..."},
    {'C', "-.-."},
    {'D', "-.."},
    {'E', "."},
    {'F', "..-."},
    {'G', "--."},
    {'H', "...."},
    {'I', ".."},
    {'J', ".---"},
    {'K', "-.-"},
    {'L', ".-.."},
    {'M', "--"},
    {'N', "-."},
    {'O', "---"},
    {'P', ".--."},
    {'Q', "--.-"},
    {'R', ".-."},
    {'S', "..."},
    {'T', "-"},
    {'U', "..-"},
    {'V', "...-"},
    {'W', ".--"},
    {'X', "-..-"},
    {'Y', "-.--"},
    {'Z', "--.."},
    {'0', "-----"},
    {'1', ".----"},
    {'2', "..---"},
    {'3', "...--"},
    {'4', "....-"},
    {'5', "....."},
    {'6', "-...."},
    {'7', "--..."},
    {'8', "---.."},
    {'9', "----."},
    {'.', ".-.-.-"},
    {',', "--..--"},
    {'?', "..--.."},
    {'\'', ".----."},
    {'!', "-.-.--"},
    {'/', "-..-."},
    {'(', "-.--."},
    {')', "-.--.-"},
    {'&', ".-..."},
    {':', "---..."},
    {';', "-.-.-."},
    {'=', "-...-"},
    {'+', ".-.-."},
    {'-', "-....-"},
    {'_', "..--.-"},
    {'"', ".-..-."},
    {'$', "...-..-"},
    {'@', ".--.-."},
    {' ', "/"}
};



char* encodeToMorse(char input) {
    for (int i = 0; i < sizeof(morseTable) / sizeof(MorseCode); i++) {
        if (toupper(input) == morseTable[i].character) {
            return morseTable[i].code;
        }
    }
    return NULL; // Not found in table
}

void stringToMorse(char* str, char* output) {
    while (*str) {
        char* morse = encodeToMorse(*str);
        if (morse) {
            strcat(output, morse);
            strcat(output, " "); // Separate Morse characters
        }
        str++;
    }
}


char decodeFromMorse(char* code) {
    for (int i = 0; i < sizeof(morseTable) / sizeof(MorseCode); i++) {
        if (strcmp(code, morseTable[i].code) == 0) {
            return morseTable[i].character;
        }
    }
    return '\0'; // Not found in table
}

void morseToString(char* morse, char* output) {
    char* token = strtok(morse, " ");
    while (token) {
        char decodedChar = decodeFromMorse(token);
        if (decodedChar) {
            *output = decodedChar;
            output++;
        }
        token = strtok(NULL, " ");
    }
    *output = '\0'; // Null-terminate the decoded string
}


