#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <windows.h>

int main()
{
    int i;
    mpz_t p, q, n, a, b, phi_n;
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));

    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(a);
    mpz_init(b);
    mpz_init(phi_n);

    mpz_urandomb(p, state, 1024);
    if (mpz_even_p(p))
        mpz_add_ui(p, p, 1);
    while (!mpz_probab_prime_p(p, 35) > 0)
        mpz_add_ui(p, p, 2);

    mpz_urandomb(q, state, 1024);
    if (mpz_even_p(q))
        mpz_add_ui(q, q, 1);
    while (!mpz_probab_prime_p(q, 35) > 0)
        mpz_add_ui(q, q, 2);

    mpz_mul(n, p, q);

    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi_n, p, q);

    mpz_init_set_ui(b, 65537); //公共指数b按标准为65537

    mpz_invert(a, b, phi_n); //求e的数论倒数d

    gmp_printf("%s (\n  %ZX,\n  %ZX\n)\n", "public key:\n", n, b);             //输出公钥(n, b)
    gmp_printf("%s (\n  %ZX,\n  %ZX,\n  %ZX\n)\n", "private key:\n", p, q, a); //输出私钥(p,q,a)

    return 0;
}