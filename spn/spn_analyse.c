#include <stdio.h>
#include "basic_spn.h"
#include "spn_analyse.h"

int main()
{
    //k:0011 1010 1001 0100 1101 0110 0011 1111
    //x:0010 0110 1011 0111
    int i;
    unsigned int k = 0x3A94D63F,
                 x = 0x26B7,
                 res = 0,
                 k_group[NR + 2],
                 *ptk = k_group,
                 T1[8000 * 2],
                 T2_1[110 * 4],
                 T2_2[1600 * 4],
                 k_try = 0,
                 k_try_group[NR + 2],
                 *ptk_try = k_try_group;
    keygen(ptk, k);
    for (i = 0; i < 8000; i++)
    {
        T1[2 * i] = i;
        T1[2 * i + 1] = spn(i, ptk);
    }
    //xor requirement:
    //0000 1011 0000 0000
    //0000 0000 0010 0000
    for (i = 0; i < 110; i++)
    {
        T2_1[4 * i] = i;
        T2_1[4 * i + 1] = i ^ 0x0b00;
        T2_1[4 * i + 2] = spn(i, ptk);
        T2_1[4 * i + 3] = spn(T2_1[4 * i + 1], ptk);
    }
    for (i = 0; i < 1600; i++)
    {
        T2_2[4 * i] = i;
        T2_2[4 * i + 1] = i ^ 0x0020;
        T2_2[4 * i + 2] = spn(i, ptk);
        T2_2[4 * i + 3] = spn(T2_2[4 * i + 1], ptk);
    }
    res = linear_attack(T1, 8000);
    printf("linear_attack: %x\n", res);
    res = diff_attack(T2_1, T2_2, 110, 1600);
    printf("diff_attack: %x\n", res);
    do
    {
        keygen(ptk_try, k_try << 16 | res);
        if (spn(T1[0], ptk_try) == T1[1])
            if (spn(T1[2], ptk_try) == T1[3])
                if (spn(T1[4], ptk_try) == T1[5])
                    if (spn(T1[6], ptk_try) == T1[7])
                        if (spn(T1[8], ptk_try) == T1[9])
                        {
                            printf("real K is %x%x", k_try, res);
                            break;
                        }
        k_try++;
    } while (k_try < 0x10000);
}