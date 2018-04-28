
#include <stdio.h>


char raw[10][8]=
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

int main ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    printf("const unsigned int seven[10]=\n");
    printf("{\n");
    for(ra=0;ra<10;ra++)
    {
        rc=0x01FB0000;
        for(rb=0;raw[ra][rb];rb++)
        {
            switch(raw[ra][rb])
            {
                case 'a': rc|=1<<3; rc&=(~(1<<(3+16))); break;
                case 'b': rc|=1<<4; rc&=(~(1<<(4+16))); break;
                case 'c': rc|=1<<6; rc&=(~(1<<(6+16))); break;
                case 'd': rc|=1<<7; rc&=(~(1<<(7+16))); break;
                case 'e': rc|=1<<8; rc&=(~(1<<(8+16))); break;
                case 'f': rc|=1<<1; rc&=(~(1<<(1+16))); break;
                case 'g': rc|=1<<0; rc&=(~(1<<(0+16))); break;
            }
        }
            printf("0x%08X,\n",rc);
    }
    printf("};\n");
    return(0);
}


