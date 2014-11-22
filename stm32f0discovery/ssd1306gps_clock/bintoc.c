
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

char newname[8192];
unsigned int data[8192];

int main ( int argc, char *argv[] )
{
    unsigned int ra;
    unsigned int rb;

    if(argc<2)
    {
        printf("bin file not specified\n");
        return(1);
    }

    fp=fopen(argv[1],"rb");
    if(fp==NULL) return(1);
    rb=fread(data,1,sizeof(data),fp);
    fclose(fp);
    rb=rb+3;
    rb>>=2;
    printf("%u\n",rb);
    sprintf(newname,"%s.s",argv[1]);
    fp=fopen(newname,"wt");
    if(fp==NULL) return(1);




fprintf(fp,".thumb_func                       \n");
fprintf(fp,".global _start                    \n");
fprintf(fp,"_start:                           \n");
fprintf(fp,"stacktop: .word 0x20001000        \n");
fprintf(fp,".word reset                       \n");
fprintf(fp,".word hang                        \n");
fprintf(fp,".word hang                        \n");
fprintf(fp,".word hang                        \n");
fprintf(fp,".word hang                        \n");
fprintf(fp,".word hang                        \n");
fprintf(fp,".word hang                        \n");
fprintf(fp,".word hang                        \n");
fprintf(fp,".word hang                        \n");
fprintf(fp,".word hang                        \n");
fprintf(fp,".word hang                        \n");
fprintf(fp,".word hang                        \n");
fprintf(fp,".word hang                        \n");
fprintf(fp,".word hang                        \n");
fprintf(fp,".word hang                        \n");
fprintf(fp,"                                  \n");
fprintf(fp,".thumb_func                       \n");
fprintf(fp,"hang:   b .                       \n");
fprintf(fp,".thumb_func                       \n");
fprintf(fp,"reset:                            \n");
fprintf(fp,"    ldr r0,dataptr                     \n");
fprintf(fp,"    ldr r1,ramadd                 \n");
fprintf(fp,"    ldr r2,nwords                 \n");
fprintf(fp,"copyloop:                     \n");
fprintf(fp,"    ldr r3,[r0]                 \n");
fprintf(fp,"    add r0,r0,#4               \n");
fprintf(fp,"    str r3,[r1]                 \n");
fprintf(fp,"    add r1,r1,#4               \n");
fprintf(fp,"    sub r2,r2,#1                 \n");
fprintf(fp,"    bne copyloop                 \n");
fprintf(fp,"    ldr r1,ramadd                 \n");
fprintf(fp,"    add r1,r1,#1                 \n");
fprintf(fp,"    bx r1                 \n");
fprintf(fp,";@-----------------------         \n");
fprintf(fp,".align                            \n");
fprintf(fp,"nwords: .word %u                  \n",rb);
fprintf(fp,"ramadd: .word 0x20000000          \n");
fprintf(fp,"dataptr: .word database          \n");
fprintf(fp,"database:                       \n");
for(ra=0;ra<rb;ra++) fprintf(fp,".word 0x%08X ;@0x%08X\n",data[ra],0x20000000+(ra<<2));

    fclose(fp);

    
}



