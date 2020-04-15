
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char data[128][32][4];
unsigned char row[16];
unsigned char temp[8192];
unsigned char numout[16384];
unsigned char test[8192];


char hello[32];

int main ( void )
{
    FILE *fp;
    unsigned int rx,ry,rz;
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rt;
    unsigned int fn;
    unsigned int rtot;


    unsigned int start[10];



    rtot=0;

printf("const unsigned char fontdata[]={\n");

    for(fn=0;fn<10;fn++)
    {
        start[fn]=rtot;
        sprintf(hello,"%u.bmp",fn);

        fp=fopen(hello,"rb");
        if(fp==NULL) return(1);

        fread(data,1,138,fp);
        fread(data,1,sizeof(data),fp);
        fclose(fp);

//if(fn==0)
//{

        //for(ry=0;ry<3;ry++)
        //{
            //for(rx=0;rx<32;rx++)
            //{
                //fprintf(stderr,"%02X ",data[ry][rx][2]);
            //}
            //fprintf(stderr,"\n");
        //}
//}

        rt=0;
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
                temp[rt++]=row[ra];
            }
            for(ra=0;ra<16;ra++)
            {
                temp[rt++]=row[ra];
            }
        }

//        printf("%u\n",rt);

        rx=0;
        rb=temp[0];
        rc=1;
        for(ra=1;ra<rt;ra++)
        {
            if(temp[ra]==rb)
            {
                if(rc==255)
                {
                    numout[rx++]=rc;
                    numout[rx++]=rb;
                    rb=temp[ra];
                    rc=1;
                }
                else
                {
                    rc++;
                }
            }
            else
            {
                numout[rx++]=rc;
                numout[rx++]=rb;
                rb=temp[ra];
                rc=1;
            }
        }
        numout[rx++]=rc;
        numout[rx++]=rb;
        numout[rx++]=0;
        numout[rx++]=0;



        ry=0;
        for(ra=0;numout[ra];ra+=2)
        {
            rc=numout[ra+0];
            rb=numout[ra+1];
            for(rz=0;rz<rc;rz++) test[ry++]=rb;
        }
        rc=0;
        for(ra=0;ra<rx;ra++)
        {
            if(test[ra]!=temp[ra])
            {
                rc++;
                fprintf(stderr,"%u %02X %02X\n",rc,test[ra],temp[ra]);
            }
        }
        if(rc) exit(1);











        printf("//----- %u %u %u -----\n",fn,rx,rtot);
        rtot+=rx;

        for(ra=0;ra<rx;ra+=2)
        {
            printf("0x%02X,0x%02X,\n",numout[ra+0],numout[ra+1]);
        }

        rc=0;
        for(ra=0;numout[ra];ra+=2)
        {
            rc+=numout[ra];
        }
  //      printf("%u\n",rc);
    }
    printf("};\n");
    printf("const unsigned int datastart[10]={\n");
    for(ra=0;ra<10;ra++)
    {
        printf("%u,\n",start[ra]);
    }
    printf("//%u\n",rtot);
    printf("};\n");


    //printf("const unsigned char tempout[1024]={\n");
    //for(ra=0;ra<1024;ra++)
    //{
        //printf("%u,\n",temp[ra]);
    //}
    //printf("};\n");


    return(0);
}


