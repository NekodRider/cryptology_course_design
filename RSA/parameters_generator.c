#include <stdio.h>
#include <time.h>
#include "rsa.h"

int main()
{
    int i;
    mpz_t p, q, n, a, b;
    clock_t t1, t2;
    double duration;
    mpz_inits(p, q, n, a, b, NULL);
    t1 = clock();
    generate_rsa_parameters(p, q, n, a, b);
    t2 = clock();
    duration = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Use Time:%fs\n", duration);
    gmp_printf("%s (\n  %ZX,\n  %ZX\n)\n", "public key:\n", n, b);             //输出公钥(n, b)
    gmp_printf("%s (\n  %ZX,\n  %ZX,\n  %ZX\n)\n", "private key:\n", p, q, a); //输出私钥(p,q,a)
    return 0;
}