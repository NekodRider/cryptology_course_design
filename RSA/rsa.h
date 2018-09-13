#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <windows.h>

void generate_rsa_parameters(mpz_t p, mpz_t q, mpz_t n, mpz_t a, mpz_t b);
int miller_rabin(mpz_t n,mpz_t m,unsigned int k);
int miller_rabin_test(mpz_t n, int rep);
void get_random_prime(mpz_t res, gmp_randstate_t s);
void multi_inverse(mpz_t res, mpz_t m, mpz_t n);
void square_mul(mpz_t z,mpz_t x,mpz_t c,mpz_t n);
void rsa_encrypt(mpz_t n,mpz_t b,mpz_t x,mpz_t y);
void rsa_decrypt(mpz_t n,mpz_t a,mpz_t y,mpz_t x);

void generate_rsa_parameters(mpz_t p, mpz_t q, mpz_t n, mpz_t a, mpz_t b)
{
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));
    mpz_t phi_n;
    mpz_init(phi_n);

    get_random_prime(p, state);
    get_random_prime(q, state);
    mpz_mul(n, p, q);
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi_n, p, q);
    mpz_add_ui(p, p, 1);
    mpz_add_ui(q, q, 1);
    mpz_init_set_ui(b, 65537);
    multi_inverse(a, b, phi_n);
    mpz_clear(phi_n);
}

int miller_rabin(mpz_t n,mpz_t m,unsigned int k)
{
    unsigned int i;

    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));
    mpz_t temp_1,temp_2,x,a;

    mpz_init(temp_1);
    mpz_init(temp_2);
    mpz_init(x);
    mpz_init(a);

    mpz_set(temp_1,n);
    mpz_sub_ui(temp_1,temp_1,2);
    mpz_urandomm(a, state, temp_1);
    if (mpz_cmp_ui(a, 2) <= 0)
        mpz_set_ui(a, 2);
    square_mul(x,a,m,n);
    mpz_add_ui(temp_1,temp_1,1);
    if (mpz_cmp_ui(x, 1) == 0 || mpz_cmp(x, temp_1) == 0)
        return 1;
    for (i = 1; i < k; i++)
    {
        mpz_mul(temp_2, x, x);
        mpz_mod(x, temp_2, n);
        if (mpz_cmp(x, temp_1) == 0)
            return 1;
        if (mpz_cmp_ui(x, 1) <= 0)
            return 0;
    }
    return 0;
}

int miller_rabin_test(mpz_t n, int rep)
{
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));
    unsigned int flag = 1, i, j, k;
    mpz_t a, b, m, temp_1, temp_2;
    mpz_init(a);
    mpz_init(b);
    mpz_init(m);
    mpz_init(temp_1);
    mpz_init(temp_2);
    mpz_sub_ui(temp_1, n, 1);
    while (1)
    {
        mpz_fdiv_r_ui(temp_2, temp_1, 2);
        if (mpz_cmp_ui(temp_2, 0) == 0)
        {
            k++;
            mpz_fdiv_q_ui(temp_1, temp_1, 2);
        }
        else
            break;
    }
    mpz_set(m, temp_1);
    mpz_sub_ui(temp_1, n, 2);

    for (j = 0; j < rep; j++)
    {
        if(miller_rabin(n,m,k)==0)
            return 0;
    }
    return 1;
}

void get_random_prime(mpz_t res, gmp_randstate_t s)
{
    mpz_t tmp;
    mpz_init(tmp);
    mpz_urandomb(res, s, 1024);
    mpz_mod_ui(tmp, res, 2);
    if (mpz_cmp_ui(tmp, 1) != 0)
        mpz_add_ui(res, res, 1);
    while (miller_rabin_test(res, 5) == 0)
        mpz_add_ui(res, res, 2);
    mpz_clear(tmp);
}

void multi_inverse(mpz_t res, mpz_t m, mpz_t n)
{
    mpz_t m0, n0, t0, q, tmp, r;
    mpz_inits(m0,n0,t0,q,tmp,r,NULL);
    mpz_set(m0, m);
    mpz_set(n0, n);
    mpz_set_ui(t0, 1);
    mpz_set_ui(res, 0);
    mpz_div(q, m0, n0);
    mpz_mul(tmp, q, n0);
    mpz_sub(r, m0, tmp);
    while (mpz_cmp_ui(r, 0) > 0)
    {
        mpz_mul(tmp, res, q);
        mpz_sub(tmp, t0, tmp);
        mpz_set(t0, res);
        mpz_set(res, tmp);
        mpz_set(m0, n0);
        mpz_set(n0, r);
        mpz_div(q, m0, n0);
        mpz_mul(tmp, q, n0);
        mpz_sub(r, m0, tmp);
    }
    if (mpz_cmp_ui(n0, 1) != 0)
        mpz_set_ui(res, 0);
    if(mpz_cmp_ui(res,0)<0)
        mpz_add(res,res,n);
    mpz_clears(m0,n0,t0,q,tmp,r,NULL);
}

void square_mul(mpz_t z,mpz_t x,mpz_t c,mpz_t n){
    unsigned int i,l=1;
    mpz_t tmp,t,tx;
    mpz_inits(tmp,t,tx,NULL);
    mpz_set(tmp,c);
    mpz_set(t,c);
    mpz_set(tx,x);
    mpz_set_ui(z,1);
    while(mpz_cmp_ui(t,0)){
        mpz_fdiv_r_ui(tmp,t,2);
        mpz_fdiv_q_ui(t,t,2);
        if(mpz_cmp_ui(tmp,1)==0){
            mpz_mul(z,z,tx);
            mpz_mod(z,z,n);
        }
        mpz_mul(tx,tx,tx);
        mpz_mod(tx,tx,n);
    }
}

void rsa_encrypt_square_mul(mpz_t y,mpz_t n,mpz_t b,mpz_t x){
    square_mul(y,x,b,n);
}

void rsa_decrypt_square_mul(mpz_t x,mpz_t n,mpz_t a,mpz_t y){
    square_mul(x,y,a,n);
}

void rsa_decrypt_crt(mpz_t x,mpz_t y,mpz_t a,mpz_t p,mpz_t q){
    mpz_t a1,a2,x1,x2,y1,y2,b1,b2,tmp;
    mpz_inits(a1,a2,x1,x2,y1,y2,b1,b2,tmp,NULL);
    mpz_sub_ui(tmp,p,1);
    mpz_mod(a1,a,tmp);
    mpz_sub_ui(tmp,q,1);
    mpz_mod(a2,a,tmp);
    mpz_mod(y1,y,p);
    mpz_mod(y2,y,q);
    square_mul(x1,y1,a1,p);
    square_mul(x2,y2,a2,q);
    multi_inverse(b1,q,p);
    multi_inverse(b2,p,q);
    mpz_mul(x,x1,b1);
    mpz_mul(x,x,q);
    mpz_mul(tmp,x2,b2);
    mpz_mul(tmp,tmp,p);
    mpz_add(x,x,tmp);
    mpz_mul(tmp,p,q);
    mpz_mod(x,x,tmp);
}

void mont_mul(mpz_t n,mpz_t a,mpz_t b,mpz_t N){
    mpz_t N_tmp,a_tmp,tmp,r_tmp,r,t;
    mpz_inits(N_tmp,a_tmp,tmp,r,r_tmp,t,NULL);
    mpz_set(a_tmp,a);
    mpz_set(N_tmp,N);
    //mpz_mul(n,a,b);
    while(1){
        if(mpz_cmp_ui(N_tmp,0)==0)
            break;
        mpz_fdiv_r_2exp(t,n,1);
        if(mpz_cmp_ui(N_tmp,0)!=0){
            mpz_fdiv_r_2exp(r,a_tmp,1);
            mpz_mul(tmp,r,b);
            mpz_add(t,tmp,t);
            mpz_add(n,n,tmp);
            mpz_set_ui(tmp,0);
        }
        mpz_fdiv_r_2exp(t,t,1);
        if(mpz_cmp_ui(t,1)==0){
            mpz_add(n,n,N);
        }
        mpz_fdiv_q_2exp(n,n,1);
        mpz_fdiv_q_2exp(a_tmp,a_tmp,1);
        mpz_fdiv_q_2exp(N_tmp, N_tmp, 1);
    }
    mpz_mod(n,n,N);
}

void montgomery_mod_exp(mpz_t n,mpz_t a,mpz_t b,mpz_t N){
    mpz_t b_tmp,r,s,tmp,tmp_1,k,N_tmp;
    mpz_inits(b_tmp,r,s,tmp,tmp_1,k,N_tmp,NULL);
    mpz_set_ui(tmp_1,1);
    mpz_set_ui(k,1);

    mpz_set(N_tmp,N);
    while(1){
        if(mpz_cmp_ui(N_tmp,0)==0)
            break;
        mpz_mul_2exp(k,k,2);
        mpz_mod(k,k,N);
        mpz_fdiv_q_2exp(N_tmp, N_tmp, 1);
    }

    mont_mul(s,k,a,N);
    mont_mul(n,k,tmp_1,N);
    
    mpz_set(b_tmp,b);
    while(1){
        if(mpz_cmp_ui(b_tmp,0)==0)
            break;
        mpz_fdiv_r_2exp(r, b_tmp, 1);
        if(mpz_cmp_ui(r,1)==0){
            mpz_set_ui(tmp,0);
            mont_mul(tmp,n,s,N);
            mpz_set(n,tmp);
        }
        mpz_set_ui(tmp,0);
        mont_mul(tmp,s,s,N);
        mpz_set(s,tmp);
        mpz_fdiv_q_2exp(b_tmp, b_tmp, 1);
    }
    mpz_set_ui(tmp,0);
    mont_mul(tmp,tmp_1,n,N);
    mpz_mod(tmp,tmp,N);
    mpz_set(n,tmp);
    
}

void rsa_encrypt_montgomery(mpz_t y,mpz_t n,mpz_t b,mpz_t x){
    montgomery_mod_exp(y,x,b,n);
}

void rsa_decrypt_montgomery(mpz_t x,mpz_t n,mpz_t a,mpz_t y){
    montgomery_mod_exp(x,y,a,n);
}
