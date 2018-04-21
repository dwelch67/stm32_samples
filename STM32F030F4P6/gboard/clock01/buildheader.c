

#include <stdio.h>
#include <stdlib.h>

#define A 5
#define B 7
#define C 6
#define D 4

unsigned char raw [3][4][2]=
{
    {{A,B},{B,A},{C,A},{D,A}},
    {{A,C},{B,C},{C,B},{D,B}},
    {{A,D},{B,D},{C,D},{D,C}},
};

unsigned int stuff[4][4][2];

void figure ( unsigned int y, unsigned int x, unsigned int h, unsigned int l )
{
    unsigned int amoder;
    unsigned int absrr;

    amoder=0x28000000;
    //amoder|=2<<18; //NO UART TX!!!
    amoder|=2<<20;

    absrr=0;

    amoder|=(1<<(h<<1));
    absrr|=(1<<(h+ 0));
    amoder|=(1<<(l<<1));
    absrr|=(1<<(l+16));

    stuff[y][x][0]=amoder;
    stuff[y][x][1]=absrr;
}

int main ( void )
{
    unsigned int rx,ry,rz;

    for(ry=0;ry<3;ry++)
    {
        for(rx=0;rx<4;rx++)
        {
            figure(ry,rx,raw[2-ry][3-rx][1],raw[2-ry][3-rx][0]);
        }
    }
        for(rx=0;rx<4;rx++)
        {
            figure(ry,rx,raw[2][3][1],raw[2][3][0]);
        }

    printf("const unsigned int gbinary[4][4][2]=\n");
    printf("{\n");

    for(ry=0;ry<4;ry++)
    {
        printf("{");
        for(rx=0;rx<4;rx++)
        {
            printf("{0x%08X,0x%08X},",stuff[ry][rx][0],stuff[ry][rx][1]);
        }
        printf("},\n");
    }
    printf("};\n");
    return(0);
}

