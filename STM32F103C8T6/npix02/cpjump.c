
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FILE *fpin;
FILE *fpout;

unsigned int dlen;
unsigned int data[512<<10];


int main ( int argc, char *argv[] )
{
    unsigned int ra;

    if(argc<2)
    {
        printf("What?\n");
        return(1);
    }

    fpin=fopen(argv[1],"rb");
    if(fpin==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }

    sprintf((char *)data,"%s.s",argv[1]);
    fpout=fopen((char *)data,"wt");
    if(fpout==NULL)
    {
        printf("Error creating file [%s]\n",((char *)data));
        return(1);
    }

    fprintf(fpout,"\n");
    fprintf(fpout,"\n");
    fprintf(fpout,".thumb_func                 \n");
    fprintf(fpout,".global _start              \n");
    fprintf(fpout,"_start:                     \n");
    fprintf(fpout,"stacktop: .word 0x20001000  \n");
    fprintf(fpout,".word reset                 \n");
    fprintf(fpout,".word hang                  \n");
    fprintf(fpout,".word hang                  \n");
    fprintf(fpout,".word hang                  \n");
    fprintf(fpout,".word hang                  \n");
    fprintf(fpout,".word hang                  \n");
    fprintf(fpout,".word hang                  \n");
    fprintf(fpout,".word hang                  \n");
    fprintf(fpout,".word hang                  \n");
    fprintf(fpout,".word hang                  \n");
    fprintf(fpout,".word hang                  \n");
    fprintf(fpout,".word hang                  \n");
    fprintf(fpout,".word hang                  \n");
    fprintf(fpout,".word hang                  \n");
    fprintf(fpout,".word hang                  \n");
    fprintf(fpout,"                            \n");


    memset(data,0xFF,sizeof(data));
    dlen=fread(data,1,sizeof(data),fpin);
    if(dlen&0xF) dlen=(dlen+0x10)&(~0xF);

    //dlen=(dlen+0xFF)&(~0xFF); //pad to 256 bytes
    //dlen=(dlen+0x7F)&(~0x7F); //pad to 128 bytes
    dlen=(dlen+0x3F)&(~0x3F); //pad to 64 bytes
    //dlen=(dlen+0x1F)&(~0x1F); //pad to 32 bytes

    dlen>>=2; //four bytes per word

    fprintf(fpout,".align\n");
    fprintf(fpout,"program:\n");
    for(ra=0;ra<dlen;ra++)
    {
        fprintf(fpout,"  .word 0x%08X\n",data[ra]);
    }


    fprintf(fpout,".thumb_func               \n");
    fprintf(fpout,"reset:                       \n");
    fprintf(fpout,"    ldr r0,program_add       \n");
    fprintf(fpout,"    ldr r1,program_length    \n");
    fprintf(fpout,"    ldr r2,program_dest      \n");
    fprintf(fpout,"loadloop:                    \n");
//    fprintf(fpout,"    ldmia r0!,{r3,r4,r5,r6,r7,r8,r9,r10,r11,r12}  \n");
//    fprintf(fpout,"    stmia r2!,{r3,r4,r5,r6,r7,r8,r9,r10,r11,r12}  \n");
//    fprintf(fpout,"    ldmia r0!,{ r3,r4,r5,r6, r7,r8,r9,r10 }  \n");
//    fprintf(fpout,"    stmia r2!,{ r3,r4,r5,r6, r7,r8,r9,r10 }  \n");
    fprintf(fpout,"    ldmia r0!,{ r3,r4,r5,r6 }  \n");
    fprintf(fpout,"    stmia r2!,{ r3,r4,r5,r6 }  \n");
//    fprintf(fpout,"    ldmia r0!,{ r3,r4 }  \n");
//    fprintf(fpout,"    stmia r2!,{ r3,r4 }  \n");
//    fprintf(fpout,"    ldr r3,[r0],#4\n");
//    fprintf(fpout,"    str r3,[r2],#4\n");

    fprintf(fpout,"    sub r1,#1            \n");
    fprintf(fpout,"    bne loadloop         \n");
    fprintf(fpout,"                         \n");
    fprintf(fpout,"    ldr r0,program_dest  \n");
    fprintf(fpout,"    add r0,#1            \n"); //ORR is better but we know this is even
    fprintf(fpout,"    blx r0               \n");
    fprintf(fpout,".thumb_func              \n");
    fprintf(fpout,"hang: b hang             \n");
    
    fprintf(fpout,".align\n");
//    fprintf(fpout,"program_length: .word 0x%08X\n",dlen/10); //ten words copied per loop
//    fprintf(fpout,"program_length: .word 0x%08X\n",dlen>>3); //eight words copied per loop
    fprintf(fpout,"program_length: .word 0x%08X\n",dlen>>2); //four words copied per loop
//    fprintf(fpout,"program_length: .word 0x%08X\n",dlen>>1); //two word copied per loop
//    fprintf(fpout,"program_length: .word 0x%08X\n",dlen); //one word copied per loop
    fprintf(fpout,"program_add: .word program\n");
    fprintf(fpout,"program_dest: .word 0x20000000\n");

    fprintf(fpout,"\n");

    fprintf(fpout,"\n");
    fprintf(fpout,"\n");

    fclose(fpout);
    fclose(fpin);


    return(0);
}

