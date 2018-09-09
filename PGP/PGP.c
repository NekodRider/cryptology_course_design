#include<stdio.h>
#include<string.h>
#include<openssl/md5.h>
#include <openssl/des.h>
#include <openssl/rsa.h>
#include <openssl/comp.h>
int main(){
    MD5_CTX md5_ctx,*p_md5_ctx = &md5_ctx;
    unsigned char res[16];
    char filename[1000],buf[128];
    int length,i;
    FILE *fp;
    memset(buf,0,sizeof(buf));
    MD5_Init(p_md5_ctx);
    printf("请输入文件名:");
    scanf("%s",filename);
    fp=fopen(filename,"rb");
    if(fp==NULL)
    {
        printf("Can't open file\n");
        return 0;
    }
    while((length=fread(buf,1,128,fp))>0)
    {
        MD5_Update(p_md5_ctx,buf,length);
        memset(buf,0,sizeof(buf));
    }
    MD5_Final(res,p_md5_ctx);
    for(i=0;i<16;i++)
        printf("%x",res[i]);
    return 0;
}