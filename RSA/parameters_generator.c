#include <stdio.h>
#include "rsa.h"

int main()
{
    int i;
    mpz_t p, q, n, a, b;
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(a);
    mpz_init(b);
    generate_rsa_parameters(p,q,n,a,b);
    gmp_printf("%s (\n  %ZX,\n  %ZX\n)\n", "public key:\n", n, b);             //输出公钥(n, b)
    gmp_printf("%s (\n  %ZX,\n  %ZX,\n  %ZX\n)\n", "private key:\n", p, q, a); //输出私钥(p,q,a)
    return 0;
}