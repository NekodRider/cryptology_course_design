#include <stdio.h>
#include "enhanced_spn.h"
int main()
{
    int i;
    num128 tmp_num128, tmp,
        k = {0x3A, 0x94, 0xD6, 0x3F, 0x3A, 0x94, 0xD6, 0x3F, 0x3A, 0x94, 0xD6, 0x3F, 0x3A, 0x94, 0xD6, 0x3F},
        k_group[14],
        *ptk = k_group;
    unsigned int tmp_uint;
    unsigned char tmp_ch,*pch = &tmp_ch;
    char path_in[10] = "plain.txt",
         path_out[10] = "out.txt";
    FILE *fp_in, *fp_out;
    fp_in = fopen(path_in, "rb");
    fp_out = fopen(path_out, "wb");
    while(!feof(fp_in)){
        
        for(i=0;i<16;i++){
            fread(pch, sizeof(unsigned char), 1, fp_in);
            tmp_num128.part[i]=tmp_ch;
        }
        keygen_enhanced(ptk, k);
        tmp = spn_enhanced(tmp_num128, ptk);
        for(i=0;i<16;i++){
            tmp_ch = tmp.part[i];
            fwrite(pch, sizeof(unsigned char), 1, fp_out);
        }
    }
    fclose(fp_in);
    fclose(fp_out);
    return 0;
}