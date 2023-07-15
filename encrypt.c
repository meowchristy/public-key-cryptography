#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <gmp.h>

#include "randstate.h"
#include "ss.h"

#define OPTIONS ":i:o:nvh"

int main(int argc, char **argv) {

    bool verbose = false;
    bool file = false;
    bool in = false;
    bool out = false;

    int i = 0;
    int help = 0;
    for (i = 1; i < argc; i++) {
        if (argv[i][1] == 'h') {
            help = 1;
        }
    }

    mpz_t n;
    mpz_init(n);

    char *username = (char *) malloc(255 * sizeof(char));

    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pbfile;

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            if (help == 0) {
                infile = fopen(optarg, "r");
                in = true;
            }
            break;

        case 'o':
            if (help == 0) {
                outfile = fopen(optarg, "w");
                out = true;
            }
            break;

        case 'n':
            if (help == 0) {
                pbfile = fopen(optarg, "r");
                file = true;
            }
            break;

        case 'v':
            if (help == 0) {
                verbose = true;
            }
            break;

        case 'h':
            if (help == 1) {
                printf("SYNOPSIS\n");
                printf("    Encrypts data using SS encryption.\n");
                printf("    Encrypted data is decrypted by the decrypt program.\n");
                printf("\nUSAGE\n");
                printf("    ./encrypt [OPTIONS]\n");
                printf("\nOPTIONS\n");
                printf("    -h              Display program help and usage.\n");
                printf("    -v              Display verbose program output.\n");
                printf("    -i infile       Input file of data to encrypt (default: stdin).\n");
                printf("    -o outfile      Output file for encrypted data (default: stdout).\n");
                printf("    -n pbfile       Public key file (default: ss.pub).\n");
            }
            break;
        }
    }

    if (file == false) {
        pbfile = fopen("ss.pub", "r");
    }

    ss_read_pub(n, username, pbfile);

    if (verbose == true) {
        printf("user = %s \n", username);
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
    }

    ss_encrypt_file(infile, outfile, n);

    if (in == true) {
        fclose(infile);
    }
    if (out == true) {
        fclose(outfile);
    }

    free(username);
    fclose(pbfile);
    mpz_clear(n);

    return 0;
}
