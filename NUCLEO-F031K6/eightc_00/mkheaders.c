
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raw.h"

const unsigned char hilo[20][2]=
{
{0,4},//A
{0,3},//B
{0,2},//C
{0,1},//D
{3,1},//E
{1,3},//F
{3,4},//G
{1,4},//H
{4,0},//I
{4,3},//J
{4,2},//K
{4,1},//L
{3,0},//M
{1,0},//N
{3,2},//O
{1,2},//P
{2,0},//Q
{2,4},//R
{2,3},//S
{2,1},//T
};

const unsigned char hilopins[5]=
{
0,
1,
2,
3,
4,
};

unsigned char one[10][21];
unsigned int mask[10];

unsigned int xtable[10][20][2];


FILE *fp;

int main ( void )
{
    unsigned int ra,rb,rc;
    unsigned int rd;
    unsigned int hi,lo;

    memset(one,0,sizeof(one));

    for(ra=0;ra<10;ra++)
    {
        rd=0;
        for(rb=0;rb<7;rb++)
        {
            for(rc=0;rc<4;rc++)
            {
                if(raw[ra][rb][rc]=='.')
                {
                }
                else
                {
                    one[ra][++rd]=raw[ra][rb][rc];
                }
            }
        }
        one[ra][0]=rd;
    }

    for(ra=0;ra<10;ra++)
    {
        printf("%2u: ",ra);
        printf("%2u ",one[ra][0]);
        for(rb=0;rb<one[ra][0];rb++)
        {
            printf("%c",one[ra][rb+1]);
        }
        printf(" ");
        for(;rb<20;rb++)
        {
            printf("%X",one[ra][rb+1]);
        }
        printf("\n");
    }
    printf("\n");


    for(ra=0;ra<10;ra++)
    {
        mask[ra]=0;
        for(rb=0;rb<one[ra][0];rb++)
        {
            //printf("%c",one[ra][rb+1]);
            mask[ra]|=1<<(one[ra][rb+1]-'A');

        }
        printf("%2u 0x%08X\n",ra,mask[ra]);
    }
    printf("\n");

    for(ra=0;ra<10;ra++)
    {
        for(rb=0;rb<20;rb++)
        {
            if(mask[ra]&(1<<rb))
            {
                hi=hilopins[hilo[rb][0]];
                lo=hilopins[hilo[rb][1]];
                xtable[ra][rb][0]=(1<<(hi<<1))|(1<<(lo<<1));
                xtable[ra][rb][1]=(1<<(hi+0))|(1<<(lo+16));
            }
            else
            {
                xtable[ra][rb][0]=0;
                xtable[ra][rb][1]=0;
            }
        }
    }

    fp=fopen("xtable.h","wt");
    if(fp==NULL) return(1);
    fprintf(fp,"const unsigned int xtable[10][20][2]=\n");
    fprintf(fp,"{\n");
    for(ra=0;ra<10;ra++)
    {
        fprintf(fp,"  { //%u\n",ra);
        for(rb=0;rb<20;rb++)
        {
            fprintf(fp,"    {0x%08X,0x%08X},\n",xtable[ra][rb][0],xtable[ra][rb][1]);
        }
        fprintf(fp,"  },\n");
    }
    fprintf(fp,"};\n");
    fclose(fp);

    return(0);
}
