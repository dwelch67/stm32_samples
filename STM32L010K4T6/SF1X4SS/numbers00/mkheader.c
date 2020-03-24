
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

    unsigned int seven[16];
    //all leds off
    for(rb=0;rb<16;rb++)
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
    //make the table
    for(rb=0;rb<16;rb++)
    {
        // above 9 all off
        if(rb>9)
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
        }
        else
        //the bsrr to make each digit on for each ground/position
        //seven[5][0] makes the first digit a 5
        //seven[5][1] makes the second digit a 5
        //seven[5][2] makes the third digit a 5
        //seven[5][3] makes the fourth digit a 5
        {
            //ONE GROUND AT A TIME
            printf("{");
            printf("0x%08X,",seven[rb]|(1<<(GND0+16))|(1<<(GND1+ 0))|(1<<(GND2+ 0))|(1<<(GND3+ 0)));
            printf("0x%08X,",seven[rb]|(1<<(GND0+ 0))|(1<<(GND1+16))|(1<<(GND2+ 0))|(1<<(GND3+ 0)));
            printf("0x%08X,",seven[rb]|(1<<(GND0+ 0))|(1<<(GND1+ 0))|(1<<(GND2+16))|(1<<(GND3+ 0)));
            printf("0x%08X,",seven[rb]|(1<<(GND0+ 0))|(1<<(GND1+ 0))|(1<<(GND2+ 0))|(1<<(GND3+16)));
            printf("},//%u\n",rb);
        }
    }
    return(0);
}




