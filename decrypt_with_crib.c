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

    // Capitalize letters if necessary
    capitalize(ciphertext, ciphertext_size);
    capitalize(crib, crib_size);

    // Perform decryption with crib

    // Free memory
    free(ciphertext);
    free(crib);
    free(key1);
    free(key2);
}


int * get_column_frequency(char * crib, int crib_size, int key1_size){
    // Creates a table of frequencies for letters A-Z for easy lookup and compare
	int *column_freq = (int *) malloc(key1_size*26*sizeof(int));

	for(int i=0;i<key1_size*26;i++){
		column_freq[i] = 0;
	}

	for(int k = 0; k < key1_size; ++k) {
        for(int j = k; j < crib_size; j += key1_size) { // Go down column
           int letter = crib[j]-'A';
           column_freq[k*26+letter]++;
        }
    }

    return column_freq;
}

int get_first_col(char * ciphertext, int ciphertext_size, int * column_freq, int key2_size, int key1_size){
    // Returns the best matching column from the first DCT's matrix
	int min_colsize= ciphertext_size/key2_size;
	int *matches= (int *) malloc(key1_size*sizeof(int));

	for(int i=0;i<key1_size;i++){
		matches[i]=0;
	}

	for(int i=0;i<ciphertext_size;i+=min_colsize){
		int letter = ciphertext[i]-'A';
        printf("%c ", ciphertext[i]);
		for(int k=0;k<key1_size;k++){
			if(column_freq[k*26+letter]>0){
				column_freq[k*26+letter]--;
				matches[k]++;
			}
		}
        printf("\n");
	}

	int max_pos=0;

	for(int i=1;i<key1_size;i++){
		if(matches[i]>matches[max_pos]){
			max_pos=i;
		}
	}

	return max_pos;
}
