#include <stdio.h>
#include "enhanced_spn.h"

int main()
{
    //k:0011 1010 1001 0100 1101 0110 0011 1111 0011 1010 1001 0100 1101 0110 0011 1111 0011 1010 1001 0100 1101 0110 0011 1111 0011 1010 1001 0100 1101 0110 0011 1111
    //x:0010 0110 1011 0111
    int i;
    char ch[129];
    num128 k = {0x3A, 0x94, 0xD6, 0x3F, 0x3A, 0x94, 0xD6, 0x3F, 0x3A, 0x94, 0xD6, 0x3F, 0x3A, 0x94, 0xD6, 0x3F},
           x = {0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7},
           t = {34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34},
           res = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           k_group[NR + 2],
           *ptk = k_group,
           y;
    keygen_enhanced(ptk, k);
    res = spn_enhanced(x, ptk);
    for (i = 0; i < NUM128_SIZE; i++)
        printf("%02x", res.part[i]);
    printf("\n");
    y = spn_enhanced_inverse(res, ptk);
    for (i = 0; i < NUM128_SIZE; i++)
        printf("%02x", y.part[i]);
    num2char(ch,k);
    printf("\n%s",ch);
}