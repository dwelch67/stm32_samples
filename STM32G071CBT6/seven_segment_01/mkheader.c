
#include <stdio.h>


 //aaa
//f   b
//f   b
//f   b
 //ggg
//e   c
//e   c
//e   c
 //ddd

//10 9 8 7 6
//g  f X a b
//0  1   3 4


 //8 7   6 5
 //e d Z c .
 //1 2 3 4 5

//X common ground
//Z common source

char segs[10][16]=
{
{"abcdef"}, //0
{"bc"}, //1
{"abged"}, //2
{"abgcd"}, //3
{"fbgc"}, //4
{"afgcd"}, //5
{"afgcde"}, //6
{"abc"}, //7
{"abcdefg"}, //8
{"abcfg"}, //9
};

//76543210
//abcdefgp

int main ( void )
{
    unsigned int ra;
    unsigned int rb;

    unsigned int seven[16];

    for(rb=0;rb<16;rb++)
    {
        seven[rb]=0x00FF0000;
    }

    for(ra=0;ra<10;ra++)
    {
        for(rb=0;segs[ra][rb];rb++)
        {
            switch(segs[ra][rb])
            {
                case 'a': { seven[ra]^=0x00800080; break; }
                case 'b': { seven[ra]^=0x00400040; break; }
                case 'c': { seven[ra]^=0x00200020; break; }
                case 'd': { seven[ra]^=0x00100010; break; }
                case 'e': { seven[ra]^=0x00080008; break; }
                case 'f': { seven[ra]^=0x00040004; break; }
                case 'g': { seven[ra]^=0x00020002; break; }
            }
        }
    }
    for(rb=0;rb<16;rb++)
    {
        if(rb>10)
        {
            printf("{");
            printf("0x00FF0F00,");
            printf("0x00FF0F00,");
            printf("0x00FF0F00,");
            printf("0x00FF0F00,");
            printf("},\n");
        }
        else
        {
            printf("{");
            printf("0x%08X,",seven[rb]|0x08000700);
            printf("0x%08X,",seven[rb]|0x04000B00);
            printf("0x%08X,",seven[rb]|0x02000D00);
            printf("0x%08X,",seven[rb]|0x01000E00);
            printf("},\n");
        }
    }




    return(0);
}




