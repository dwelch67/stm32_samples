
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define DEBUGUART

#define GPIOABASE 0x48000000
#define GPIOBBASE 0x48000400

#define RCCBASE 0x40021000

#define USART1BASE  0x40013800


int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<14; //enable USART1
    PUT32(RCCBASE+0x18,ra);

#ifdef DEBUGUART
    //PA2 UART1_TX
    //PA15 UART1_RX
    //moder 10
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<4); //PA2
    ra|=2<<4; //PA2
    ra&=~(3<<30); //PA15
    ra|=2<<30; //PA15
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER 0
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<2); //PA2
    ra&=~(1<<15); //PA2
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr 11
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<4; //PA2
    ra|=3<<30; //PA15
    PUT32(GPIOABASE+0x08,ra);
    //pupdr 00
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<4); //PA2
    ra&=~(3<<30); //PA15
    PUT32(GPIOABASE+0x0C,ra);
    //afr 0001
    ra=GET32(GPIOABASE+0x20);
    ra&=~(0xF<<8); //PA2
    ra|=0x1<<8; //PA2
    PUT32(GPIOABASE+0x20,ra);
    //afr 0001
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<28); //PA15
    ra|=0x1<<28; //PA15
    PUT32(GPIOABASE+0x24,ra);

#else
    //PA9 UART1_TX
    //PA10 UART1_RX

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

#endif

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

static void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
    PUT32(USART1BASE+0x28,x);
}

//unsigned int uart_recv ( void )
//{
    //while(1) if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    //return(GET32(USART1BASE+0x24));
//}

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

static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}


int notmain ( void )
{
    unsigned int ra;

    uart_init();
    hexstring(0x12345678);

    ra=GET32(RCCBASE+0x14);
    ra|=1<<18; //enable port b
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOBBASE+0x00);
    ra&=~(3<<6); //PB3
    ra|=1<<6; //PB3
    PUT32(GPIOBBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOBBASE+0x04);
    ra&=~(1<<3); //PB3
    PUT32(GPIOBBASE+0x04,ra);
//    PUT32(GPIOBBASE+0x18,(1<<3));

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<0); //PA0
    //ra|=0<<6; //PA0
    PUT32(GPIOABASE+0x00,ra);

    while(1)
    {
        if(GET32(GPIOABASE+0x10)&1)
        {
            PUT32(GPIOBBASE+0x18,(1<<3));
        }
        else
        {
            PUT32(GPIOBBASE+0x18,(1<<(3+16)));
        }
    }

    return(0);
}
