char * decrypt_partial(char * ciphertext, int ciphertext_size, char * key2_putative, int key2_size, int key1_size);
int partial_score(char * possible_intermediate_ciphertext, int ciphertext_size, char * crib, int crib_size, int key2_size, int key1_size);



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

int get_first_col(char * ciphertext, int ciphertext_size, int * column_frequencies, int key2_size, int key1_size){
    // Returns the best matching column from the first DCT's matrix
	int min_colsize= ciphertext_size/key2_size;

	int *matches= (int *) malloc(key1_size*sizeof(int));
	for(int i=0;i<key1_size;i++){
		matches[i]=0;
	}

    // Make copy of column_freqencies to modify
    int *column_freq = (int *)malloc(key1_size*26*sizeof(int));
    for(int i = 0; i < key1_size*26; ++i) {
        column_freq[i] = column_frequencies[i];
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

char * get_key2_putative(char * ciphertext, int ciphertext_size, char * crib, int crib_size, int most_likely_first_column, int * column_frequencies, int key1_size, int key2_size) {
    int min_colsize = ciphertext_size/key2_size;

    char * key2 = (char *)malloc(key2_size);
    for(int i = 0; i < key2_size; ++i) {
        key2[i] = -1;
    }

    for(int i = 0; i < key2_size; ++i) {
        // Find characters that are only found once in column_frequencies
        int letter = ciphertext[min_colsize*i] - 'A';
        if(column_frequencies[most_likely_first_column*26+letter] == 1) {
            // Find letter in crib column
            int pos = 0;
            for(int j = most_likely_first_column; j < crib_size && crib[j] != letter + 'A'; j += key1_size, ++pos);

            key2[pos] = i;
        }
    }

    return key2;
}

// void swap (int *x, int *y)
// {
//     int temp;
//     temp = *x;
//     *x = *y;
//     *y = temp;
// }
//
//
//
// void heap_permute(char * ciphertext, int ciphertext_size, int * v, int ns) {
//     int * c = (int *)malloc(n * sizeof(int));
//     for(int j = 0; j < n; ++n) {
//         c[j] = 0;
//     }
//
//     // output(v)
//
//     for (int i = 0; i < n; i++) {
//         if(c[i] < i) {
//                 if (n % 2 == 0) {
//                     swap(&v[0], &v[i]);
//         	    }
//                 else {
//                     swap(&v[c[i]], &v[i]);
//                 }
//                 // output(v)
//                 c[i] += 1;
//                 i = 0;
//             }
//     	}
//         else {
//             c[i] = 0;
//             i += 1;
//         }
//     }
// }

char * key2_find_permutations(char * ciphertext, int ciphertext_size, char * crib, int crib_size, char * key2_putative, int key1_size, int key2_size, int * column_frequencies, int key2_col_match) {
    int min_colsize = ciphertext_size/key2_size;

    for(int i = 0, p = 0; i < key2_size; ++i) {
        int letter = ciphertext[p] - 'A';
        int count = column_frequencies[key2_col_match*26+letter];
        // if(count <= 1) {
        //     // Ignore and move on if they're unique instances. This means we don't need to worry about them.
        //     continue;
        // }

        // Find all instances of the letter in the matched column
        // printf("%c is found in matched column at locations [", letter+'A');
        int * letter_instances = (int *)malloc(count * sizeof(int));
        for(int j = key2_col_match, k = 0, a = 0; j < crib_size; j += key1_size, ++a) {
            if(crib[j] - 'A' == letter) {
                letter_instances[k++] = a;
                // printf("%d ", a);
            }
        }
        // printf("]\n");

        // Find which instance of letter is the actual one by scoring the deciphered ciphertext
        int score = 0;
        int best_pos = -1;
        // int key2_putative_x[16] = {12,4,2,15,7,5,13,3,1,8,0,14,6,10,9,11};
        for(int i_in_col = 0; i_in_col < count; ++i_in_col) {
            key2_putative[letter_instances[i_in_col]] = i;
            // char * possible_intermediate_ciphertext = decrypt_partial(ciphertext, ciphertext_size, key2_putative, key2_size, key1_size);
            // int score = partial_score(possible_intermediate_ciphertext, ciphertext_size, crib, crib_size, key2_size, key1_size);
            printf("Begin\n");
            printf("ciphertext: %d %s\n", ciphertext_size, ciphertext);
            int * key2_sequential = rearrange_key(key2_putative, key2_size);
            char * intermediate = single_column_transposition(ciphertext, ciphertext_size, key2_putative, key2_size, 1);
            printf("%s\n", intermediate);
            key2_putative[letter_instances[i_in_col]] = -1; // reset
        }

        // TODO: REMOVE THIS BREAK STATEMENT
        break;

        // Increment p properly
        p += min_colsize;
        // if(i > -1 && key2_putative[i] < ciphertext_size % 16) {
        //     ++p;
        //     printf("true for %d, %d", key2_putative[i], ciphertext_size % 16);
        // }
        free(letter_instances);
    }

    return key2_putative;
}

char * decrypt_partial(char * ciphertext, int ciphertext_size, char * key2_putative_x, int key2_size, int key1_size) {
    char *output = (char *)malloc(ciphertext_size + 1);
    for(int j = 0; j < ciphertext_size; ++j) {
        output[j] = ' ';
    }

    char key2_putative[16] = {'M'-'A',
                            'E'-'A',
                            'C'-'A',
                            'P'-'A',
                            'H'-'A',
                            'F'-'A',
                            'N'-'A',
                            'D'-'A',
                            'B'-'A',
                            'I'-'A',
                            'A'-'A',
                            'O'-'A',
                            'G'-'A',
                            'K'-'A',
                            'J'-'A',
                            'L'-'A'};

    printf("key2_putative: ");
    for(int k = 0; k < key2_size; ++k){
        printf("%d ", key2_putative[k]);
    }
    printf("\n");


    // int i = 0;
    // for(int k = 0; k < key2_size; ++k) {
    //     int m = key2_putative[k];
    //     for(int j = m; j < ciphertext_size; j += key2_size) { // Go down column
    //         // if(j == -1) {
    //         //     i += ciphertext_size / key2_size;
    //         //     if(k <= ciphertext_size % key2_size) {
    //         //         ++i;
    //         //     }
    //         //     break;
    //         // }
    //         if(j == -1) {
    //             j = 0;
    //         }
    //         if(m == -1) {
    //             i++;
    //         }
    //         else{
    //             output[j] = ciphertext[i++];
    //         }
    //     }
    // }

    int mode = 1;
    int i = 0;
    for(int k = 0; k < key2_size; ++k) {
        for(int j = key2_putative[k]; j < ciphertext_size; j += key2_size) { // Go down column
            if(mode == 0) { // Encrypt by writing out column-by-column
                output[i++] = ciphertext[j];
            } else { // Decrypt by filling column
                output[j] = ciphertext[i++];
            }
        }
    }

    printf("Intermediate Ciphertext is\n%s\n", output);

    output[ciphertext_size] = '\0'; // Just in case

    return output;
}
int partial_score(char * possible_intermediate_ciphertext, int ciphertext_size, char * crib, int crib_size, int key2_size, int key1_size) {
    return 0;
}





















// end of spacer
