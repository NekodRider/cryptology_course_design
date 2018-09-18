#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <openssl/md5.h>
#include <openssl/des.h>
#include <openssl/ec.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
int main()
{
    MD5_CTX md5_ctx, *p_md5_ctx = &md5_ctx;
    DES_cblock key;
    DES_key_schedule key_schedule;
    DES_cblock ivec;
    unsigned char md5_res[16], ep_des[128], *signature, share_key[20];
    char buf[128], path_in[100], path_out[100];
    unsigned int length, i, nid, ec_crv_len, sig_len = 0;
    BIO *fp_in, *fp_out, *fp_alice_pub, *fp_bob_pub;
    EC_KEY *ec_key;
    EC_builtin_curve *ec_curves;
    EC_GROUP *ec_group;
    //ENGINE *engine = ENGINE_get_cipher_engine();
    memset(buf, 0, sizeof(buf));
    memset(md5_res, 0, sizeof(md5_res));
    memset(ep_des, 0, sizeof(ep_des));

    printf("please input raw filename:");
    scanf("%s", path_in);
    if ((fp_in = BIO_new_file(path_in, "rb")) == NULL)
    {
        printf("cant open file!\n");
        return -1;
    }
    printf("please input PGP filename:");
    scanf("%s", path_out);
    fp_out = BIO_new_file(path_out, "wb");
    fp_alice_pub = BIO_new_file("alice_pub.pem", "wb");

    ec_key = EC_KEY_new();
    /* 获取实现的椭圆曲线个数 */
    ec_crv_len = EC_get_builtin_curves(NULL, 0);
    ec_curves = (EC_builtin_curve *)malloc(sizeof(EC_builtin_curve) * ec_crv_len);
    /* 获取椭圆曲线列表 */
    EC_get_builtin_curves(ec_curves, ec_crv_len);
    /* 选取一种椭圆曲线 */
    nid = ec_curves[25].nid;
    /* 根据选择的椭圆曲线生成密钥参数 group */
    ec_group = EC_GROUP_new_by_curve_name(nid);
    EC_KEY_set_group(ec_key, ec_group);
    EC_KEY_generate_key(ec_key);
    /* Alice 公钥 */
    PEM_write_bio_EC_PUBKEY(fp_alice_pub, ec_key);
    BIO_flush(fp_alice_pub);
    printf("public key generated!\n");
    getchar();
    getchar();
    /* 获取对方公钥 */
    fp_bob_pub = BIO_new_file("bob_pub.pem", "rb");
    const EC_POINT *bob_pub_point;
    EC_KEY *bob_pub_key = EC_KEY_new();

    PEM_read_bio_EC_PUBKEY(fp_bob_pub, &bob_pub_key, NULL, NULL);
    bob_pub_point = EC_KEY_get0_public_key(bob_pub_key);

    /* 获取共享密钥 */
    ECDH_compute_key(share_key, 20, bob_pub_point, ec_key, NULL);
    printf("shared key:");
    for (i = 0; i < 20; i++)
        printf("%02x ", share_key[i]);
    getchar();
    getchar();
    DES_string_to_key(share_key, &key);
    DES_set_key_unchecked(&key, &key_schedule);

    MD5_Init(p_md5_ctx);

    while ((length = BIO_read(fp_in, buf, 128)) > 0)
    {
        MD5_Update(p_md5_ctx, buf, 128);
        memset((char *)&ivec, 0, sizeof(ivec));
        DES_ncbc_encrypt(buf, ep_des, 128, &key_schedule, &ivec, DES_ENCRYPT);
        BIO_write(fp_out, ep_des, 128);
        // for (i = 0; i < 120; i++)
        //     printf("%02x", ep_des[i]);
        memset(buf, 0, sizeof(buf));
        memset(ep_des, 0, sizeof(ep_des));
    }
    MD5_Final(md5_res, p_md5_ctx);
    printf("md5:");
    for (i = 0; i < 16; i++)
        printf("%02x ", md5_res[i]);
    getchar();
    getchar();

    signature = (unsigned char *)malloc(48);
    /* 签名数据，本例未做摘要，可将 digest 中的数据看作是 sha1 摘要结果 */
    if (ECDSA_sign(0, md5_res, 16, signature, &sig_len, ec_key) != 1)
    {
        printf("sign err!\n");
        return -1;
    }
    printf("\nsignature:");
    for (i = 0; i < 48; i++)
        printf("%02x ", signature[i]);
    getchar();
    getchar();
    memset((char *)&ivec, 0, sizeof(ivec));
    DES_ncbc_encrypt(signature, ep_des, 48, &key_schedule, &ivec, DES_ENCRYPT);
    BIO_write(fp_out, ep_des, 48);
    BIO_flush(fp_out);

    printf("PGP completed!");
    getchar();
    return 0;
}