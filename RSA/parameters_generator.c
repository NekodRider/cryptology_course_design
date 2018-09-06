#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <windows.h>

int miller_rabin(mpz_t n,int rep){
    int i,j,k,flag=0;
    mpz_t a,b,m,temp_1,temp_2;
    mpz_init(a);
    mpz_init(b);
    mpz_init(m);
    mpz_init(temp_1);
    mpz_init(temp_2);
    mpz_sub(temp_1,n,1);
    while(1){
        mpz_cdiv_r(temp_2,temp_1,2);
        if(mpq_equal(temp_2,0)){
            k++;
            mpz_cdiv_q(temp_1,temp_1,2);
        }
        else
            break;
    }
    mpz_set(m,temp_1);
    mpz_sub(temp_1,n,2);
    for(j=0;j<rep;j++){
        int flag=0;
        mpz_urandomm(a, gmp_randinit_mt, temp_1);
        if(mpq_cmp(a,2)<=0)
            mpz_set(a,2);
        mpz_pow_ui(temp_1,a,m);
        mpz_mod(b,temp_1,n);
        if(mpq_equal(b,1)||mpq_equal(b,temp_1)){
            continue;
        }
        for(i=1;i<k;i++){
            mpz_mul(temp_2,b,b);
            mpz_mod(b,temp_2,n);
            if(mpq_equal(b,temp_1)){
                flag=1;
                break;
            }
        }
        if(flag==1)
            continue;
        flag=2;
        break;
    }
    if(flag==2)
        flag=0;
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(m);
    mpz_clear(temp_1);
    mpz_clear(temp_2);
    return flag;
}

void get_random_prime(mpz_t res,gmp_randstate_t s){
    mpz_t tmp;
    mpz_init(tmp);
    mpz_urandomb(res, s, 1024);
    mpz_mod(tmp,res,2);
    if (mpq_equal(tmp,1))
        mpz_add_ui(res, res, 1);
    while (!miller_rabin(res))
        mpz_add_ui(res, res, 2);
    mpz_clear(tmp);
}

void multi_inverse(mpz_t res,mpz_t m,mpz_t n){
    mpz_t m0,n0,t0,q,tmp,r;
    mpz_init(m0);
    mpz_init(n0);
    mpz_init(t0);
    mpz_init(q);
    mpz_init(tmp);
    mpz_init(r);

    mpz_set(res,1);
    mpz_div(q,m0,n0);
    mpz_mul(tmp,q,n0);
    mpz_sub(r,m0,tmp);
    while(mpq_cmp(r,0)>0){
        mpz_mul(tmp,res,q);
        mpz_sub(tmp,t0,tmp);
        mpz_mod(tmp,tmp,m);

        mpz_set(t0,res);
        mpz_set(res,tmp);
        mpz_set(m0,n0);
        mpz_set(n0,r);
        mpz_div(q,m0,n0);
        mpz_mul(tmp,q,n0);
        mpz_sub(r,m0,tmp);
    }
    if(mpq_equal(n0,1)!=0)
        mpz_set(res,-1);
    
    mpz_clear(m0);
    mpz_clear(n0);
    mpz_clear(t0);
    mpz_clear(q);
    mpz_clear(tmp);
    mpz_clear(r);

}

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

    get_random_prime(p,state);
    get_random_prime(q,state);
    mpz_mul(n, p, q);
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi_n, p, q);

    mpz_init_set_ui(b, 65537); //公共指数b按标准为65537

    multi_inverse(a, b, phi_n); //求e的数论倒数d


    gmp_printf("%s (\n  %ZX,\n  %ZX\n)\n", "public key:\n", n, b);             //输出公钥(n, b)
    gmp_printf("%s (\n  %ZX,\n  %ZX,\n  %ZX\n)\n", "private key:\n", p, q, a); //输出私钥(p,q,a)

    return 0;
}