#include <stdio.h>

unsigned int linear_attack(unsigned int *T, int t)
{
    unsigned int k2, maxkey = 0;
    int i, j, l1, l2 = 0,
                  count[16][16],
                  max = -1;
    for (l1 = 0; l1 < 16; l1++)
        for (l2 = 0; l2 < 16; l2++)
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
        for (l1 = 0; l1 < 16; l1++)
            for (l2 = 0; l2 < 16; l2++)
            {
                unsigned int z, v4_2, v4_4, u4_2, u4_4, u4__6, u4__8, u4__14, u4__16;
                v4_2 = l1 ^ y2;
                v4_4 = l2 ^ y4;
                u4_2 = sub_inverse(v4_2);
                u4_4 = sub_inverse(v4_4);
                u4__6 = (u4_2 >> 2) & 0x1;
                u4__8 = u4_2 & 0x1;
                u4__14 = (u4_4 >> 2) & 0x1;
                u4__16 = u4_4 & 0x1;
                z = x5 ^ x7 ^ x8 ^ u4__6 ^ u4__8 ^ u4__14 ^ u4__16;
                if (z == 0)
                {
                    count[l1][l2]++;
                }
            }
    }

    for (l1 = 0; l1 < 16; l1++)
        for (l2 = 0; l2 < 16; l2++)
        {
            count[l1][l2] = count[l1][l2] - t / 2;
            if (count[l1][l2] < 0)
                count[l1][l2] = -count[l1][l2];
            if (count[l1][l2] > max)
            {
                max = count[l1][l2];
                maxkey = (l1 << 8) | l2;
                k2 = l1;
            }
        }

    max = -1;

    for (l1 = 0; l1 < 16; l1++)
        for (l2 = 0; l2 < 16; l2++)
            count[l1][l2] = 0;

    for (i = 0; i < t; i++)
    {
        unsigned int x = T[2 * i],
                     y = T[2 * i + 1];
        int x9 = (x >> 7) & 0x1,
            x10 = (x >> 6) & 0x1,
            y1 = (y >> 12) & 0xf,
            y2 = (y >> 8) & 0xf,
            y3 = (y >> 4) & 0xf;
        for (l1 = 0; l1 < 16; l1++)
            for (l2 = 0; l2 < 16; l2++)
            {
                unsigned int z, v4_1, v4_2, v4_3, u4_1, u4_2, u4_3, u4__1, u4__2, u4__5, u4__6, u4__9, u4__10, u4__11;
                v4_1 = l1 ^ y1;
                v4_2 = k2 ^ y2;
                v4_3 = l2 ^ y3;
                u4_1 = sub_inverse(v4_1);
                u4_2 = sub_inverse(v4_2);
                u4_3 = sub_inverse(v4_3);

                u4__1 = (u4_1 >> 3) & 0x1;
                u4__2 = (u4_1 >> 2) & 0x1;
                u4__5 = (u4_2 >> 3) & 0x1;
                u4__6 = (u4_2 >> 2) & 0x1;
                u4__9 = (u4_3 >> 3) & 0x1;
                u4__10 = (u4_3 >> 2) & 0x1;
                u4__11 = (u4_3 >> 1) & 0x1;
                z = x9 ^ x10 ^ u4__1 ^ u4__2 ^ u4__5 ^ u4__6 ^ u4__9 ^ u4__10 ^ u4__11;
                if (z == 0)
                    count[l1][l2]++;
            }
    }

    for (l1 = 0; l1 < 16; l1++)
        for (l2 = 0; l2 < 16; l2++)
        {
            count[l1][l2] = count[l1][l2] - t / 2;
            if (count[l1][l2] < 0)
                count[l1][l2] = -count[l1][l2];
            if (count[l1][l2] > max)
            {
                max = count[l1][l2];
                maxkey &= 0x0f0f;
                maxkey |= (l1 << 12) | (l2 << 4);
            }
        }
    return maxkey;
}

//0000 1011 0000 0000
//0000 0000 0010 0000
unsigned int diff_attack(unsigned int *T1, unsigned int *T2, int t1, int t2)
{
    unsigned int maxkey = 0;
    int i, j, l1, l2 = 0,
                  count[16][16],
                  max = -1;
    for (l1 = 0; l1 < 16; l1++)
        for (l2 = 0; l2 < 16; l2++)
            count[l1][l2] = 0;

    for (i = 0; i < t1; i++)
    {
        unsigned int x = T1[4 * i],
                     x_ = T1[4 * i + 1],
                     y = T1[4 * i + 2],
                     y_ = T1[4 * i + 3];
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
            for (l1 = 0; l1 < 16; l1++)
                for (l2 = 0; l2 < 16; l2++)
                {
                    unsigned int v4_2, v4_4, u4_2, u4_4, v4_2_, v4_4_, u4_2_, u4_4_, u4_2__, u4_4__;
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

                    if (u4_2__ == 6 && u4_4__ == 6)
                    {
                        count[l1][l2]++;
                    }
                }
        }
    }

    for (l1 = 0; l1 < 16; l1++)
        for (l2 = 0; l2 < 16; l2++)
        {
            if (count[l1][l2] > max)
            {
                max = count[l1][l2];
                maxkey = (l1 << 8) | l2;
            }
        }

    max = -1;

    for (l1 = 0; l1 < 16; l1++)
        for (l2 = 0; l2 < 16; l2++)
            count[l1][l2] = 0;

    for (i = 0; i < t2; i++)
    {
        unsigned int x = T2[4 * i],
                     x_ = T2[4 * i + 1],
                     y = T2[4 * i + 2],
                     y_ = T2[4 * i + 3];
        int y1 = (y >> 12) & 0xf,
            y2 = (y >> 8) & 0xf,
            y3 = (y >> 4) & 0xf,
            y4 = y & 0xf,
            y1_ = (y_ >> 12) & 0xf,
            y2_ = (y_ >> 8) & 0xf,
            y3_ = (y_ >> 4) & 0xf,
            y4_ = y_ & 0xf;
        if (y2 == y2_ && y4 == y4_)
        {
            for (l1 = 0; l1 < 16; l1++)
                for (l2 = 0; l2 < 16; l2++)
                {
                    unsigned int v4_1, v4_2, v4_3, u4_1, u4_2, u4_3, v4_1_, v4_2_, v4_3_, u4_1_, u4_2_, u4_3_, u4_1__, u4_2__, u4_3__;
                    v4_1 = l1 ^ y1;
                    v4_3 = l2 ^ y3;
                    u4_1 = sub_inverse(v4_1);
                    u4_3 = sub_inverse(v4_3);

                    v4_1_ = l1 ^ y1_;
                    v4_3_ = l2 ^ y3_;
                    u4_1_ = sub_inverse(v4_1_);
                    u4_3_ = sub_inverse(v4_3_);

                    u4_1__ = u4_1 ^ u4_1_;
                    u4_3__ = u4_3 ^ u4_3_;

                    if (u4_1__ == 0x5 && u4_3__ == 0x5)
                        count[l1][l2]++;
                }
        }
    }

    for (l1 = 0; l1 < 16; l1++)
        for (l2 = 0; l2 < 16; l2++)
        {
            if (count[l1][l2] > max)
            {
                max = count[l1][l2];
                maxkey &= 0x0f0f;
                maxkey |= (l1 << 12) | (l2 << 4);
            }
        }

    return maxkey;
}
