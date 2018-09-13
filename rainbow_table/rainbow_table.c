#include <stdio.h>
#include <time.h>
#include<stdlib.h>
#include <openssl/md5.h>
#define R_TIME 500
#define MAXSIZE 6
unsigned char PASSCHAR[] = "0123456789abcdefghijklmnopqrstuvwxyz";

void reduction(unsigned char *y, unsigned char *x, unsigned int index)
{
    int i;
    for (i = 0; i < 6; i++)
    {
        y[i] = PASSCHAR[(unsigned char)(x[i] ^ (index%12)) % 36];
    }
}

void table_generate(FILE *fp, unsigned int END)
{
    unsigned char start_str[6], tmp_str[6], md5_str[16];
    unsigned int i, j, START = 0;
    while (START < END)
    {
        unsigned int x;
        // x = START;
        for (i = 0; i < MAXSIZE; i++)
        {
            x = rand()%36;
            start_str[i] = tmp_str[i] = PASSCHAR[x];
        }

        for (i = 0; i < R_TIME; i++)
        {
            MD5_CTX md5_ctx;
            MD5_Init(&md5_ctx);
            MD5_Update(&md5_ctx, tmp_str, 6);
            MD5_Final(md5_str, &md5_ctx);
            reduction(tmp_str, md5_str, i);
        }
        fwrite(start_str, 6, sizeof(unsigned char), fp);
        fwrite(tmp_str, 6, sizeof(unsigned char), fp);
        START++;
    }
}

int search(unsigned char *p, FILE *fp, unsigned char *q)
{
    unsigned char start_str[7], tmp_str[7], md5_str[17], buf[12];
    start_str[6]=tmp_str[6]=md5_str[16]='\0';
    unsigned int i, j, length;
    MD5_CTX md5_ctx;
    reduction(start_str, q, 0);
    while ((length = fread(buf, 12, sizeof(unsigned), fp)) > 0)
    {
        int flag = 0;
        for (i = 0; i < 6; i++)
        {
            if (start_str[i] != buf[6 + i])
            {
                flag = 1;
                break;
            }
            tmp_str[i] = start_str[i];
        }
        if (flag == 1)
            continue;
        printf("%d %s",flag,tmp_str);
        for (i = 0; i < R_TIME - 1; i++)
        {
            MD5_Init(&md5_ctx);
            MD5_Update(&md5_ctx, tmp_str, 6);
            MD5_Final(md5_str, &md5_ctx);
            reduction(tmp_str, md5_str, i);
        }
        MD5_Init(&md5_ctx);
        MD5_Update(&md5_ctx, tmp_str, 6);
        MD5_Final(md5_str, &md5_ctx);
        for (i = 0; i < 16; i++)
        {
            if (md5_str[i] != q[i])
            {
                flag = 1;
                break;
            }
        }
        if (flag == 1)
            continue;
        for(i=0;i<6;i++)
            p[i] = tmp_str[i];
        return 1;
    }
    return 0;
}

int main()
{
    int i,j,x;
    unsigned char input[7],ch,res[7], tmp[16];
    srand(time(NULL));
    input[6]='\0';
    res[6]='\0';
    FILE *fp;
    printf("Do you want to generate rainbow table(Y/Any):");
    if((ch=getchar())=='Y'){
        fp = fopen("table_200.txt", "wb");
        table_generate(fp, 2000000);
    }
    // printf("Enter P:");
    // scanf("%s",input);
    // MD5_CTX md5_ctx;
    // MD5_Init(&md5_ctx);
    // MD5_Update(&md5_ctx, input, 6);
    // MD5_Final(tmp, &md5_ctx);
    // printf("So the Q is:");
    // for(i=0;i<16;i++)
    //     printf("%02x ",tmp[i]);
    
    fp = fopen("table.txt", "rb");
    int s=0;
    
    for(i=0;i<100000;i++){
        x = i;
        for (j = 0; j < MAXSIZE; j++)
        {
            input[j] = PASSCHAR[x%36];
            x=x/36;
        }
        MD5_CTX md5_ctx;
        MD5_Init(&md5_ctx);
        MD5_Update(&md5_ctx, input, 6);
        MD5_Final(tmp, &md5_ctx);
        if(search(res,fp,tmp)==1){
            s++;
            printf("\nFind the P:%s",res);
        }
    }
    printf("success rate %.2f%%",(double)s/1000);
    
    fclose(fp);
    return 0;
}
