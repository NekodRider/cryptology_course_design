#include <stdio.h>

unsigned int sub_inverse(unsigned int a)
{
    int i = 0;
    unsigned int tmp, res = 0,
                      mask = 0xf,
                      s_inverse_index[16] = {14, 3, 4, 8, 1, 12, 10, 15, 7, 13, 9, 6, 11, 2, 0, 5};
    for (i = 0; i < 4; i++)
    {
        tmp = a & mask;
        res = res | (s_inverse_index[tmp] << (i * 4));
        a = a >> 4;
    }
    return res;
}

unsigned int linear_attack(unsigned int *T, int t)
{
    unsigned maxkey;
    int i, j, l1, l2 = 0,
                  count[17][17],
                  max = -1;
    for (l1 = 0; l1 < 17; l1++)
        for (l2 = 0; l2 < 17; l2++)
            count[l1][l2] = 0;

    for (i = 0; i < t; i++)
    {
        unsigned int x = T[2 * i],
                     y = T[2 * i + 1];
        int x5 = (x >> 11) & 0x1,
            x7 = (x >> 9) & 0x1,
            x8 = (x >> 8) & 0x1,
            y2 = (y >> 8) & 0xf,
            y4 = y & 0xf;
        for (l1 = 0; l1 < 17; l1++)
            for (l2 = 0; l2 < 17; l2++)
            {
                unsigned int z, v4_2, v4_4, u4_2, u4_4, u4_6, u4_8, u4_14, u4_16;
                v4_2 = l1 ^ y2;
                v4_4 = l2 ^ y4;
                u4_2 = sub_inverse(v4_2);
                u4_4 = sub_inverse(v4_4);
                u4_6 = (u4_2 >> 2) & 0x1;
                u4_8 = u4_2 & 0x1;
                u4_14 = (u4_4 >> 2) & 0x1;
                u4_16 = u4_4 & 0x1;
                z = x5 ^ x7 ^ x8 ^ u4_6 ^ u4_8 ^ u4_14 ^ u4_16;
                if (z = 0)
                    count[l1][l2]++;
            }
    }

    for (l1 = 0; l1 < 17; l1++)
        for (l2 = 0; l2 < 17; l2++)
        {
            count[l1][l2] = count[l1][l2] - t / 2;
            if (count[l1][l2] < 0)
                count[l1][l2] = -count[l1][l2];
            if (count[l1][l2] > max)
            {
                max = count[l1][l2];
                maxkey = (l1 << 1) | l2;
            }
        }
    return maxkey;
}

unsigned int diff_attack(unsigned int *T, int t)
{
    unsigned maxkey;
    int i, j, l1, l2 = 0,
                  count[17][17],
                  max = -1;
    for (l1 = 0; l1 < 17; l1++)
        for (l2 = 0; l2 < 17; l2++)
            count[l1][l2] = 0;

    for (i = 0; i < t; i++)
    {
        unsigned int x = T[4 * i],
                     x_ = T[4 * i + 1],
                     y = T[4 * i + 2],
                     y_ = T[4 * i + 3];
        int y1 = (y >> 12) & 0xf,
            y2 = (y >> 8) & 0xf,
            y3 = (y >> 4) & 0xf,
            y4 = y & 0xf,
            y1_ = (y_ >> 12) & 0xf,
            y2_ = (y_ >> 8) & 0xf,
            y3_ = (y_ >> 4) & 0xf,
            y4_ = y_ & 0xf;
        if (y1 == y1_ && y3 == y3_)
        {
            for (l1 = 0; l1 < 17; l1++)
                for (l2 = 0; l2 < 17; l2++)
                {
                    unsigned int v4_2, v4_4, u4_2, u4_4, v4_2_,v4_4_,u4_2_,u4_4_,u4_2__,u4_4__;
                    v4_2 = l1 ^ y2;
                    v4_4 = l2 ^ y4;
                    u4_2 = sub_inverse(v4_2);
                    u4_4 = sub_inverse(v4_4);

                    v4_2_ = l1 ^ y2_;
                    v4_4_ = l2 ^ y4_;
                    u4_2_ = sub_inverse(v4_2_);
                    u4_4_ = sub_inverse(v4_4_);

                    u4_2__ = u4_2 ^ u4_2_;
                    u4_4__ = u4_4 ^ u4_4_;

                    if  (u4_2__==6&&u4_4__ ==6) 
                        count[l1][l2]++;
                }
        }
    }

    for (l1 = 0; l1 < 17; l1++)
        for (l2 = 0; l2 < 17; l2++)
        {
            if (count[l1][l2] > max)
            {
                max = count[l1][l2];
                maxkey = (l1 << 1) | l2;
            }
        }
    return maxkey;
}