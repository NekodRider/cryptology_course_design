#include <stdio.h>
#include<time.h>
#include "enhanced_spn.h"

int main()
{
    clock_t t1,t2;
    double duration;
    //k:0011 1010 1001 0100 1101 0110 0011 1111 0011 1010 1001 0100 1101 0110 0011 1111 0011 1010 1001 0100 1101 0110 0011 1111 0011 1010 1001 0100 1101 0110 0011 1111
    //x:0010 0110 1011 0111
    int i;
    char ch[129];
    num128 k = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           x = {0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7},
           t = {34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34},
           res = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           k_group[NR + 2],
           *ptk = k_group,
           y;
    keygen_enhanced(ptk, k);
    t1 = clock();
    res = spn_enhanced(x, ptk);
    t2 = clock();
    duration = (double)(t2 - t1) / CLOCKS_PER_SEC;
    // printf("enhanced spn Use Time:%fs\n",duration);
    for (i = 0; i < NUM128_SIZE; i++)
        printf("%02x", res.part[i]);
    printf("\n");
    y = spn_enhanced_inverse(res, ptk);
    for (i = 0; i < NUM128_SIZE; i++)
        printf("%02x", y.part[i]);
    getchar();getchar();
    // num2char(ch,k);
    // printf("\n%s",ch);
}