// Header Prototypes
int * rearrange_key(char *, int);
char * single_column_transposition(char *, int, int *, int, int);
void capitalize(char *, int);
void DCT(int);

// Implement functions
void DCT(int mode) {
    // Prompt for
    // - plaintext/ciphertext size
    // - plaintext/ciphertext
    // - key1 size
    // - key1
    // - key2 size
    // - key2

    char *text, *dct_1, *dct_2, *key1, *key2;
    int *key1_sequential, *key2_sequential;
    int text_size, key1_size, key2_size;

    printf("Please provide the following information:\n");
    if(mode == 0) { printf(" - plaintext size\n - plaintext\n");}
    else { printf(" - ciphertext size\n - ciphertext\n");}
    printf(" - key1 size\n - key1\n - key2 size\n - key2\n");

    if(mode == 0) { printf("\nPlaintext size: "); }
    else { printf("\nCiphertext size: "); }
    scanf("%d", &text_size);
    text = (char *)malloc(text_size + 1);

    if(mode == 0) { printf("Plaintext: "); }
    else { printf("Ciphertext: "); }
    scanf("%s", text);

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

    // Capitalize if necessary
    capitalize(text, text_size);

    // Convert keys into digits and rearrange to sequential order
    key1_sequential = rearrange_key(key1, key1_size);
    key2_sequential = rearrange_key(key2, key2_size);

    // Perform DCT by running two sequential instances of single column transposition
    if(mode == 0) { // Encrypt
        dct_1 = single_column_transposition(text, text_size, key1_sequential, key1_size, 0);
        dct_2 = single_column_transposition(dct_1, text_size, key2_sequential, key2_size, 0);
    } else { // Decrypt
        dct_1 = single_column_transposition(text, text_size, key2_sequential, key2_size, 1);
        dct_2 = single_column_transposition(dct_1, text_size, key1_sequential, key1_size, 1);
    }

    // Display the encrypted or decrypted text
    printf("\n\nHere is your provided ");
    if(mode == 0) {printf("plaintext");}
    else {printf("ciphertext");}
    printf(": %s", text);

    if(mode == 2) {
        printf("\n\nIntermediate Ciphertext: %s\n", dct_1);
    }

    printf("\nHere is your ");
    if(mode == 0) {printf("encrypted ciphertext");}
    else {printf("decrypted plaintext");}
    printf(": %s\n", dct_2);

    // Free memory
    free(text);
    free(dct_1);
    free(dct_2);
    free(key1);
    free(key2);
    free(key1_sequential);
    free(key2_sequential);
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
    
    int *key_sequential = (int *)malloc((keysize + 1) * sizeof(int));

    int j = 0;
    for(int letter_search = 0; letter_search < 26 && j < keysize; ++letter_search) { // Goes through each letter since duplicates may occur
        for(int i = 0; i < keysize && j < keysize; ++i) { // Find letters from left to right in the key
            if(key[i] == letter_search) { // map positions to sequential order
                key_sequential[j] = i;
                j++;
            }
        }
    }

    return key_sequential;
}
char * single_column_transposition(char *p, int p_size, int *k_seq, int k_size, int mode) {
    // Modes: 0 is encrypt, 1 is decrypt
    char *output = (char *)malloc(p_size + 1);

    int i = 0;
    for(int k = 0; k < k_size; ++k) {
        for(int j = k_seq[k]; j < p_size; j += k_size) { // Go down column
            if(mode == 0) { // Encrypt by writing out column-by-column
                output[i++] = p[j];
            } else { // Decrypt by filling column
                output[j] = p[i++];
            }
        }
    }

    output[p_size] = '\0'; // Just in case

    return output;
}

void capitalize(char * text, int length) {
    // Capitalizes text in place
    for(int i = 0; i < length; ++i) {
        if(text[i] >= 'a' && text[i] <= 'A') {
            text[i] -= 'a' - 'A';
        }
    }

    // TODO: Consider removing spaces or periods, if that is necessary
}
