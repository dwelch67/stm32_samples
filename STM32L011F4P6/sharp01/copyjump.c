

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned int data[0x10000];



int main ( int argc, char *argv[] )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int len;

    FILE *fp;

    if(argc<3)
    {
        printf("copyjump input.bin output.s\n");
        return(1);
    }
    memset(data,0xFF,sizeof(data));

    fp=fopen(argv[1],"rb");
    if(fp==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    len=fread(data,1,sizeof(data),fp);
    fclose(fp);
    printf("read %u bytes\n",len);
    len=(len+0xF)&((~0)<<4); //4 words
if(len>0x600)
{
    printf("PROGRAM TOO BIG\n");
    return(1);
}
    fp=fopen(argv[2],"wt");
    if(fp==NULL)
    {
        printf("Error creating file [%s]\n",argv[2]);
        return(1);
    }

    fprintf(fp,".cpu cortex-m0             \n");
    fprintf(fp,".thumb                     \n");
    fprintf(fp,".thumb_func                \n");
    fprintf(fp,".global _start             \n");
    fprintf(fp,"_start:                    \n");
    fprintf(fp,"stacktop: .word 0x20000800 \n");
    fprintf(fp,".word reset                \n");
    fprintf(fp,".word hang                 \n");
    fprintf(fp,".word hang                 \n");
    fprintf(fp,".word hang                 \n");
    fprintf(fp,".word hang                 \n");
    fprintf(fp,".word hang                 \n");
    fprintf(fp,".word hang                 \n");
    fprintf(fp,".word hang                 \n");
    fprintf(fp,".word hang                 \n");
    fprintf(fp,".word hang                 \n");
    fprintf(fp,".word hang                 \n");
    fprintf(fp,".word hang                 \n");
    fprintf(fp,".word hang                 \n");
    fprintf(fp,".word hang                 \n");
    fprintf(fp,".word hang                 \n");
    fprintf(fp,".thumb_func                \n");
    fprintf(fp,"hang:   b .                \n");
    fprintf(fp,".thumb_func                \n");
    fprintf(fp,"reset:                     \n");
//65khz
    fprintf(fp,"  ldr r0,rccreg       \n");
    fprintf(fp,"  ldr r1,[r0]       \n");
    //fprintf(fp,"  bic r1,#0x2000   \n");
    fprintf(fp,"  ldr r2,rccmask\n");
    fprintf(fp,"  and r1,r2           \n");
    fprintf(fp,"  str r1,[r0]       \n");
//copy
    fprintf(fp,"  ldr r0,program_base     \n");
    fprintf(fp,"  ldr r1,sram_base        \n");
    fprintf(fp,"  ldr r2,program_length   \n");
    fprintf(fp,"copy_loop:                \n");
    fprintf(fp,"  ldmia r0!,{r4,r5,r6,r7} \n");
    fprintf(fp,"  stmia r1!,{r4,r5,r6,r7} \n");
    //fprintf(fp,"  subs r2,#1               \n");
    fprintf(fp,"  sub r2,#1               \n");
    fprintf(fp,"  bne copy_loop           \n");
//jump
    fprintf(fp,"  ldr r0,sram_base        \n");
    fprintf(fp,"  add r0,#1               \n");
    fprintf(fp,"  bx r0                   \n");
    fprintf(fp,".align 6                   \n");

    fprintf(fp,"rccreg:         .word 0x40021004\n");
    fprintf(fp,"rccmask:        .word 0xFFFF1FFF\n"); //bit 13
    fprintf(fp,"program_base:   .word program\n");
    fprintf(fp,"sram_base:      .word 0x20000000\n");
    fprintf(fp,"program_length: .word 0x%08X\n",len>>4);
    fprintf(fp,"program:\n");
    for(ra=0;ra<(len>>2);ra++) fprintf(fp,".word 0x%08X\n",data[ra]);


    fclose(fp);

    return(0);
}

