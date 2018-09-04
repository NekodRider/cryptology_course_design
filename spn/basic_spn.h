#include <stdio.h>
#define NR 4

unsigned int sub(unsigned int a)
{
    int i = 0;
    unsigned int tmp, res = 0,
                      mask = 0xf,
                      s_index[16] = {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7};
    for (i = 0; i < 4; i++)
    {
        tmp = a & mask;
        res = res | (s_index[tmp] << (i * 4));
        a = a >> 4;
    }
    return res;
}

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

unsigned int per(unsigned int b)
{
    int i = 0;
    unsigned int tmp, res = 0,
                      mask = 0x1,
                      p_index[16] = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};
    for (i = 15; i > -1; i--)
    {
        if (b & mask)
        {

            res = res | (mask << (16 - p_index[i]));
            mask << (16 - p_index[i]);
        }
        b = b >> 1;
    }
    return res;
}

void keygen(unsigned int *ptk, unsigned int k)
{
    int i;
    unsigned int mask = 0xffff,
                 tmp = 0;
    *ptk = 0;
    tmp = k >> 16;
    tmp |= k << 16;
    k = tmp;
    for (i = 1; i < NR + 2; i++)
    {
        *(ptk + i) = mask & k;
        tmp = k << 4;
        tmp |= k >> (32 - 4);
        k = tmp;
    }
}

unsigned int spn(unsigned int x, unsigned int *k)
{
    int r;
    unsigned int u, v, y = 0,
                       w = x; //w0 = x
    for (r = 1; r < NR; r++)
    {
        u = w ^ k[r];
        v = sub(u);
        w = per(v);
    }
    u = w ^ k[NR];
    v = sub(u);
    y = v ^ k[NR + 1];
    return y;
}

unsigned int spn_inverse(unsigned int y, unsigned int *k)
{
    int r;
    unsigned int u, v, w, x = 0; //w0 = x
    v = y ^ k[NR + 1];
    u = sub_inverse(v);
    w = u ^ k[NR];
    for (r = NR-1; r > 0; r--)
    {
        v = per(w);
        u = sub_inverse(v);
        w = u ^ k[r];
    }
    x = w;
    return x;
}

char* uint2char(char* c,unsigned int a){
    int i=15;
    while(i>=0){
        c[i]='0';
        if((a>>(15-i))&1)
            c[i]='1';
        i--;
    }
    c[i]='\0';
}
