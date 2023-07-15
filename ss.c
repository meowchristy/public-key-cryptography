#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <gmp.h>
#include <stdlib.h>

#include "randstate.h"
#include "numtheory.h"
#include "ss.h"

void lcm(mpz_t o, mpz_t a, mpz_t b) {
    mpz_t m;
    mpz_init(m);

    mpz_t n;
    mpz_init(n);

    mpz_mul(m, a, b);
    gcd(n, a, b);

    mpz_div(o, m, n);

    mpz_clear(m);
    mpz_clear(n);
}

void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    mpz_t n1, n2, i, t, a;
    mpz_inits(n1, n2, i, t, a, NULL);

    uint64_t block = (random() % (3 * nbits / 4 - nbits / 4 + 1)) + nbits / 4;
    uint64_t remain = nbits - block;

    make_prime(p, block, iters);
    make_prime(q, remain, iters);
    mpz_mul(n, p, q);
    mpz_mul(n, n, p);

    mpz_sub_ui(n2, q, 1);
    mpz_sub_ui(n1, p, 1);

    lcm(i, n1, n2);

    while (mpz_cmp_ui(t, 1) != 0) {
        mpz_random(a, block);
        gcd(t, a, i);
    }

    mpz_clears(n1, n2, i, t, a, NULL);
}

void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    mpz_t lam;
    mpz_init(lam);

    mpz_t n1;
    mpz_init(n1);

    mpz_t n2;
    mpz_init(n2);

    mpz_t n;
    mpz_init(n);
    mpz_mul(n, p, p);
    mpz_mul(n, n, q);

    mpz_mul(pq, p, q);

    mpz_sub_ui(n1, p, 1);
    mpz_sub_ui(n2, q, 1);
    lcm(lam, n1, n2);
    mod_inverse(d, n, lam);

    mpz_clear(lam);
    mpz_clear(n1);
    mpz_clear(n2);
    mpz_clear(n);
}

void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%s\n", username);
}

void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", pq);
    gmp_fprintf(pvfile, "%Zx\n", d);
}

void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%s\n", username);
}

void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", pq);
    gmp_fscanf(pvfile, "%Zx\n", d);
}

void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    pow_mod(c, m, n, n);
}

void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    uint64_t k;
    size_t j;

    mpz_t m, c;
    mpz_inits(m, c, NULL);

    k = ((mpz_sizeinbase(n, 2) / 2) - 1) / 8;
    uint8_t *kbytes = (uint8_t *) calloc(k, sizeof(uint8_t));
    kbytes[0] = 0xff;

    while ((j = fread(&kbytes[1], sizeof(uint8_t), k - 1, infile)) > 0) {
        mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, kbytes);
        ss_encrypt(c, m, n);
        gmp_fprintf(outfile, "%Zx\n", c);
    }
    mpz_clear(m);
    mpz_clear(c);
    free(kbytes);
}

void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    pow_mod(m, c, d, pq);
}

void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    uint64_t k;
    size_t j;

    mpz_t m;
    mpz_init(m);

    mpz_t c;
    mpz_init(c);

    k = (mpz_sizeinbase(pq, 2) - 1) / 8;
    uint8_t *kbytes = (uint8_t *) calloc(k, sizeof(uint8_t));

    while (gmp_fscanf(infile, "%Zx\n", c) != EOF) {
        ss_decrypt(m, c, d, pq);
        mpz_export(kbytes, &j, 1, sizeof(uint8_t), 1, 0, m);
        fwrite(&kbytes[1], sizeof(uint8_t), j - 1, outfile);
    }
    mpz_clear(m);
    mpz_clear(c);
    free(kbytes);
}
