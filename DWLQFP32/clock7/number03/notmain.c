
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define GPIOBBASE 0x48000400
#define RCC_BASE 0x40021000

#define STK_CSR     0xE000E010
#define STK_RVR     0xE000E014
#define STK_CVR     0xE000E018
#define STK_MASK    0x00FFFFFF

#include "seven.h"

volatile unsigned int num[4];
volatile unsigned int counter;
void systick_handler ( void )
{
    unsigned int x;

    PUT32(GPIOBBASE+0x00,0x00000000);
    x=(counter++)&3;
    PUT32(GPIOABASE+0x18,seven[num[x]]);
    PUT32(GPIOBBASE+0x00,(1<<((x+4)<<1)));
    //GET32(STK_CSR);
}


void delay ( void )
{
    unsigned int ra;
    for(ra=0;ra<0x1000;ra++) dummy(ra);
}

int notmain ( void )
{
    unsigned int ra;


    ra=GET32(RCC_BASE+0x14);
    ra|=1<<17; //enable port A
    ra|=1<<18; //enable port B
    PUT32(RCC_BASE+0x14,ra);

    //moder
    PUT32(GPIOABASE+0x00,0x28005555);
    //OTYPER
    PUT32(GPIOABASE+0x04,0x00000000);
    //BSRR
    PUT32(GPIOABASE+0x18,0x00FF0000);

    //moder
    PUT32(GPIOBBASE+0x00,0x00000000);
    //OTYPER
    PUT32(GPIOBBASE+0x04,0x00000000);
    //BSRR
    PUT32(GPIOBBASE+0x18,0x00F00000);

    counter=0;
    num[0]=1;
    num[1]=2;
    num[2]=3;
    num[3]=4;

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,10000-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,7);

    while(1)
    {
        delay();
        num[3]++;
        if(num[3]>9) { num[3]=0; num[2]++; }
        if(num[2]>9) { num[2]=0; num[1]++; }
        if(num[1]>9) { num[1]=0; num[0]++; }
        if(num[0]>9) { num[0]=0;  }
    }

    return(0);
}

/*

reset halt
flash write_image erase number03/notmain.elf
reset

*/


