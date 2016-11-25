
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// a
//g b
// c
//f d
// e
//
char digtab[16]=
{
      //  gfe dcba
0x7B, //0 111 1011
0x0A, //1 000 1010
0x37, //2 011 0111
0x1F, //3 001 1111
0x4E, //4 100 1110
0x5D, //5 101 1101
0x7D, //6 111 1101
0x0B, //7 000 1011
0x7F, //8 111 1111
0x5F, //9 101 1111
};

char raw[4][64]=
{
{"A01,A02,C03,D02,B03,B04,A00,"},
{"A05,A06,A03,A15,C10,C11,A04,"},
{"B00,B01,C04,C07,C08,C09,C05,"},
{"B11,B12,B02,B13,B14,B15,B10,"},
};

char ptab[4][16];

unsigned int mask[4][4][10];
unsigned int zoff[4];


int main ( void )
{
    unsigned int rx,x;


    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
    unsigned int re;

    unsigned int hi,lo;

    memset(mask,0,sizeof(mask));

    for(rx=0;rx<4;rx++)
    {
        rb=0;
        rc=0;
        for(ra=0;raw[rx][ra];ra++)
        {
//printf("%u %u\n",rb,ra);
            if(raw[rx][ra]==',')
            {
                if((ra-rb)==3)
                {
                    switch(raw[rx][rb+0])
                    {
                        case 'A':
                        {
                            hi=0;
                            break;
                        }
                        case 'B':
                        {
                            hi=1;
                            break;
                        }
                        case 'C':
                        {
                            hi=2;
                            break;
                        }
                        case 'D':
                        {
                            hi=3;
                            break;
                        }
                        default:
                        {
                            printf("bad %u %u %c\n",rx,rb,raw[rx][rb]);
                            break;
                        }
                    }
                }
                else
                {
                    printf("Syntax error %u %u %s\n",ra,rb,&raw[rx][ra]);
                    return(1);
                }
                lo=(raw[rx][rb+1]-0x30)*10;
                lo+=(raw[rx][rb+2]-0x30);
                //printf("%c%02u,",'A'+hi,lo);
                ptab[rx][rc]=hi<<4;
                ptab[rx][rc]|=lo;
                //printf("0x%02X,\n",ptab[rx][rc]);
                rb=ra+1;
                rc++;
            }
        }
        printf("\n");
    }
    for(rx=0;rx<4;rx++)
    {
        for(rc=0;rc<7;rc++)
        {
            //printf("0x%02X,\n",ptab[rx][rc]);
        }
        printf("\n");
    }
    for(rx=0;rx<4;rx++)
    {
        for(ra=0;ra<10;ra++)
        {
            for(rb=0;rb<7;rb++)
            {
                rc=ptab[rx][rb]>>4;
                rd=ptab[rx][rb]&0xF;
                if(digtab[ra]&(1<<rb))
                {
                    mask[rx][rc][ra]|=(1<<rd)<<0;
                }
                else
                {
                    mask[rx][rc][ra]|=(1<<rd)<<16;
                }
            }
        }
    }

//char mask[4][4][10];
printf("const unsigned int mask[4][4][10]=\n");
printf("{\n");
    for(rx=0;rx<4;rx++)
    {
printf("{\n");
        for(rc=0;rc<4;rc++)
        {
printf("{\n");
            for(ra=0;ra<10;ra++)
            {
printf("0x%08X,\n",mask[rx][rc][ra]);

            }
printf("},\n");
        }
printf("},\n");
    }
printf("};\n");

    rx=0;
    zoff[0]=0;
    zoff[1]=0;
    zoff[2]=0;
    zoff[3]=0;
    for(rb=0;rb<7;rb++)
    {
        rc=ptab[rx][rb]>>4;
        rd=ptab[rx][rb]&0xF;
        {
            zoff[rc]|=(1<<rd)<<16;
        }
    }
printf("const unsigned int zero_off[4]=\n");
printf("{\n");
for(ra=0;ra<4;ra++)
{
    printf("0x%08X,\n",zoff[ra]);
}
printf("};\n");


    return(0);
}

