
#undef INVERTED
//#define INVERTED

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

#define GND0  0
#define LEDA  1
#define LEDF  2
#define GND1  3
#define GND2  4
#define LEDB  5
#define GND3  6
#define LEDG  7
#define LEDC  8
#define LEDP 11
#define LEDD 12
#define LEDE 15

int main ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    unsigned int seven[32];
    //all leds off
    for(rb=0;rb<32;rb++)
    {
        seven[rb]=
            (1<<(LEDA+16))|
            (1<<(LEDB+16))|
            (1<<(LEDC+16))|
            (1<<(LEDD+16))|
            (1<<(LEDE+16))|
            (1<<(LEDF+16))|
            (1<<(LEDG+16))|
            (1<<(LEDP+16));
    }
#ifdef INVERTED
    for(ra=0;ra<10;ra++)
    {
        for(rb=0;segs[ra][rb];rb++)
        {
            switch(segs[ra][rb])
            {
                case 'a': { segs[ra][rb]='d'; break; }
                case 'b': { segs[ra][rb]='e'; break; }
                case 'c': { segs[ra][rb]='f'; break; }
                case 'd': { segs[ra][rb]='a'; break; }
                case 'e': { segs[ra][rb]='b'; break; }
                case 'f': { segs[ra][rb]='c'; break; }
                case 'g': { segs[ra][rb]='g'; break; }
            }
        }
    }
#endif

    //toggle the ones on that need to be on for numbers 0 to 9
    for(ra=0;ra<10;ra++)
    {
        for(rb=0;segs[ra][rb];rb++)
        {
            switch(segs[ra][rb])
            {
                case 'a': { seven[ra]^=(1<<(LEDA+16))|(1<<(LEDA+ 0)); break; }
                case 'b': { seven[ra]^=(1<<(LEDB+16))|(1<<(LEDB+ 0)); break; }
                case 'c': { seven[ra]^=(1<<(LEDC+16))|(1<<(LEDC+ 0)); break; }
                case 'd': { seven[ra]^=(1<<(LEDD+16))|(1<<(LEDD+ 0)); break; }
                case 'e': { seven[ra]^=(1<<(LEDE+16))|(1<<(LEDE+ 0)); break; }
                case 'f': { seven[ra]^=(1<<(LEDF+16))|(1<<(LEDF+ 0)); break; }
                case 'g': { seven[ra]^=(1<<(LEDG+16))|(1<<(LEDG+ 0)); break; }
            }
        }
    }
    //add the decimal point
    for(ra=0;ra<10;ra++)
    {
        seven[ra+16]=seven[ra];
        seven[ra+16]^=(1<<(LEDP+16))|(1<<(LEDP+ 0));
    }
    //make the table
    for(rb=0;rb<32;rb++)
    {
        switch(rb)
        {
            case 0: case 16:
            case 1: case 17:
            case 2: case 18:
            case 3: case 19:
            case 4: case 20:
            case 5: case 21:
            case 6: case 22:
            case 7: case 23:
            case 8: case 24:
            case 9: case 25:
            {
                //the bsrr to make each digit on for each ground/position
                //seven[5][0] makes the first digit a 5
                //seven[5][1] makes the second digit a 5
                //seven[5][2] makes the third digit a 5
                //seven[5][3] makes the fourth digit a 5
                {
                    //ONE GROUND AT A TIME
                    printf("{");
#ifdef INVERTED
                    printf("0x%08X,",seven[rb]|(1<<(GND0+ 0))|(1<<(GND1+ 0))|(1<<(GND2+ 0))|(1<<(GND3+16)));
                    printf("0x%08X,",seven[rb]|(1<<(GND0+ 0))|(1<<(GND1+ 0))|(1<<(GND2+16))|(1<<(GND3+ 0)));
                    printf("0x%08X,",seven[rb]|(1<<(GND0+ 0))|(1<<(GND1+16))|(1<<(GND2+ 0))|(1<<(GND3+ 0)));
                    printf("0x%08X,",seven[rb]|(1<<(GND0+16))|(1<<(GND1+ 0))|(1<<(GND2+ 0))|(1<<(GND3+ 0)));
#else
                    printf("0x%08X,",seven[rb]|(1<<(GND0+16))|(1<<(GND1+ 0))|(1<<(GND2+ 0))|(1<<(GND3+ 0)));
                    printf("0x%08X,",seven[rb]|(1<<(GND0+ 0))|(1<<(GND1+16))|(1<<(GND2+ 0))|(1<<(GND3+ 0)));
                    printf("0x%08X,",seven[rb]|(1<<(GND0+ 0))|(1<<(GND1+ 0))|(1<<(GND2+16))|(1<<(GND3+ 0)));
                    printf("0x%08X,",seven[rb]|(1<<(GND0+ 0))|(1<<(GND1+ 0))|(1<<(GND2+ 0))|(1<<(GND3+16)));
#endif
                    printf("},//%u\n",rb);
                }
                break;
            }
            default:
            {
                //GNDS HIGH LEDS LOW
                rc=0;
                rc|=(1<<(LEDA+16));
                rc|=(1<<(LEDB+16));
                rc|=(1<<(LEDC+16));
                rc|=(1<<(LEDD+16));
                rc|=(1<<(LEDE+16));
                rc|=(1<<(LEDF+16));
                rc|=(1<<(LEDG+16));
                rc|=(1<<(LEDP+16));
                rc|=(1<<(GND0+0));
                rc|=(1<<(GND1+0));
                rc|=(1<<(GND2+0));
                rc|=(1<<(GND3+0));
                printf("{");
                printf("0x%08X,",rc);
                printf("0x%08X,",rc);
                printf("0x%08X,",rc);
                printf("0x%08X,",rc);
                printf("},//%u\n",rb);
                break;
            }
        }

    }
    return(0);
}




