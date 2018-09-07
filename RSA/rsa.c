#include<stdio.h>
#include<gmp.h>
#include"rsa.h"
int main(){
    mpz_t x,y,p, q, n, a, b;
    mpz_inits(p,q,n,a,b,x,y,NULL);
    generate_rsa_parameters(p,q,n,a,b);
    mpz_set_ui(x,3333);
    rsa_encrypt_square_mul(y,n,b,x);
    rsa_decrypt_crt(x,y,a,p,q);
    gmp_printf("%Zd\n",x);

    rsa_encrypt_montgomery(y,n,b,x);
    rsa_decrypt_montgomery(x,n,a,y);
    gmp_printf("%Zd",x);

    return 0;
}