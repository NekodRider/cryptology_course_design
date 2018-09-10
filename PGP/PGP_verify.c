#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include<ws2tcpip.h>
#include <openssl/md5.h>
#include <openssl/des.h>
#include <openssl/ec.h>
#include <openssl/comp.h>
int main()
{
    COMP_CTX *p_comp_ctx;
    MD5_CTX md5_ctx, *p_md5_ctx = &md5_ctx;
    DES_cblock key;
    DES_key_schedule key_schedule;
    unsigned char md5_res[16], md5_res_tmp[16], key_str[10] = "hustyyw",dp_des[120],comp_res[144],m_buf[114];
    char filename[1000], buf[120];
    int length, i, nid, ec_crv_len, total_len=0;
    FILE *fp;
    EC_KEY *ec_key;
    EC_builtin_curve *ec_curves;
    EC_GROUP *ec_group;

    
    COMP_CTX_new(COMP_zlib());
    memset(buf, 0, sizeof(buf));
    DES_string_to_key(key_str, &key);
    DES_set_key_unchecked(&key, &key_schedule);

    printf("please input PGP filename:");
    scanf("%s", filename);
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Can't open file\n");
        return -1;
    }

    while ((length = fread(buf, 1, 120, fp)) > 0)
    {
        MD5_Init(p_md5_ctx);
        DES_cblock ivec;
        memset((char *)&ivec, 0, sizeof(ivec));
        DES_ncbc_encrypt(buf, dp_des, 120, &key_schedule, &ivec, 0);
        COMP_expand_block(p_comp_ctx,comp_res,144,dp_des,114);
        for (i = 0; i < 16; i++)
            md5_res[i]=comp_res[i];
        for(i=16;i<144;i++)
            m_buf[i-16]=comp_res[i];
        MD5_Update(p_md5_ctx, m_buf, 128);
        MD5_Final(md5_res_tmp, p_md5_ctx);
        
        if(strcmp(md5_res,md5_res_tmp)!=0){
            printf("验证失败!\n");
            return -1;
        }

        memset(m_buf, 0, sizeof(m_buf));
        memset(buf, 0, sizeof(buf));
        memset(md5_res, 0, sizeof(md5_res));
        memset(comp_res, 0, sizeof(comp_res));
        memset(dp_des, 0, sizeof(dp_des));
    }
    printf("验证失败!\n");
    return 0;
}