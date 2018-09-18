#include <stdio.h>
#include <time.h>
#include "basic_spn.h"
#include "spn_analyse.h"

int main()
{
    //k:0011 1010 1001 0100 1101 0110 0011 1111
    //x:0010 0110 1011 0111
    clock_t t1, t2;
    double duration;
    int i;
    unsigned int k = 0x3A94D63F,
                 x = 0x26B7,
                 res = 0,
                 k_group[NR + 2],
                 *ptk = k_group,
                 T1[8000 * 2],
                 T2_1[2000 * 4],
                 T2_2[2000 * 4],
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
    t1 = clock();
    res = linear_attack(T1, 8000);
    t2 = clock();
    printf("linear_attack: %x\n", res);
    duration = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("linear Use Time:%fs\n", duration);

    for (i = 0; i < 2000; i++)
    {
        T2_1[4 * i] = i;
        T2_1[4 * i + 1] = i ^ 0x0b00;
        T2_1[4 * i + 2] = spn(i, ptk);
        T2_1[4 * i + 3] = spn(T2_1[4 * i + 1], ptk);
    }
    for (i = 0; i < 2000; i++)
    {
        T2_2[4 * i] = i;
        T2_2[4 * i + 1] = i ^ 0x0020;
        T2_2[4 * i + 2] = spn(i, ptk);
        T2_2[4 * i + 3] = spn(T2_2[4 * i + 1], ptk);
    }
    t1 = clock();
    res = diff_attack(T2_1, T2_2, 2000, 2000);
    res = diff_attack(T2_1, T2_2, 2000, 2000);
    t2 = clock();
    printf("diff_attack: %x\n", res);
    duration = (double)(t2 - t1) / 2 / CLOCKS_PER_SEC;
    printf("diff Use Time:%fs\n", duration);

    res = 0x3f;
    t1 = clock();
    do
    {
        keygen(ptk_try, k_try << 8 | res);
        if (spn(T1[0], ptk_try) == T1[1])
            if (spn(T1[2], ptk_try) == T1[3])
                if (spn(T1[4], ptk_try) == T1[5])
                    if (spn(T1[6], ptk_try) == T1[7])
                        if (spn(T1[8], ptk_try) == T1[9])
                        {
                            printf("real K is %x%x\n", k_try, res);
                            break;
                        }
        k_try++;
    } while (k_try < 0x1000000);
    t2 = clock();
    duration = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("24 violence Use Time:%fs\n", duration);

    // res=0xd63f;
    // t1=clock();
    // do
    // {
    //     keygen(ptk_try, k_try << 16 | res);
    //     if (spn(T1[0], ptk_try) == T1[1])
    //         if (spn(T1[2], ptk_try) == T1[3])
    //             if (spn(T1[4], ptk_try) == T1[5])
    //                 if (spn(T1[6], ptk_try) == T1[7])
    //                     if (spn(T1[8], ptk_try) == T1[9])
    //                     {
    //                         printf("real K is %x%x\n", k_try, res);
    //                         break;
    //                     }
    //     k_try++;
    // } while (k_try < 0x10000);
    // t2=clock();

    // duration = (double)(t2 - t1) / CLOCKS_PER_SEC;
    // printf("16 violence Use Time:%fs\n",duration);
    // getchar();getchar();
}