
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned char data[128][32][4];

unsigned char row[16];
int main ( void )
{
    FILE *fp;
    unsigned int rx,ry,rz;
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    fp=fopen("zero.bmp","rb");
    if(fp==NULL) return(1);

    fread(data,1,138,fp);
    fread(data,1,sizeof(data),fp);
    fclose(fp);

    //for(ry=0;ry<3;ry++)
    //{
        //for(rx=0;rx<32;rx++)
        //{
            //printf("%02X ",data[127-ry][rx][2]);
        //}
        //printf("\n");
    //}


    for(rx=0;rx<32;rx++)
    {
        rb=0;
        rc=0;
        for(ry=0;ry<128;ry++)
        {
            rc<<=1;
            if(data[ry][rx][2]==0x00)
            {
                rc|=1;
            }
            if((ry&7)==7)
            {
                row[rb++]=rc;
                rc=0;
            }
        }
        for(ra=0;ra<16;ra++)
        {
            printf("0x%02X,",row[ra]);
        }
        printf("\n");
        for(ra=0;ra<16;ra++)
        {
            printf("0x%02X,",row[ra]);
        }
        printf("\n");
    }


    return(0);
}


