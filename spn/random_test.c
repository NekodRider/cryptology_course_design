#include <stdio.h>
#include <string.h>
#include <time.h>
#include "enhanced_spn.h"
int main()
{
    clock_t t1,t2;
    double duration;
    int i,j;
    num128 tmp_num128, tmp, tmp_last,
        k = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        k_group[14],
        *ptk = k_group;
    unsigned int tmp_uint,length;
    unsigned char tmp_ch,*pch = &tmp_ch,buf[16];
    char path_in[10] = "plain.txt",
         path_out[10] = "out.txt";
    FILE *fp_in, *fp_out;
    fp_in = fopen(path_in,"wb");
    memset(buf,0,sizeof(buf));
    for(i=0;i<1000000;i++){
        fwrite(buf, sizeof(unsigned char), 16, fp_in);
    }
    fclose(fp_in);


    fp_in = fopen(path_in, "rb");
    fp_out = fopen(path_out, "wb");
    tmp_last = k;
    while((length = fread(buf,16,1,fp_in)) > 0){
        
        for(i=0;i<16;i++){
            tmp_ch = buf[i];
            unsigned char r=0;
            for(j=0;j<8;j++){
                int mask = 0x1;
                if(mask&(tmp_ch>>j)&&(tmp_last.part[i]>>j)&mask)
                    r |= 0;
                if(!(mask&(tmp_ch>>j))&&!((tmp_last.part[i]>>j)&mask))
                    r |= 0;
                else
                    r |= mask<<j;
            }
            
            tmp_num128.part[i]=r;
        }
        keygen_enhanced(ptk, k);
        tmp = spn_enhanced(tmp_num128, ptk);
        for(i=0;i<16;i++){
            tmp_ch = tmp_last.part[i] = tmp.part[i];
            fwrite(pch, sizeof(unsigned char), 1, fp_out);
        }
    }
    fclose(fp_in);
    fclose(fp_out);
    return 0;
}