#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include<ws2tcpip.h>
#include <openssl/md5.h>
#include <openssl/des.h>
#include <openssl/ec.h>
#include <openssl/comp.h>
int main()
{
    MD5_CTX md5_ctx, *p_md5_ctx = &md5_ctx;
    DES_cblock key;
    DES_key_schedule key_schedule;
    unsigned char md5_res[16], key_str[10] = "hustyyw",ep_des[120],comp_res[114],m_buf[144];
    char buf[128],path_in[100],path_out[100];
    int length, i, nid, ec_crv_len, total_len=0;
    FILE *fp_in,*fp_out;
    EC_KEY *ec_key;
    EC_builtin_curve *ec_curves;
    EC_GROUP *ec_group;

    
    
    memset(m_buf, 0, sizeof(m_buf));
    memset(buf, 0, sizeof(buf));
    memset(md5_res, 0, sizeof(md5_res));
    memset(comp_res, 0, sizeof(comp_res));
    memset(ep_des, 0, sizeof(ep_des));
    DES_string_to_key(key_str, &key);
    DES_set_key_unchecked(&key, &key_schedule);
    printf("pls input raw filename:");
    scanf("%s",path_in);
    if((fp_in = fopen(path_in, "rb"))==NULL){
        printf("cant open file!\n");
        return -1;
    }
    printf("pls input PGP filename:");
    scanf("%s",path_out);
    //fp_out = fopen(path_out,"wb");
    while ((length = fread(buf, 1, 128, fp_in)) > 0)
    {
        MD5_Init(p_md5_ctx);
        MD5_Update(p_md5_ctx, buf, length);
        MD5_Final(md5_res, p_md5_ctx);
        // for (i = 0; i < 16; i++)
        //     printf("%02x ", md5_res[i]);
        // printf("\n");
        for(i=0;i<16;i++)
            m_buf[i]=md5_res[i];
        for(i=16;i<144;i++)
            m_buf[i]=buf[i-16];
        // for (i = 0; i < 144; i++)
        //     printf("%02x ",m_buf[i]);
        COMP_CTX *p_comp_ctx = COMP_CTX_new(COMP_rle());
        COMP_compress_block(p_comp_ctx,comp_res,114,m_buf,144);
        COMP_CTX_free(p_comp_ctx);
        for (i = 0; i < 114; i++)
            printf("%02x ", comp_res[i]);
        DES_cblock ivec;
        memset((char *)&ivec, 0, sizeof(ivec));
        DES_ncbc_encrypt(comp_res, ep_des, 120, &key_schedule, &ivec, DES_ENCRYPT);

        for (i = 0; i < 120; i++)
            printf("%02x", ep_des[i]);
        printf("\n");
        memset(m_buf, 0, sizeof(m_buf));
        memset(buf, 0, sizeof(buf));
        memset(md5_res, 0, sizeof(md5_res));
        memset(comp_res, 0, sizeof(comp_res));
        memset(ep_des, 0, sizeof(ep_des));
    }
    
    

    // if((ec_key=EC_KEY_new())==NULL)
    // {
    //     printf("EC_KEY_new error!\n");
    //     return -1;
    // }
    // /* 获取实现的椭圆曲线个数 */
    // ec_crv_len = EC_get_builtin_curves(NULL, 0);
    // ec_curves = (EC_builtin_curve *)malloc(sizeof(EC_builtin_curve) * ec_crv_len);
    // /* 获取椭圆曲线列表 */
    // EC_get_builtin_curves(ec_curves, ec_crv_len);
    // /* 选取一种椭圆曲线 */
    // nid=ec_curves[25].nid;
    // /* 根据选择的椭圆曲线生成密钥参数 group */
    // if((ec_group=EC_GROUP_new_by_curve_name(nid))==NULL)
    // {
    //     printf("EC_GROUP_new_by_curve_name error!\n");
    //     return -1;
    // }
    // EC_KEY_set_group(ec_key,ec_group);
    // EC_KEY_generate_key(ec_key);
    // if(EC_KEY_check_key(ec_key)!=1)
    // {
    //     printf("check key error.\n");
    //     return -1;
    // }


    // for (i = 0; i < 16; i++)
    //     printf("%02x", md5_res[i]);
    

    // memset((char*)&ivec, 0, sizeof(ivec));
    // DES_ncbc_encrypt(ep_des, dp_des, len, &key_schedule, &ivec, 0);
    fclose(fp_in);
    fclose(fp_out);
    return 0;
}