
//-------------------------------------------------------------------
//-------------------------------------------------------------------

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000
#define USART1BASE 0x40013800

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

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
void systick_handler ( void )
{
    if(counter&1)
    {
        PUT32(GPIOABASE+0x18,(1<<(5+0))|(1<<(4+16)));
    }
    else
    {
        PUT32(GPIOABASE+0x18,(1<<(5+16))|(1<<(4+0)));
    }
    hexstring(counter++);
    hexstring(GET32(STK_CSR));
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

if(1)
{
    PUT32(STK_CSR,4);
    PUT32(STK_RVR,8000000-1);
    PUT32(STK_CVR,0);
    PUT32(STK_CSR,5);

    for(rx=0;rx<6;)
    {
        ra=GET32(STK_CSR);
        if(ra&(1<<16))
        {
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
    PUT32(STK_CSR,4);
    counter=0;
    PUT32(STK_RVR,4000000-1);
    PUT32(STK_CVR,0);
    PUT32(STK_CSR,7);

    while(1)
    {
        if(counter>7) break;
    }
    hexstring(GET32(STK_CSR));
    PUT32(STK_CSR,4);
}

    PUT32(GPIOABASE+0x18,(1<<(5+16))|(1<<(4+16)));
    hexstring(0x12345678);
    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------

