#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// Function Prototypes
int chooseMode();
void encrypt();
void decrypt();
void decrypt_with_crib();
int * rearrange_key(char *, int);
char * single_column_transposition(char *, int, int *, int);


int main() {
    printf("Welcome to the Double Column Transposition tool!\n");

    switch(chooseMode()) {
        case(0):
            encrypt();
            break;
        case(1):
            decrypt();
            break;
        case(2):
            decrypt_with_crib();
            break;
    }

    return 0;
}

int chooseMode() {
    int mode = 0;

    // Prompt user to pick one of three modes
    printf("Pick one of the following three options:\n");
    printf("0) encrypt\n");
    printf("1) decrypt\n");
    printf("2) decrypt with crib\n");
    printf("\nDesired Mode: ");
    scanf("%d", &mode);

    while (mode < 0 || mode > 2) {
        printf("Sorry, that is an invalid mode. Please choose again.\n");
        printf("Desired Mode: ");
        scanf("%d", &mode);
    }

    return mode;
}

void encrypt() {
    // Prompt for
    // - plaintext size
    // - plaintext
    // - key1 size
    // - key1
    // - key2 size
    // - key2

    char *plaintext, *cipher_1, *cipher_2, *key1, *key2;
    int *key1_sequential, *key2_sequential;
    int plaintext_size, key1_size, key2_size;

    printf("Please provide the following information:\n");
    printf(" - plaintext size\n - plaintext\n - key1 size\n - key1\n - key2 size\n - key2\n");

    printf("\nPlaintext size: ");
    scanf("%d", &plaintext_size);
    plaintext = (char *)malloc(plaintext_size + 1);
    // cipher_1  = (char *)malloc(plaintext_size + 1);
    // cipher_2  = (char *)malloc(plaintext_size + 1);

    printf("Plaintext: ");
    scanf("%s", plaintext);

    printf("Key1 size: ");
    scanf("%d", &key1_size);
    key1 = (char *)malloc(key1_size + 1);

    printf("Key1: ");
    scanf("%s", key1);

    printf("Key2 size: ");
    scanf("%d", &key2_size);
    key2 = (char *)malloc(key2_size + 1);

    printf("Key2: ");
    scanf("%s", key2);

    // Convert keys into digits and rearrange to sequential order
    key1_sequential = rearrange_key(key1, key1_size);
    key2_sequential = rearrange_key(key2, key2_size);

    // Encrypt the text
    cipher_1 = single_column_transposition(plaintext, plaintext_size, key1_sequential, key1_size);
    cipher_2 = single_column_transposition(cipher_1, plaintext_size, key2_sequential, key2_size);

    // Display the encrypted ciphertext
    printf("\n\nHere is your plaintext: %s", plaintext);
    printf("\nHere is your encrypted ciphertext: %s\n", cipher_2);

    // Free memory
    free(plaintext);
    free(cipher_1);
    free(cipher_2);
    free(key1);
    free(key2);
    free(key1_sequential);
    free(key2_sequential);
}
void decrypt() {
    // Prompt for
    // - ciphertext size
    // - ciphertext
    // - key1 size
    // - key1
    // - key2 size
    // - key2
}
void decrypt_with_crib() {
    // Prompt for
    // - ciphertext size
    // - ciphertext
    // - crib size
    // - crib
    // - key1 size
    // - key2 size

    char *ciphertext, *crib, *key1, *key2;
    int ciphertext_size, crib_size, key1_size, key2_size;

    printf("Please provide the following information:\n");
    printf(" - ciphertext size\n - ciphertext\n - crib size\n - crib\n - key1 size\n - key2 size\n");

    printf("\nCiphertext size: ");
    scanf("%d", &ciphertext_size);
    ciphertext = (char *)malloc(ciphertext_size + 1);

    printf("Ciphertext: ");
    scanf("%s", ciphertext);

    printf("Crib size:");
    scanf("%d", &crib_size);
    crib = (char *)malloc(crib_size + 1);

    printf("Crib: ");
    scanf("%s", crib);

    printf("Key1 size: ");
    scanf("%d", &key1_size);
    key1 = (char *)malloc(key1_size + 1);

    printf("Key2 size: ");
    scanf("%d", &key2_size);
    key2 = (char *)malloc(key2_size + 1);

    // Perform decryption with crib

    // Free memory
    free(ciphertext);
    free(crib);
    free(key1);
    free(key2);
}
int * rearrange_key(char *key, int keysize) {
    // Assumption - keys may have duplicate values, are always A-Z
    for(int i = 0; i < keysize; ++i) { // change to integers
        if(key[i] >= 'a' && key[i] <= 'z') {
            key[i] -= 'a';
        } else if(key[i] >= 'A' && key[i] <= 'Z') {
            key[i] -= 'A';
        }
    }

    // Map appropriately
    int *key_sequential = (int *)malloc((keysize + 1) * sizeof(int));
    int j = 0;
    for(int letter_search = 0; letter_search < 26 && j < keysize; ++letter_search) {
        for(int i = 0; i < keysize && j < keysize; ++i) {
            if(key[i] == letter_search) {
                key_sequential[j] = i;
                j++;
            }
        }
    }

    return key_sequential;
}
char * single_column_transposition(char *p, int p_size, int *k_seq, int k_size) {
    char *cipher = (char *)malloc(p_size + 1);

    int i = 0;
    for(int k = 0; k < k_size; ++k) {
        int c = k_seq[k]; // which column to take first
        for(int j = k_seq[k]; j < p_size; j += k_size) {
            cipher[i++] = p[j];
        }
    }

    return cipher;
}


































// end padding for text-editor scroll
/*

Notes:
0
6
ABCDEF
3
ABC
3
ABC

ABCDEF
ABC
DEF

ADBECF
ADB
ECF

Ciphertext: AEDCBF

*/
