#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// Custom headers
#include "dct_encrypt_and_decrypt.c"
#include "decrypt_with_crib.c"

int main() {
    int ciphertext_size = 657, crib_size = 194, key1_size = 13, key2_size = 16;
    char *ciphertext = "UDUOSETINENATARLNRTEEDCEMWESCNERWHTIRTRCOIVEHRHEDSFFONDDSIDSHYHMSSIERSOLDTOHSSMLCOIMAINTRBITMTURWLIGOEHHHEOFMXOOPHWTRHFLFSSVHPLDEANCANREESOIIIOLONROEIGEOWSOIEDAOEETFESASWRIIOHLIWNTMTNSENHEBISFEEFOOSITFASBRTCNNOUDMNPEUAROTTPEEOEUOMLWIIOPMNMNIAESWYAETHBAEUITOSICCEGIIELIOUOFSCNNASRENOIUENAORHWAEOLFFNHUETMRTFAEBLAEKIAAAEENDNFLMIEEOASLEOAHIAKOUOYFRCSIHEEELLNEMTNSWFOTTNLYNDAOGESSTALYPIITNISFDSRBMSODVYAIEEDTADENECIACARHSATYHAHIIMEGGHOEOOGWEIHKHBECTETABRFRSSTASNSCANFBGDTNPIRNGRSEMCFYDTLISREOMKLHIELTOTFINIMIROEDNSESBULLNPDIIBEEDRAOTHHNEGEOBESIETTRGAHLUFEYGTAISDHOCUYTVEUEHAEAEEOUCSIEMOHEWTTSTOEAEDGIMHRDOEEAABBDLTAOOKNOFWYFENNSDRNMSTROONFTFWOEORKOUREDSTCHTCRUA";
    char *crib = "FINDSHIMSELFMASTEROFHISOWNESTABLISHMENTWERESUFFICIENTTOABSORBALLMYATTENTIONWHILEHOLMESWHOLOATHEDEVERYFORMOFSOCIETYWITHHISWHOLEBOHEMIANSOULREMAINEDINOURLODGINGSINBAKERSTREETBURIEDAMONGHISOLDBOOKS";

    // Test get_column_frequency
    int * column_frequencies = get_column_frequency(crib, crib_size, key1_size);

    for(int i = 0; i < key1_size; ++i) {
        printf("%d: ", i);
        for(int j = 0; j < 26; ++j) {
            if(column_frequencies[i*26+j] > 0) {
                printf("%d%c ", column_frequencies[i*26+j], ('A'+j));
            }
        }
        printf("\n");
    }

    // Test key2_col_match
    int key2_col_match = get_first_col(ciphertext, ciphertext_size, column_frequencies, key2_size, key1_size);

    printf("Matching Column: %c\n", ('a' + key2_col_match));

    char * key2 = get_key2_putative(ciphertext, ciphertext_size, crib, crib_size, key2_col_match, column_frequencies, key1_size, key2_size);
    key2 = key2_find_permutations(ciphertext, ciphertext_size, crib, crib_size, key2, key1_size, key2_size, column_frequencies, key2_col_match);

    return 0;
}
