#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <gmp.h>
#include <time.h>
#include <sys/stat.h>

#include "randstate.h"
#include "ss.h"

#define OPTIONS ":i:o:nvh"

int main(int argc, char **argv) {

    bool verbose = false;
    bool nfile = false;
    bool dfile = false;
    bool sseed = false;

    uint64_t nbits = 256;
    uint64_t iters = 50;
    int seed = 0;

    int i = 0;
    int help = 0;
    for (i = 1; i < argc; i++) {
        if (argv[i][1] == 'h') {
            help = 1;
        }
    }

    mpz_t p, q, n, d, pq;
    mpz_inits(p, q, n, d, pq, NULL);

    FILE *pbfile;
    FILE *pvfile;

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b':
            if (help == 0) {
                nbits = (uint64_t) strtoul(optarg, NULL, 10);
            }
            break;

        case 'i':
            if (help == 0) {
                iters = (uint64_t) strtoul(optarg, NULL, 10);
            }
            break;

        case 'n':
            if (help == 0) {
                pbfile = fopen(optarg, "r");
                nfile = true;
            }
            break;

        case 'd':
            if (help == 0) {
                pvfile = fopen(optarg, "r");
                dfile = true;
            }
            break;

        case 's':
            if (help == 0) {
                seed = (uint64_t) strtoul(optarg, NULL, 10);
                sseed = true;
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
                printf("    Generates an SS public/private key pair.\n");
                printf("\nUSAGE\n");
                printf("    ./keygen [OPTIONS]\n");
                printf("\nOPTIONS\n");
                printf("    -h              Display program help and usage.\n");
                printf("    -v              Display verbose program output.\n");
                printf("    -b bits         Minimum bits needed for public key n (default: 256).");
                printf("    -i iterations   Miller-Rabin iterations for testing primes (default: "
                       "50).\n");
                printf("    -n pbfile       Public key file (default: ss.pub).\n");
                printf("    -d pvfile       Private key file (default: ss.priv).\n");
                printf("    -s seed         Random seed for testing.\n");
            }
            break;
        }
    }
    if (nfile == false) {
        pbfile = fopen("ss.pub", "w");
    }
    if (dfile == false) {
        pvfile = fopen("ss.priv", "w");
    }

    fchmod(fileno(pvfile), S_IRUSR | S_IWUSR);

    if (sseed == false) {
        randstate_init(time(NULL));
    } else {
        randstate_init(seed);
    }

    ss_make_pub(p, q, n, nbits, iters);
    ss_make_priv(d, pq, p, q);

    char *user = getenv("USER");

    ss_write_pub(n, user, pbfile);
    ss_write_priv(pq, d, pvfile);

    if (verbose == true) {
        printf("user = %s \n", user);
        gmp_printf("p (%d bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q (%d bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
        gmp_printf("pq (%d bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
    }

    fclose(pbfile);
    fclose(pvfile);
    randstate_clear();
    mpz_clears(p, q, n, d, pq, NULL);

    return 0;
}
