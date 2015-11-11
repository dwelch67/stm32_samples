
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fpin;
FILE *fpout;

unsigned int data[1024];

unsigned int ra,rb;
unsigned int total;


int main ( int argc, char *argv[] )
{
    if(argc<2) return(1);
    fpin=fopen(argv[1],"rb");
    if(fpin==NULL) return(1);
    sprintf((char *)data,"%s.h",argv[1]);
    fpout=fopen((char *)data,"wt");
    if(fpout==NULL) return(1);

    fprintf(fpout,"\n");
    fprintf(fpout,"const unsigned int bindata[]=\n");
    fprintf(fpout,"{\n");

    total=0;
    while(1)
    {
        rb=fread(data,1,sizeof(data),fpin);
        if(rb==0) break;
        if(rb&3) rb+=4;
        rb>>=2;
        for(ra=0;ra<rb;ra++)
        {
            total++;
            fprintf(fpout,"0x%08X,\n",data[ra]);
        }
    }
    fprintf(fpout,"};\n");
    fprintf(fpout,"#define bindatalen %u\n",total);
    fprintf(fpout,"\n");




    return(0);
}

