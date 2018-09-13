#include<stdio.h>
#include<openssl/md5.h>

#define MAXSIZE 6
unsigned char PASSCHAR[]="0123456789abcdefghijklmnopqrstuvwxyz";

void table_generate(char* filename){
    char tmp_str[6],md5_str[128];
    int i,j;
    for(i=0;i<MAXSIZE;i++){
        
    }


    MD5_CTX *md5_ctx;
    MD5_Init(md5_ctx);
    MD5_Update(md5_ctx,tmp_str,6);
    MD5_Final(md5_str,md5_ctx);
}

void reduction(unsigned int a,unsigned int index){

}

int main()
{

    return 0;
}
