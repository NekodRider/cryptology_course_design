#include <stdio.h>
#include <gmp.h>
int main()
{
    int i, reps = 40;
    char num1024b[1025], num2048b[2049];
    gmp_randstate_t state;
    gmp_randinit_default(state);
    mpz_t p, q, n, a, b, P_MAX, N_MAX;
    num1024b[0] = "1";
    for (i = 1; i < 1025; i++)
    {
        num1024b[i] = "0";
    }
    num2048b[0] = "1";
    for (i = 1; i < 2049; i++)
    {
        num2048b[i] = "0";
    }

    mpz_init_set_str(P_MAX, num1024b, 10);
    mpz_init_set_str(N_MAX, num2048b, 10);

    mpz_init(p);
    while (1)
    {
        mpz_urandomm(p, state, P_MAX);
        if (mpz_probab_prime_p(p, reps) == 2)
            break;
        //2 is prime 1 prob. Reasonable values of reps are between 15 and 50.
    }

    mpz_init(q);
    while (1)
    {
        mpz_urandomm(q, state, P_MAX);
        if (mpz_probab_prime_p(q, reps) == 2)
            break;
    }

    return 0;
}