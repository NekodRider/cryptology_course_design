#include <stdio.h>
#include "basic_spn.h"
#include <time.h>
//l = m = n = 4

int main()
{
    clock_t t1, t2;
    double duration;
    //k:0011 1010 1001 0100 1101 0110 0011 1111
    //x:0010 0110 1011 0111
    char ch[17];
    unsigned int k = 0x3A94D63F,
                 x = 0x26B7,
                 res = 0,
                 k_group[NR + 2],
                 *ptk = k_group;

    t1 = clock();
    keygen(ptk, k);
    res = spn(x, ptk);
    t2 = clock();
    duration = (double)(t2 - t1) / CLOCKS_PER_SEC;
    // printf("spn Use Time:%fs\n",duration);
    printf("encrypted:%x decrypted:%x\n", res, spn_inverse(res, ptk));
    getchar();
    getchar();
    // uint2char(ch,res);
    // printf("%s",ch);
}