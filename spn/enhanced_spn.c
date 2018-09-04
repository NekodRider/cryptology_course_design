#include <stdio.h>
#include "enhanced_spn.h"

int main()
{
    //k:0011 1010 1001 0100 1101 0110 0011 1111 0011 1010 1001 0100 1101 0110 0011 1111 0011 1010 1001 0100 1101 0110 0011 1111 0011 1010 1001 0100 1101 0110 0011 1111
    //x:0010 0110 1011 0111
    int i;
    num128 k = {0x3A, 0x94, 0xD6, 0x3F, 0x3A, 0x94, 0xD6, 0x3F, 0x3A, 0x94, 0xD6, 0x3F, 0x3A, 0x94, 0xD6, 0x3F},
           x = {0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7, 0x26, 0xB7},
           res = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
           k_group[NR + 2],
           *ptk = k_group;
    keygen_enhanced(ptk, k);
    res = spn_enhanced(x, ptk);
    for (i = 0; i < NUM128_SIZE; i++)
        printf("%x", 0xff & res.part[i]);
}