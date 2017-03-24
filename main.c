#include "stdio.h"
#include "stdlib.h"
#include "string.h"


// Main Function Prototypes
int chooseMode();
void capitalize(char *, int);

// Custom headers
#include "dct_encrypt_and_decrypt.c"
#include "decrypt_with_crib.c"



int main() {
    printf("Welcome to the Double Column Transposition tool!\n");

    switch(chooseMode()) {
        case(0):
            // encrypt();
            DCT(0);
            break;
        case(1):
            // decrypt();
            DCT(1);
            break;
        case(2):
            decrypt_with_crib();
            break;
        case(3):
            DCT(2);
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

    while (mode < 0 || mode > 3) {
        printf("Sorry, that is an invalid mode. Please choose again.\n");
        printf("Desired Mode: ");
        scanf("%d", &mode);
    }

    return mode;
}
































// end padding for text-editor scroll
