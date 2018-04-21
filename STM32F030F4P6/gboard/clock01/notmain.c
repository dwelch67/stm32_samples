
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000

#define RCCBASE     0x40021000
#define RCC_CR      (RCCBASE+0x00)
#define RCC_CFGR    (RCCBASE+0x04)
#define RCC_CFGR2   (RCCBASE+0x2C)

#define FLASH_BASE  0x40022000
#define FLASH_ACR   (FLASH_BASE+0x00)

#define STK_CSR     0xE000E010
#define STK_RVR     0xE000E014
#define STK_CVR     0xE000E018
#define STK_MASK    0x00FFFFFF

#define USART1BASE  0x40013800

#include "gbinary.h"

static unsigned char xstring[32];

volatile unsigned int hour;
volatile unsigned int minute;
volatile unsigned int second;

volatile unsigned int mtens;
volatile unsigned int mones;
volatile unsigned int counter;

volatile unsigned int x;
volatile unsigned int y;
volatile unsigned int z;

void systick_handler ( void )
{
    if(counter==0)
    {
        x++;
        if(x>3)
        {
            x=0;
            y++;
            if(y>2) y=0;
            switch(y)
            {
                case 0: z=hour;  break;
                case 1: z=mtens; break;
                case 2: z=mones; break;
                default: z=0xF; break;
            }
        }
        PUT32(GPIOABASE+0x00,0x28200000);
        PUT32(GPIOABASE+0x18,0x00FF0000);
        PUT32(GPIOABASE+0x00,gbinary[y][x][0]);
        PUT32(GPIOABASE+0x18,gbinary[y][x][1]);
        if(z&(1<<x))
        {
            counter=20;
        }
        else
        {
            counter=1;
        }
    }
    counter--;
}

static int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<14; //enable USART1
    PUT32(RCCBASE+0x18,ra);

    //PA9  UART1_TX NOT USING THIS, NEED IT FOR A GPIO
    //PA10 UART1_RX

    //moder 10
    ra=GET32(GPIOABASE+0x00);
    //ra&=~(3<<18); //PA9
    //ra|=2<<18; //PA9
    ra&=~(3<<20); //PA10
    ra|=2<<20; //PA10
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER 0
    ra=GET32(GPIOABASE+0x04);
    //ra&=~(1<<9); //PA9
    ra&=~(1<<10); //PA10
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr 11
    ra=GET32(GPIOABASE+0x08);
    //ra|=3<<18; //PA9
    ra|=3<<20; //PA10
    PUT32(GPIOABASE+0x08,ra);
    //pupdr 00
    ra=GET32(GPIOABASE+0x0C);
    //ra&=~(3<<18); //PA9
    ra&=~(3<<20); //PA10
    PUT32(GPIOABASE+0x0C,ra);
    //afr 0001
    ra=GET32(GPIOABASE+0x24);
    //ra&=~(0xF<<4); //PA9
    //ra|=0x1<<4; //PA9
    ra&=~(0xF<<8); //PA10
    ra|=0x1<<8; //PA10
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x0C);
    ra|=1<<14; //reset USART1
    PUT32(RCCBASE+0x0C,ra);
    ra&=~(1<<14);
    PUT32(RCCBASE+0x0C,ra);

    //PUT32(USART1BASE+0x0C,69); //115200
    //PUT32(USART1BASE+0x0C,1667); //4800
    PUT32(USART1BASE+0x0C,833); //9600

    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(0<<3)|(1<<2)|1); //not TE

    return(0);
}

static unsigned int uart_recv ( void )
{
    while(1) if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    return(GET32(USART1BASE+0x24));
}

//$GPRMC,000143.00,A,4030.97866,N,07955.13947,W,0.419,,020416,,,A*6E

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
static unsigned int timezone;
//------------------------------------------------------------------------
static int do_nmea ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int state;
    unsigned int off;
    unsigned int sum;
    unsigned int savesum;
    unsigned int xsum;

    state=0;
    off=0;
//$GPRMC,054033.00,V,
    sum=0;
    savesum=0;
    xsum=0;
    while(1)
    {
        ra=uart_recv();
        //uart_send(ra);
        //uart_send(0x30+state);
        //PUT32(USART1BASE+0x28,ra);

        if(ra!='*') sum^=ra;

        switch(state)
        {
            case 0:
            {
                if(ra=='$') state++;
                else state=0;
                sum=0;
                break;
            }
            case 1:
            {
                if(ra=='G') state++;
                else state=0;
                break;
            }
            case 2:
            {
                if(ra=='P') state++;
                else state=0;
                break;
            }
            case 3:
            {
                if(ra=='R') state++;
                else state=0;
                break;
            }
            case 4:
            {
                if(ra=='M') state++;
                else state=0;
                break;
            }
            case 5:
            {
                if(ra=='C') state++;
                else state=0;
                break;
            }
            case 6:
            {
                off=0;
                if(ra==',') state++;
                else state=0;
                break;
            }
            case 7:
            {
                if(ra==',')
                {
                    if(off>7)
                    {
                        //xstring[0] //tens of hours
                        //xstring[1] //ones of hours
                        //xstring[2] //tens of minutes
                        //xstring[3] //ones of minutes
                        //xstring[4] //tens of seconds
                        //xstring[5] //ones of seconds

                        rb=xstring[0]&0xF;
                        rc=xstring[1]&0xF;
                        //1010
                        rb=/*rb*10*/(rb<<3)+(rb<<1); //times 10
                        rb+=rc;
                        if(rb>12) rb-=12;
                        //ra=5; //time zone adjustment winter
                        //ra=4; //time zone adjustment summer
                        ra=timezone;
                        if(rb<=ra) rb+=12;
                        rb-=ra;
                        if(rb>9)
                        {
                            xstring[0]='1';
                            rb-=10;
                        }
                        else
                        {
                            xstring[0]='0';
                        }
                        rb&=0xF;
                        xstring[1]=0x30+rb;
                        rb=0;
                        //hour  =((xstring[0]&0xF)*10)+(xstring[1]&0xF);
                        //minute=((xstring[2]&0xF)*10)+(xstring[3]&0xF);
                        //second=((xstring[4]&0xF)*10)+(xstring[5]&0xF);
                        //hexstrings(hour);
                        //hexstrings(minute);
                        //hexstring(second);
                    }
                    off=0;
                    state++;
                }
                else
                {
                    if(off<16)
                    {
                        xstring[off++]=ra;
                    }
                }
                break;
            }
            //could check for A vs V here...
            case 8:
            {
                if(ra=='*')
                {
                    savesum=sum;
                    state++;
                }
                break;
            }
            case 9:
            {
                if(ra>0x39) ra-=0x37;
                xsum=(ra&0xF)<<4;
                state++;
                break;
            }
            case 10:
            {
                if(ra>0x39) ra-=0x37;
                xsum|=(ra&0xF);
                if(savesum==xsum)
                {
                    hour  =((xstring[0]&0xF)*10)+(xstring[1]&0xF);
                    //if(hour>12) hour=12;
                    minute=((xstring[2]&0xF)*10)+(xstring[3]&0xF);
                    //if(minute>59) minute=59;
                    second=((xstring[4]&0xF)*10)+(xstring[5]&0xF);
                    //if(second>59) second=59;

                    mtens = xstring[2]&0xF;
                    mones = xstring[3]&0xF;
                }
                for(rb=0;rb<6;rb++) xstring[rb]=0;
                state=0;
                break;
            }
        }
    }
    return(0);
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    timezone=4;
    rb=0x44444444;
    ra=GET32(0x20000D00);
    if(ra==0x44444444)
    {
        timezone=5;
        rb=0x55555555;
    }
    else
    if(ra==0x55555555)
    {
        timezone=4;
        rb=0x44444444;
    }
    PUT32(0x20000D00,rb);

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    //moder
    PUT32(GPIOABASE+0x00,0x28000000);
    //OTYPER
    PUT32(GPIOABASE+0x04,0x00000000);
    //BSRR
    PUT32(GPIOABASE+0x18,0x00FF0000);

    uart_init();

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,2000-1);
    PUT32(STK_CVR,0x00000000);

    counter = 20;
    hour = 0;
    minute = 0;
    second = 0;

    x = 0;
    y = 0;
    z = 5;

    mtens = 5;
    mones = 6;
    for(ra=0;ra<6;ra++) xstring[ra]=0;

    PUT32(STK_CSR,7);

    do_nmea();

    return(0);
}

/*

halt
flash write_image erase gboard/clock01/notmain.elf
reset

*/


