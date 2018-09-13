#include<stdio.h>
#include <time.h>
#include<gmp.h>
#include"rsa.h"
int main(){
    int i;
    clock_t t1,t2;
    double duration;
    mpz_t x,y,p, q, n, a, b;
    char bit[501];
    mpz_inits(p,q,n,a,b,x,y,NULL);
    generate_rsa_parameters(p,q,n,a,b);
    for(i=0;i<500;i++){
        bit[i]='1';
    }
    bit[i]='\0';
    mpz_set_str(x,bit,2);
    t1 = clock();
    rsa_encrypt_square_mul(y,n,b,x);
    rsa_decrypt_square_mul(x,n,a,y);
    gmp_printf("%Zd\n",x);
    t2 = clock();
    duration = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Square mul Use Time:%fs\n",duration);
    
    t1 = clock();
    rsa_decrypt_crt(x,y,a,p,q);
    rsa_decrypt_crt(x,y,a,p,q);
    gmp_printf("%Zd\n",x);
    t2 = clock();
    duration = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("CRT Use Time:%fs\n",duration);

    t1 = clock();
    mpz_set_ui(y,0);
    montgomery_mod_exp(y,x,b,n);
    t2 = clock();
    duration = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Montgomery Use Time:%fs\n",duration);
    t1=clock();
    mpz_set_ui(x,0);
    montgomery_mod_exp(x,y,a,n);
    t2=clock();
    gmp_printf("%Zd\n",x);
    
    duration = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Montgomery Use Time:%fs\n",duration);
    return 0;
}