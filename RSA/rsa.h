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

    mpz_init_set_ui(b, 65537); //公共指数b按标准为65537

    multi_inverse(a, b, phi_n); //求e的数论倒数d


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
    mpz_powm(x, a, m, n);
    //square_mul(x,a,m,n);
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
    //while (mpz_probab_prime_p(res, 25)!=0)
    while (miller_rabin_test(res, 5) == 0)
        mpz_add_ui(res, res, 2);
    mpz_clear(tmp);
}

void multi_inverse(mpz_t res, mpz_t m, mpz_t n)
{
    mpz_t m0, n0, t0, q, tmp, r;
    mpz_init(m0);
    mpz_init(n0);
    mpz_init(t0);
    mpz_init(q);
    mpz_init(tmp);
    mpz_init(r);

    mpz_set(m0, m);
    mpz_set(n0, n);
    mpz_set_ui(t0, 0);
    mpz_set_ui(res, 1);
    mpz_div(q, m0, n0);
    mpz_mul(tmp, q, n0);
    mpz_sub(r, m0, tmp);
    while (mpz_cmp_ui(r, 0) > 0)
    {
        mpz_mul(tmp, res, q);
        mpz_sub(tmp, t0, tmp);
        mpz_mod(tmp, tmp, m);

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

    mpz_clear(m0);
    mpz_clear(n0);
    mpz_clear(t0);
    mpz_clear(q);
    mpz_clear(tmp);
    mpz_clear(r);
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
