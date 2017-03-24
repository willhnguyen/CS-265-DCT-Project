// Header Prototypes
int * get_column_frequency(char *, int, int);
int get_first_col(char *, int, int *, int, int);
int * get_key2_putative(char *, int, char *, int, int, int *, int, int);
int * rearrange_key_putative(int *, int);
int * key2_find_permutations(char *, int, char *, int, int *, int, int, int *, int);
char * decrypt_partial(char *, int, int *, int, int);
int partial_score(char *, int, char *, int, int, int, int);
int * find_key1(char *, int, char *, int, int *, int, int);
void printkey(int *, int);
void decrypt_with_crib();

// Implement Functions
void decrypt_with_crib() {
    // Prompt for
    // - ciphertext size
    // - ciphertext
    // - crib size
    // - crib
    // - key1 size
    // - key2 size

    char *ciphertext, *crib;
    int *key1, *key2;
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

    printf("Key2 size: ");
    scanf("%d", &key2_size);

    // Capitalize letters if necessary
    capitalize(ciphertext, ciphertext_size);
    capitalize(crib, crib_size);

    // Perform decryption with crib
    int * column_frequencies = get_column_frequency(crib, crib_size, key1_size);

    // Find the column that matches the first part of key2 before proceeding
    int key2_col_match = get_first_col(ciphertext, ciphertext_size, column_frequencies, key2_size, key1_size);

    // Find key2 and key1
    key2 = get_key2_putative(ciphertext, ciphertext_size, crib, crib_size, key2_col_match, column_frequencies, key1_size, key2_size);
    key2 = key2_find_permutations(ciphertext, ciphertext_size, crib, crib_size, key2, key1_size, key2_size, column_frequencies, key2_col_match);
    key1 = find_key1(ciphertext, ciphertext_size, crib, crib_size, key2, key2_size, key1_size);

    printf("\n\nFound key1 to be: ");
    printkey(key1, key1_size);
    printf("\nFound key2 to be: ");
    printkey(key2, key2_size);
    printf("\n");

    // Free memory
    free(ciphertext);
    free(crib);
    free(key1);
    free(key2);
    free(column_frequencies);
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
		for(int k=0;k<key1_size;k++){
			if(column_freq[k*26+letter]>0){
				column_freq[k*26+letter]--;
				matches[k]++;
			}
		}
	}

	int max_pos=0;

	for(int i=1;i<key1_size;i++){
		if(matches[i]>matches[max_pos]){
			max_pos=i;
		}
	}

  free(matches);

	return max_pos;
}

int * get_key2_putative(char * ciphertext, int ciphertext_size, char * crib, int crib_size, int most_likely_first_column, int * column_frequencies, int key1_size, int key2_size) {
    int min_colsize = ciphertext_size/key2_size;

    int * key2 = (int *)malloc(key2_size*sizeof(int));
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

int * rearrange_key_putative(int *key, int keysize) {
    // Map appropriately
    int *key_sequential = (int *)malloc((keysize + 1) * sizeof(int));
    for(int i = 0; i < keysize; ++i) {
        key_sequential[i] = -1;
    }

    int j = 0;
    for(int i = 0; i < keysize; ++i) {
        if(key[i] != -1) {
            key_sequential[key[i]] = i;
        }
    }

    return key_sequential;
}

int * key2_find_permutations(char * ciphertext, int ciphertext_size, char * crib, int crib_size, int * key2_putative, int key1_size, int key2_size, int * column_frequencies, int key2_col_match) {
    int min_colsize = ciphertext_size/key2_size;

    // Find putative key for known values
    for(int i = 0, p = 0; i < key2_size; ++i) {
        int letter = ciphertext[p] - 'A';
        int count = column_frequencies[key2_col_match*26+letter];


        // Find all instances of the letter in the matched column
        int * letter_instances = (int *)malloc(count * sizeof(int));
        int k = 0;
        for(int j = key2_col_match, a = 0; j < crib_size; j += key1_size, ++a) {
            if(crib[j] - 'A' == letter && key2_putative[a] == -1) {
                letter_instances[k++] = a;
            }
        }

        // Find which instance of letter is the actual one by scoring the deciphered ciphertext
        int score = 0;
        int best_pos = -1;
        for(int i_in_col = 0; i_in_col < k; ++i_in_col) {
            key2_putative[letter_instances[i_in_col]] = i;

            int * key2_sequential = rearrange_key_putative(key2_putative, key2_size);
            char * possible_intermediate_ciphertext = decrypt_partial(ciphertext, ciphertext_size, key2_sequential, key2_size, key1_size);
            int curr_score = partial_score(possible_intermediate_ciphertext, ciphertext_size, crib, crib_size, key2_size, key1_size, key2_col_match);
            key2_putative[letter_instances[i_in_col]] = -1; // reset

            if (curr_score > score) {
              score = curr_score;
              best_pos = letter_instances[i_in_col];
            }

            // Problem: Freeing these cause core dumps and segmentation faults on Linux
            free(key2_sequential);
            free(possible_intermediate_ciphertext);
        }
        // Update key value here
        if(best_pos != -1)
          key2_putative[best_pos] = i;
        --column_frequencies[key2_col_match*26+letter];


        // Increment p properly
        p += min_colsize;
        if(best_pos > -1 && best_pos < ciphertext_size % 16) {
          // Best position represents the column of the crib's SCT matrix
            ++p;
        }
        free(letter_instances);
    }

    // Find putative key with unknown values
    int count = 0;
    for(int i = 0; i < key2_size; ++i) {
        if (key2_putative[i] == -1) {
          ++count;
        }
    }

    // Find non-filled positions in the putative key
    int * missing_positions = (int *) malloc (count * sizeof(int));
    for(int i = 0, j = 0; i < key2_size; ++i) {
        if (key2_putative[i] == -1) {
            missing_positions[j++] = i;
        }
    } // determined missing positions
    // TODO: Guestimate each missing position until the entire key is found
    int * possible_positions_for_missing = (int *) malloc (count * sizeof(int));
    int * found = (int *) malloc(key2_size * sizeof(int)); // Keep track of which values are in key2
    for(int i = 0; i < key2_size; ++i) {
        if(key2_putative[i] != -1) {
            found[key2_putative[i]] = 1;
        } else {
            found[key2_putative[i]] = 0;
        }
    }
    for(int i = 0, j = 0; i < key2_size; ++i) {
        if(found[i] == 0) {
            possible_positions_for_missing[j++] = i;
        }
    }
    for(int i = 0; i < count; ++i) {
        // Test a value for the putative key
        int score = 0;
        int best_pos = -1;
        for(int j = 0; j < count; ++j) {
            key2_putative[missing_positions[i]] = possible_positions_for_missing[j];
            int * key2_sequential = rearrange_key_putative(key2_putative, key2_size);
            char * possible_intermediate_ciphertext = decrypt_partial(ciphertext, ciphertext_size, key2_sequential, key2_size, key1_size);
            int curr_score = partial_score(possible_intermediate_ciphertext, ciphertext_size, crib, crib_size, key2_size, key1_size, key2_col_match);
            key2_putative[missing_positions[i]] = -1; // reset

            free(key2_sequential);
            free(possible_intermediate_ciphertext);

            if (curr_score > score) {
              score = curr_score;
              best_pos = possible_positions_for_missing[j];
            }
        }

        if(best_pos != -1) {
            key2_putative[missing_positions[i]] = best_pos;
        }
    }

    free(missing_positions);
    free(possible_positions_for_missing);
    free(found);

    return key2_putative;
}

char * decrypt_partial(char * ciphertext, int ciphertext_size, int * key2_putative, int key2_size, int key1_size) {
    char *output = (char *)malloc(ciphertext_size + 1);
    for(int j = 0; j < ciphertext_size; ++j) {
        output[j] = ' ';
    }

    int i = 0;
    for(int k = 0; k < key2_size; ++k) {
        int m = key2_putative[k];
        for(int j = m; j < ciphertext_size; j += key2_size) { // Go down column
            if(j == -1) {
                i += ciphertext_size / key2_size;
                if(key2_putative[k] > -1 && key2_putative[k] < ciphertext_size % key2_size) {
                    ++i;
                }
                break;
            }
            output[j] = ciphertext[i++];

        }
    }

    output[ciphertext_size] = '\0'; // Just in case

    return output;
}

int partial_score(char * possible_intermediate_ciphertext, int ciphertext_size, char * crib, int crib_size, int key2_size, int key1_size, int key2_col_match) {
    // Try to find the crib's column substrings in the ciphertext
    int score = 0;

    int min_col_size_key1 = ciphertext_size / key1_size;
    int offset_size = ciphertext_size % key1_size;

    // Ignore the first column
    int i = min_col_size_key1;
    if(key2_col_match < offset_size) {
      ++i;
    }

    // For each place in intermeidate ciphertext where we expect a column substring, match the substring and increase score count
    // Just compare it with the next column location, otherwise risk it taking longer
    int best_score_partial = 0, best_score_col = -1;
    for(int k = 0; k < key1_size; ++k) { // Check for each column
        int score_partial = 0;
        for(int j = 0; j < crib_size / key1_size; ++j) {
          if(possible_intermediate_ciphertext[i + j] == crib[k + j * key1_size]) {
            ++score_partial;
          }
        }

        if (score_partial > best_score_partial) {
          best_score_partial = score_partial;
          best_score_col = k;
        }
    }
    score += best_score_partial;

    return score;
}

int * find_key1(char * ciphertext, int ciphertext_size, char * crib, int crib_size, int * key2, int key2_size, int key1_size) {
    // Find the first key given the deciphered key2
    int * rearranged_key2 = rearrange_key_putative(key2, key2_size);
    char * intermediate_ciphertext = single_column_transposition(ciphertext, ciphertext_size, rearranged_key2, key2_size, 1);
    int min_col_size_key1 = ciphertext_size / key1_size;
    int key1_col_offset_limit = ciphertext_size % key1_size;

    int * key1 = (int *) malloc (key1_size * sizeof(int));
    for(int i = 0; i < key1_size; ++i) {
        key1[i] = -1;
    }

    for(int i = 0, col = 0; i < ciphertext_size && col < key1_size; ++col) {
        int most_likely_column = -1;
        int score = 0;

        // Do checks
        for(int j = 0; j < key1_size; ++j) {
            int curr_score = 0;
            // Compare first matrix columns to values here;
            for(int k = j, m = 0; k < crib_size && i+m < ciphertext_size; k += key1_size, ++m) {
                if(crib[k] == intermediate_ciphertext[i+m]) {
                    ++curr_score;
                }
            }

            if(curr_score > score) {
                score = curr_score;
                most_likely_column = j;
            }
        }

        // Build key1
        key1[most_likely_column] = col;

        // Increment i accordingly
        i += min_col_size_key1;
        if(most_likely_column < key1_col_offset_limit) {
            ++i;
        }
    }

    free(rearranged_key2);
    free(intermediate_ciphertext);

    return key1;
}

void printkey(int * key, int key_size) {
    for(int i = 0; i < key_size; ++i) {
        printf("%c", key[i] + 'A');
    }
}
