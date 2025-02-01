#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>

struct pair {
    char *key;
    long value;
};

bool validateFile(char *filename);
int fileSize(char *filename);
void loadFrecuenciesToMemory(char *filename, struct pair *characters);
void calculateFrecuencies(char *filename, struct pair characters[256]);
void loadFrecuenciesToFile(char *filename, struct pair characters[256]);
void initializeCharacters(struct pair characters[256]);
void printFrequencyTable(struct pair characters[256]);

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");

    if (argc < 2) {
        printf("Usage: %s <frequency_file> [input_files...]\n", argv[0]);
        return 1;
    }

    struct pair characters[256];
    initializeCharacters(characters);

    char *freqFilename = argv[1];

    if (argc == 2) {
        if (validateFile(freqFilename)) {
            loadFrecuenciesToMemory(freqFilename, characters);
            printFrequencyTable(characters);
        } else {
            printf("Frequency file not found or invalid.\n");
        }
        return 0;
    }

    if (validateFile(freqFilename)) {
        loadFrecuenciesToMemory(freqFilename, characters);
    }

    for (int i = 2; i < argc; i++) {
        calculateFrecuencies(argv[i], characters);
    }

    loadFrecuenciesToFile(freqFilename, characters);
    printFrequencyTable(characters);

    for (int i = 0; i < 256; i++) {
        free(characters[i].key);
    }

    return 0;
}

void initializeCharacters(struct pair characters[256]) {
    for (int i = 0; i < 256; i++) {
        characters[i].key = (char *)malloc(sizeof(char) * 2);
        characters[i].key[0] = i;
        characters[i].key[1] = '\0';
        characters[i].value = 0;
    }
}

void calculateFrecuencies(char *filename, struct pair characters[256]) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("There was an error reading: %s\n", filename);
        return;
    }
    
    unsigned char byte;
    while (fread(&byte, sizeof(unsigned char), 1, file) == 1) {
        characters[byte].value++;
    }

    fclose(file);
    
}

void loadFrecuenciesToFile(char *filename, struct pair characters[256]) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("There was an error writing to: %s\n", filename);
        return;
    }

    for (int i = 0; i < 256; i++) {
        if (isprint((unsigned char)i)) {
            fprintf(file, "%c %ld\n", i, characters[i].value);
        } else {
            fprintf(file, "0x%02x %ld\n", (unsigned char)i, characters[i].value);
        }
    }

    fclose(file);
}

void loadFrecuenciesToMemory(char *filename, struct pair *characters) {

    FILE *file = fopen(filename,"r");
    setlocale(LC_ALL,"");                           //To show unicode chars
    long frequency;                               //Freq obtained by file
    char text[100];                                 //Store the char
    
    for (int i = 0; i < 256 && !feof(file); i++) {
        fscanf(file, "%s %ld", text, &frequency);
            if (text[0] == '0' && text[1] == 'x') {
                // Hex representation
                unsigned int hex_value;
                sscanf(text, "0x%x", &hex_value);
                characters[hex_value].value = frequency;
            } else {
                // Character representation
                characters[(unsigned char)text[0]].value = frequency;
            }
		}

    fclose(file);
}

bool validateFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return true;
    }
    return false;
}

int fileSize(char *filename) {
    int i;
    for (i = 0; filename[i] != '\0'; i++) {}
    return i;
}

void printFrequencyTable(struct pair characters[256]) {
    printf("+------+-------+----------+\n");
    printf("| Hex  | Char  | Frequency|\n");
    printf("+------+-------+----------+\n");

    for (int i = 0; i < 256; i++) {
        char symbol = (isprint(i)) ? i : ' ';
        printf("| 0x%02X | %-5c | %-8ld |\n", (unsigned char)i, symbol, characters[i].value);
    }

    printf("+------+-------+----------+\n");
}
