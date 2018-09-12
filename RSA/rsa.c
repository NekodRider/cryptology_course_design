#include<stdio.h>
#include <time.h>
#include<gmp.h>
#include"rsa.h"
int main(){
    clock_t t1,t2;
    double duration;
    mpz_t x,y,p, q, n, a, b;
    mpz_inits(p,q,n,a,b,x,y,NULL);
    generate_rsa_parameters(p,q,n,a,b);
    mpz_set_ui(x,7);
    gmp_printf("%Zd\n",a);
    gmp_printf("%Zd\n",b);
    gmp_printf("%Zd\n",n);
    t1 = clock();
    rsa_encrypt_square_mul(y,n,b,x);
    gmp_printf("%Zd\n",y);
    rsa_decrypt_square_mul(x,n,a,y);
    gmp_printf("%Zd\n",x);
    t2 = clock();
    duration = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Square mul Use Time:%fms\n",duration);
    
    t1 = clock();
    rsa_decrypt_crt(x,y,a,p,q);
    t2 = clock();
    duration = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("CRT Use Time:%fms\n",duration);

    t1 = clock();
    // rsa_encrypt_montgomery(y,n,b,x);
    // rsa_decrypt_montgomery(x,n,a,y);
    montgomery_mod_exp(y,x,b,n);
    gmp_printf("%Zd\n",y);
    montgomery_mod_exp(x,y,a,n);
    gmp_printf("%Zd\n",x);
    t2 = clock();
    duration = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Montgomery Use Time:%fms\n",duration);
    // mpz_set_ui(a,311);
    // mpz_set_ui(b,71);
    // mpz_set_ui(n,11111);
    // montgomery_mod_exp(x,a,b,n);
    // gmp_printf("%Zd",x);

    return 0;
}