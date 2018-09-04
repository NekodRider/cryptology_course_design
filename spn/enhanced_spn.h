#include <stdio.h>
#define NR 12
#define S_SIZE 8
#define S_NUM 16
#define NUM128_SIZE 16

typedef struct num128
{
    char part[16];
} num128;

num128 sub_enhanced(num128 a)
{
    int i, j = 0;
    num128 result={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned int tmp, res = 0,
                      mask = 0xff,
                      s_index[S_NUM * S_NUM] = {178, 127, 120, 71, 251, 115, 128, 105, 142, 23, 141, 15, 185, 100, 194, 136, 67, 207, 45, 247, 182, 82, 135, 123, 118, 93, 53, 230, 188, 246, 47, 124, 13, 114, 255, 199, 74, 180, 89, 248, 200, 152, 42, 37, 41, 186, 239, 227, 86, 190, 63, 159, 60, 221, 46, 26, 171, 147, 169, 68, 168, 130, 126, 2, 252, 51, 210, 156, 192, 38, 161, 62, 205, 164, 214, 66, 75, 206, 0, 166, 132, 52, 250, 119, 11, 103, 73, 179, 122, 191, 121, 203, 94, 198, 140, 108, 151, 10, 167, 193, 95, 163, 219, 212, 9, 39, 85, 22, 131, 254, 177, 49, 99, 44, 32, 113, 25, 139, 148, 84, 187, 249, 129, 145, 70, 27, 35, 242, 153, 20, 125, 189, 225, 138, 48, 213, 110, 197, 144, 88, 208, 234, 146, 87, 224, 237, 174, 4, 79, 231, 160, 222, 218, 28, 172, 96, 111, 238, 196, 175, 19, 104, 40, 14, 134, 202, 173, 154, 31, 149, 204, 97, 54, 107, 92, 181, 195, 61, 137, 7, 162, 24, 215, 226, 116, 98, 106, 58, 211, 243, 232, 72, 217, 69, 3, 5, 241, 233, 21, 133, 80, 236, 16, 78, 228, 165, 102, 6, 150, 65, 101, 170, 229, 223, 76, 56, 17, 209, 176, 8, 33, 90, 112, 83, 143, 201, 240, 18, 64, 117, 81, 55, 184, 29, 183, 155, 244, 1, 59, 36, 245, 50, 220, 30, 12, 216, 235, 57, 157, 109, 43, 91, 77, 253, 158, 34};
    for (j = 0; j < NUM128_SIZE; j++)
    {
        result.part[j] = s_index[a.part[j]] << (j * S_SIZE);
    }

    return result;
}

num128 per_enhanced(num128 b)
{
    int i, j = 0;
    num128 result={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned int tmp, res = 0,
                      mask = 0x1,
                      p_index[S_NUM * S_NUM] = {216, 25, 187, 56, 79, 51, 108, 177, 100, 221, 165, 31, 159, 143, 67, 145, 138, 35, 123, 186, 132, 200, 158, 36, 203, 72, 137, 94, 113, 248, 93, 170, 48, 253, 63, 171, 64, 20, 83, 73, 90, 148, 44, 237, 95, 246, 149, 196, 211, 27, 91, 54, 205, 226, 128, 255, 7, 89, 40, 231, 130, 207, 244, 179, 45, 219, 120, 188, 66, 92, 156, 26, 97, 210, 9, 243, 129, 85, 249, 112, 185, 98, 140, 21, 33, 139, 13, 197, 233, 208, 126, 49, 155, 241, 61, 131, 96, 181, 53, 37, 189, 28, 195, 39, 32, 75, 178, 11, 111, 77, 180, 34, 87, 114, 230, 134, 135, 101, 136, 252, 162, 105, 59, 84, 57, 172, 110, 76, 254, 146, 209, 133, 71, 154, 206, 174, 107, 157, 151, 125, 251, 169, 3, 228, 118, 238, 141, 6, 17, 41, 24, 50, 86, 198, 163, 74, 22, 55, 65, 183, 223, 215, 88, 202, 115, 52, 147, 62, 30, 247, 217, 168, 121, 47, 124, 119, 153, 102, 106, 176, 23, 12, 235, 239, 60, 227, 191, 160, 19, 144, 0, 10, 182, 224, 245, 192, 81, 242, 225, 173, 204, 190, 103, 18, 5, 2, 213, 82, 167, 193, 70, 68, 220, 127, 150, 4, 194, 16, 229, 250, 240, 8, 1, 38, 199, 152, 212, 117, 218, 80, 166, 116, 43, 232, 161, 78, 236, 29, 222, 201, 15, 42, 104, 164, 46, 109, 14, 184, 58, 234, 69, 99, 142, 122, 214, 175};
    for (j = 0; j < NUM128_SIZE; j++)
    {
        for (i = 128 / NUM128_SIZE - 1; i >= 0; i--)
        {
            int current_index = j * 8 + i + 1, x, y;
            if (b.part[j] & mask)
            {
                x = p_index[current_index] / 8;
                y = p_index[current_index] % 8;
                result.part[x - 1] |= mask << (8 - y);
            }
            b.part[j] = b.part[j] >> 1;
        }
    }

    return result;
}

void keygen_enhanced(num128 *ptk, num128 k)
{
    int i, j;
    char tmp = 0;
    *(ptk + 1) = k;
    for (i = 2; i < NR + 2; i++)
    {
        tmp = k.part[0];
        for (j = 0; j < NUM128_SIZE - 1; j++)
        {
            k.part[j] = k.part[j + 1];
        }
        k.part[NUM128_SIZE - 1] = tmp;
        *(ptk + i) = k;
    }
}

num128 spn_enhanced(num128 x, num128 *k)
{
    int i, r;
    num128 u, v, y = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                 w = x; //w0 = x
    for (r = 1; r < NR; r++)
    {
        for (i = 0; i < NUM128_SIZE; i++)
            u.part[i] = w.part[i] ^ k[r].part[i];
        v = sub_enhanced(u);
        w = per_enhanced(v);
    }
    for (i = 0; i < NUM128_SIZE; i++)
        u.part[i] = w.part[i] ^ k[NR].part[i];
    v = sub_enhanced(u);
    for (i = 0; i < NUM128_SIZE; i++)
        y.part[i] = v.part[i] ^ k[NR + 1].part[i];
    return y;
}
