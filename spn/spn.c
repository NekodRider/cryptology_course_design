#include <stdio.h>
#include "basic_spn.h"

//l = m = n = 4

int main()
{
    //k:0011 1010 1001 0100 1101 0110 0011 1111
    //x:0010 0110 1011 0111
    char ch[17];
    unsigned int k = 0x3A94D63F,
                 x = 0x26B7,
                 res = 0,
                 k_group[NR + 2],
                 *ptk = k_group;
    keygen(ptk, k);
    res = spn(x, ptk);
    printf("%x %x\n", res, spn_inverse(res,ptk));
    uint2char(ch,res);
    printf("%s",ch);
}