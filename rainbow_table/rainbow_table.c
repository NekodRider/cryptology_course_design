#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#define R_TIME 1000
#define LEN 6
#define MSIZE 36
unsigned char PASSCHAR[] = "0123456789abcdefghijklmnopqrstuvwxyz";

void reduction(unsigned char *encrypt, unsigned char *decrypt, unsigned int index)
{
    int i;
    unsigned char a, b;
    index = index % R_TIME;
    for (i = 0; i < LEN; i++)
    {
        a = decrypt[i] ^ decrypt[(i + 1) & 0xf] ^ decrypt[(i + 2) & 0xf] ^ decrypt[(i + 3) & 0xf];
        b = decrypt[index & 0xf] + decrypt[(index - 1) & 0xf] + decrypt[(index - 2) & 0xf] + decrypt[(index - 3) & 0xf] + decrypt[(index - 4) & 0xf];
        encrypt[i] = PASSCHAR[(a + b) % MSIZE];
        index = index * 3 + 1;
    }
    encrypt[LEN] = '\0';
}

void table_generate(FILE *fp, unsigned int END)
{
    unsigned char start_str[7], tmp_str[7], md5_str[16];
    unsigned int i, j, START = 0;
    start_str[6] = tmp_str[6] = '\0';
    while (START < END)
    {
        unsigned int x;
        // x = START;
        for (i = 0; i < LEN; i++)
        {
            x = (rand() + START) % 36;
            start_str[i] = tmp_str[i] = PASSCHAR[x];
        }

        for (i = 0; i < R_TIME; i++)
        {
            MD5_CTX md5_ctx;
            MD5(tmp_str, 6, md5_str);
            reduction(tmp_str, md5_str, i);
        }
        fwrite(start_str, 6, sizeof(unsigned char), fp);
        fwrite(tmp_str, 6, sizeof(unsigned char), fp);
        START++;
        if (START % 10000 == 0)
            printf("%s %s - %d\n", start_str, tmp_str, START);
    }
}

int search(unsigned char *p, FILE *fp, unsigned char *q)
{
    unsigned char start_str[7], tmp_str[7], md5_str[17], buf[13], hash_chain[R_TIME][17], r_chain[R_TIME][7];
    start_str[6] = tmp_str[6] = md5_str[16] = '\0';
    int i, j, k, m, length, chain_flag = 0;
    MD5_CTX md5_ctx;
    //time_t t1,t2;
    //double duration;
    //t1=clock();
    for (k = R_TIME - 1; k >= 0; k--)
    {
        if (k % 100 == 0)
            printf("Working On P(%d)\n", k);
        for (i = 0; i < 16; i++)
            md5_str[i] = q[i];
        for (i = k; i < R_TIME - 1; i++)
        {
            reduction(start_str, md5_str, i);
            MD5(start_str, 6, md5_str);
        }
        reduction(start_str, md5_str, R_TIME - 1);

        rewind(fp);
        while ((length = fread(buf, 12, sizeof(unsigned char), fp)) > 0)
        {
            int flag = 0;
            for (i = 0; i < 6; i++)
            {
                if (start_str[i] != buf[6 + i])
                {
                    flag = 1;
                    break;
                }
            }
            if (flag == 1)
                continue;
            for (i = 0; i < 6; i++)
                tmp_str[i] = buf[i];
            for (i = 0; i < k; i++)
            {
                MD5(tmp_str, 6, md5_str);
                reduction(tmp_str, md5_str, i);
            }
            MD5(tmp_str, 6, md5_str);
            for (i = 0; i < 16; i++)
            {
                if (md5_str[i] != q[i])
                    break;
                else if (i == 15)
                {
                    for (j = 0; j < 6; j++)
                        p[j] = tmp_str[j];
                    //t2=clock();
                    //duration = (double)(t2 - t1) / CLOCKS_PER_SEC;
                    //printf("Using %ds.\n",duration);
                    return 1;
                }
            }
        }
    }
    //t2=clock();
    //duration = t2 - t1;
    //printf("Using %ds.\n",duration/CLOCKS_PER_SEC);
    return 0;
}

int main()
{
    int i, j, x;
    unsigned char input[7], ch, res[7], tmp[17], test[7] = "n1lvh2", md5_str[17];
    srand(time(NULL));
    input[6] = res[6] = '\0';
    FILE *fp;
    printf("Do you want to generate rainbow table(Y/Any):");
    if ((ch = getchar()) == 'Y')
    {
        fp = fopen("table.txt", "wb");
        table_generate(fp, 10000000);
    }
    // for (i = 0; i < R_TIME; i++)
    // {
    //     MD5(test,6,md5_str);
    //     reduction(test, md5_str, i);
    //     printf("%s\n",test);
    // }

    fp = fopen("table.txt", "rb");
    int s = 0;

    for (i = 0; i < 10; i++)
    {

        for (j = 0; j < LEN; j++)
        {
            x = (rand() + j) % 36;
            input[j] = PASSCHAR[x];
        }
        // unsigned char e_input[] = "3nv11i";
        // e_input[6]='\0';
        input[6] = '\0';
        printf("Now testing No.%d - %s\n", i + 1, input);
        MD5(input, 6, tmp);
        if (search(res, fp, tmp) == 1)
        {
            s++;
            printf("Found the P:%s\n", res);
        }
        else
            printf("Failed.\n");
    }
    printf("success rate %.2f%%", (double)s * 10);

    fclose(fp);
    return 0;
}
