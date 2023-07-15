#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <gmp.h>
#include <math.h>

#include "randstate.h"
#include "numtheory.h"

void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    mpz_t v;
    mpz_init(v);
    mpz_set_ui(v, 1);

    mpz_t p;
    mpz_init(p);
    mpz_set(p, a);

    mpz_t dd;
    mpz_init(dd);
    mpz_set(dd, d);

    while (mpz_cmp_ui(dd, 0) > 0) {
        if (mpz_odd_p(dd) != 0) {
            mpz_mul(v, v, p);
            mpz_mod(v, v, n);
        }
        mpz_mul(p, p, p);
        mpz_mod(p, p, n);
        mpz_fdiv_q_ui(dd, dd, 2);
    }
    mpz_set(o, v);
    mpz_clear(v);
    mpz_clear(p);
    mpz_clear(dd);
}

bool is_prime(const mpz_t n, uint64_t iters) {
    mpz_t d, s, r, dd, y, a, sm, nm, j, rr, two, nn;
    mpz_inits(d, s, r, dd, y, a, sm, nm, j, rr, two, nn, NULL);

    if (mpz_cmp_ui(n, 1) <= 0 || mpz_cmp_ui(n, 4) == 0) {
        return false;
    }

    if (mpz_cmp_ui(n, 3) <= 0) {
        return true;
    }

    mpz_set(nn, n);

    mpz_sub_ui(d, nn, 1);

    mpz_set_ui(dd, 1);

    mpz_set(rr, nn);

    mpz_sub_ui(rr, rr, 3);

    mpz_sub_ui(nm, nn, 1);

    mpz_set_ui(two, 2);

    while (mpz_odd_p(r) == 0) {
        mpz_add_ui(s, s, 1);
        mpz_div(r, d, dd);
        mpz_mul_ui(dd, dd, 2);
    }

    mpz_sub_ui(s, s, 1);
    mpz_sub_ui(sm, s, 1);

    for (uint64_t i = 1; i < iters; i++) {
        mpz_urandomm(a, state, rr);
        mpz_add_ui(a, a, 2);
        pow_mod(y, a, r, n);

        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, nm) != 0) {
            mpz_set_ui(j, 1);

            while (mpz_cmp(j, sm) <= 0 && mpz_cmp(y, nm) != 0) {
                pow_mod(y, y, two, n);

                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(d, s, r, dd, y, a, sm, nm, j, rr, two, nn, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }

            if (mpz_cmp(y, nm) != 0) {
                mpz_clears(d, s, r, dd, y, a, sm, nm, j, rr, two, nn, NULL);
                return false;
            }
        }
    }
    mpz_clears(d, s, r, dd, y, a, sm, nm, j, rr, two, nn, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    while (is_prime(p, iters) == false) {
        mpz_urandomb(p, state, bits);
        mpz_setbit(p, 0);
    }
}

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    mpz_t t;
    mpz_init(t);

    mpz_t aa;
    mpz_init(aa);
    mpz_set(aa, a);

    mpz_t bb;
    mpz_init(bb);
    mpz_set(bb, b);

    while (mpz_cmp_ui(bb, 0) != 0) {
        mpz_set(t, bb);
        mpz_mod(bb, aa, bb);
        mpz_set(aa, t);
    }
    mpz_set(g, aa);
    mpz_clears(t, bb, aa, NULL);
}

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    mpz_t r, rp, t, tp, q, temp1, temp2;
    mpz_inits(r, rp, t, tp, q, temp1, temp2, NULL);

    mpz_set(r, n);
    mpz_set(rp, a);
    mpz_set_ui(tp, 1);

    while (mpz_cmp_ui(rp, 0) != 0) {
        mpz_fdiv_q(q, r, rp);

        mpz_set(temp1, r);
        mpz_set(r, rp);
        mpz_mul(temp2, q, rp);
        mpz_sub(rp, temp1, temp2);

        mpz_set(temp1, t);
        mpz_set(t, tp);
        mpz_mul(temp2, q, tp);
        mpz_sub(tp, temp1, temp2);
    }

    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(t, 0);
    }

    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
    }

    mpz_set(o, t);
    mpz_clears(r, rp, t, tp, q, temp1, temp2, NULL);
}
