
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x50000000

#define RCCBASE 0x40021000

#define STK_CSR     0xE000E010
#define STK_RVR     0xE000E014
#define STK_CVR     0xE000E018
#define STK_MASK    0x00FFFFFF

#define USART1BASE 0x40013800

//PA09 UART1TX AF1
//PA10 UART1RX AF1


#include "twostuff.h"

volatile unsigned int hour;
volatile unsigned int minute;
volatile unsigned int second;
volatile unsigned int counter;
volatile unsigned int sec;
void systick_handler ( void )
{
    if(counter==0)
    {
        sec++; if(sec>59) sec=0;
        PUT32(GPIOABASE+0x00,0x28000000);
        PUT32(GPIOABASE+0x18,0x01FF0000);
        PUT32(GPIOABASE+0x00,twostuff[sec][0]);
        PUT32(GPIOABASE+0x18,twostuff[sec][1]);
        counter=1;
        //if(sec==second) counter=5;
        if(sec==minute) counter=10;
        if(sec==hour) counter=60;
    }
    counter--;
}


unsigned int uart_recv ( void )
{
    while(1) if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    return(GET32(USART1BASE+0x24));
}

const unsigned char mod3[16]={0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,0};

int notmain ( void )
{
    unsigned int ra;
    unsigned int newnum[6];


    ra=GET32(RCCBASE+0x34);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x34,ra);

    //moder
    PUT32(GPIOABASE+0x00,0x28000000);
    //OTYPER
    PUT32(GPIOABASE+0x04,0x00000000);
    //BSRR
    PUT32(GPIOABASE+0x18,0x01FF0000);




    //AFRH
    ra=GET32(GPIOABASE+0x24);
    //ra&=~(0xF<<(( 9-8)<<2)); //PA09
    //ra|= (0x1<<(( 9-8)<<2)); //PA09
    ra&=~(0xF<<((10-8)<<2)); //PA10 UART RX
    ra|= (0x1<<((10-8)<<2)); //PA10 UART RX
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x40);
    ra|=1<<14; //enable uart1
    PUT32(RCCBASE+0x40,ra);

    ra=GET32(RCCBASE+0x30);
    ra|=1<<14; //reset USART1
    PUT32(RCCBASE+0x30,ra);
    ra&=~(1<<14);
    PUT32(RCCBASE+0x30,ra);

    PUT32(USART1BASE+0x0C,1667); //9600
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<2)|1);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,1000-1);
    PUT32(STK_CVR,0x00000000);

    counter = 0;
    hour = 0;
    minute = 15;
    second = 17;
    sec = 0;

    PUT32(STK_CSR,7);

    while(1)
    {
        ra=uart_recv();
        if(ra!='X') continue;
        newnum[0]=uart_recv();
        newnum[1]=uart_recv();
        newnum[2]=uart_recv();
        newnum[3]=uart_recv();
        newnum[4]=uart_recv();
        newnum[6]=uart_recv();
        ra=uart_recv();
        if(ra!='Y') continue;
        newnum[0]=(newnum[0]&0xF);
        newnum[1]=(newnum[1]&0xF);
        newnum[2]=(newnum[2]&0xF);
        newnum[3]=(newnum[3]&0xF);
        newnum[4]=(newnum[3]&0xF);
        newnum[5]=(newnum[3]&0xF);

        if(newnum[0]==10) newnum[0]=0;

        hour  =(newnum[0]*10)+newnum[1];
        if(hour>11) hour=0;
        minute=(newnum[2]*10)+newnum[3];
        if(minute>59) minute=59;
        second=(newnum[4]*10)+newnum[5];
        if(second>59) second=58;

        hour=/*hour*5*/ (hour<<2)+(hour<<0);
        hour+=mod3[minute>>2];





    }

    return(0);
}
