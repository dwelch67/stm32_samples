
//-------------------------------------------------------------------
//-------------------------------------------------------------------

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000
#define USART1BASE 0x40013800

#define TIMER14BASE 0x40002000

#define NVIC_ISER 0xE000E100
#define NVIC_ICER 0xE000E180
#define NVIC_ISPR 0xE000E200
#define NVIC_ICPR 0xE000E280
#define NVIC_IPR0 0xE000E400
#define NVIC_IPR1 0xE000E404
#define NVIC_IPR2 0xE000E408
#define NVIC_IPR3 0xE000E40C
#define NVIC_IPR4 0xE000E410
#define NVIC_IPR5 0xE000E414
#define NVIC_IPR6 0xE000E418
#define NVIC_IPR7 0xE000E41C

#define SCB_ICSR 0xE000ED04

//-------------------------------------------------------------------
static void led_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<8); //PA4
    ra&=~(3<<10); //PA5
    ra|=1<<8; //PA4
    ra|=1<<10; //PA5
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<4); //PA4
    ra&=~(1<<5); //PA5
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<8; //PA4
    ra|=3<<10; //PA5
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<8); //PA4
    ra&=~(3<<10); //PA5
    PUT32(GPIOABASE+0x0C,ra);
}
//-------------------------------------------------------------------
static int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<14; //enable USART1
    PUT32(RCCBASE+0x18,ra);

    //moder 10
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<18); //PA9
    ra|=2<<18; //PA9
    ra&=~(3<<20); //PA10
    ra|=2<<20; //PA10
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER 0
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<9); //PA9
    ra&=~(1<<10); //PA10
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr 11
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<18; //PA9
    ra|=3<<20; //PA10
    PUT32(GPIOABASE+0x08,ra);
    //pupdr 00
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<18); //PA9
    ra&=~(3<<20); //PA10
    PUT32(GPIOABASE+0x0C,ra);
    //afr 0001
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<4); //PA9
    ra|=0x1<<4; //PA9
    ra&=~(0xF<<8); //PA10
    ra|=0x1<<8; //PA10
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x0C);
    ra|=1<<14; //reset USART1
    PUT32(RCCBASE+0x0C,ra);
    ra&=~(1<<14);
    PUT32(RCCBASE+0x0C,ra);

    PUT32(USART1BASE+0x0C,69);
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|(1<<2)|1);

    return(0);
}
//-------------------------------------------------------------------
static void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
    PUT32(USART1BASE+0x28,x);
}
//-------------------------------------------------------------------
//static unsigned int uart_recv ( void )
//{
    //while(1) if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    //return(GET32(USART1BASE+0x24));
//}
//-------------------------------------------------------------------
static void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}
//-------------------------------------------------------------------
static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//  **** WARNING THE CODE BELOW IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------
static volatile unsigned int counter;
void tim14_handler ( void )
{
    if(counter&1)
    {
        PUT32(GPIOABASE+0x18,(1<<(5+0))|(1<<(4+16)));
    }
    else
    {
        PUT32(GPIOABASE+0x18,(1<<(5+16))|(1<<(4+0)));
    }
    hexstrings(GET32(SCB_ICSR));
    hexstring(counter++);
    PUT16(TIMER14BASE+0x10,0);
}
//-------------------------------------------------------------------
//  **** WARNING THE CODE ABOVE IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------

//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    uart_init();
    hexstring(0x12345678);
    led_init();
    PUT32(GPIOABASE+0x18,(1<<(5+0))|(1<<(4+16)));


    ra=GET32(RCCBASE+0x1C);
    ra|=1<<8; //enable timer 14
    PUT32(RCCBASE+0x1C,ra);


if(1)
{
    PUT16(TIMER14BASE+0x00,0x0000);
    PUT16(TIMER14BASE+0x2C,0x7A11);
    PUT16(TIMER14BASE+0x24,0x0000);
    PUT16(TIMER14BASE+0x28,0x00FF);
    PUT16(TIMER14BASE+0x00,0x0001);

    for(rx=0;rx<6;)
    {
        ra=GET16(TIMER14BASE+0x10);
        if(ra&(1<<1))
        {
            PUT16(TIMER14BASE+0x10,0);
            if(rx&1)
            {
                PUT32(GPIOABASE+0x18,(1<<(5+0))|(1<<(4+16)));
            }
            else
            {
                PUT32(GPIOABASE+0x18,(1<<(5+16))|(1<<(4+0)));
            }
            hexstring(rx);
            rx++;
        }
    }
}

if(0)
{
    PUT16(TIMER14BASE+0x00,0x0000);
    PUT16(TIMER14BASE+0x2C,0x7A11);
    PUT16(TIMER14BASE+0x24,0x0000);
    PUT16(TIMER14BASE+0x28,0x00FF);
    PUT16(TIMER14BASE+0x0C,0x0002);
    PUT16(TIMER14BASE+0x00,0x0001);

    for(rx=0;rx<10;rx++)
    {
        hexstrings(GET32(SCB_ICSR));
        hexstrings(GET32(NVIC_ICPR));
        hexstring(GET16(TIMER14BASE+0x10));
    }
    PUT16(TIMER14BASE+0x00,0x0000);
    PUT16(TIMER14BASE+0x10,0);
    hexstrings(GET32(SCB_ICSR));
    hexstrings(GET32(NVIC_ICPR));
    hexstring(GET16(TIMER14BASE+0x10));
    PUT32(NVIC_ICPR,1<<19);
    hexstrings(GET32(SCB_ICSR));
    hexstrings(GET32(NVIC_ICPR));
    hexstring(GET16(TIMER14BASE+0x10));
//12345678
//00000000 00000000 00000000
//00000000 00000000 00000003
//00400000 00080000 00000003
//00400000 00080000 00000003
//00400000 00080000 00000003
//00400000 00080000 00000003
//00400000 00080000 00000003
//00400000 00080000 00000003
//00400000 00080000 00000003
//00400000 00080000 00000003
//00400000 00080000 00000000
//00000000 00000000 00000000
//12345678
}

if(1)
{
    PUT16(TIMER14BASE+0x00,0x0000);
    PUT16(TIMER14BASE+0x2C,0x7A11);
    PUT16(TIMER14BASE+0x24,0x0000);
    PUT16(TIMER14BASE+0x28,0x007F);
    PUT16(TIMER14BASE+0x0C,0x0002);
    PUT16(TIMER14BASE+0x00,0x0001);


    for(rx=0;rx<6;)
    {
        ra=GET32(SCB_ICSR);
        if(ra&0x00400000)
        {
            PUT16(TIMER14BASE+0x10,0);
            PUT32(NVIC_ICPR,1<<19);
            if(rx&1)
            {
                PUT32(GPIOABASE+0x18,(1<<(5+0))|(1<<(4+16)));
            }
            else
            {
                PUT32(GPIOABASE+0x18,(1<<(5+16))|(1<<(4+0)));
            }
            hexstring(rx);
            rx++;
        }
    }
}


if(1)
{
    PUT16(TIMER14BASE+0x00,0x0000);
    PUT16(TIMER14BASE+0x2C,0x7A11);
    PUT16(TIMER14BASE+0x24,0x0000);
    PUT16(TIMER14BASE+0x28,0x00FF);
    PUT16(TIMER14BASE+0x0C,0x0002);

    counter=0;

    PUT32(NVIC_ISER,1<<19);

    //19 18 17 16 IPR4
    //15 14 13 12 IPR3
    //11 10  9  8 IPR2
    // 7  6  5  4 IPR1
    // 3  2  1  0 IPR0
    //PUT32(NVIC_IPR4,1<<24);

    PUT16(TIMER14BASE+0x00,0x0001);

    while(counter<8) continue;
    PUT16(TIMER14BASE+0x00,0x0000);
}




    PUT32(GPIOABASE+0x18,(1<<(5+16))|(1<<(4+16)));
    hexstring(0x12345678);
    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------

