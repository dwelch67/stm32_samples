void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x50000000

#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF


#define UART2_BASE 0x40004400


static int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x34);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x34,ra);

    ra=GET32(RCCBASE+0x3C);
    ra|=1<<17; //enable USART2
    PUT32(RCCBASE+0x3C,ra);

    //PA2 UART1_TX
    //PA3 UART1_RX

    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    ra|=2<<(2<<1); //PA2
    ra|=2<<(3<<1); //PA3
    PUT32(GPIOABASE+0x00,ra);

    //AFRL
    ra=GET32(GPIOABASE+0x20);
    ra&=~(0xF<<8); //PA2
    ra&=~(0xF<<12); //PA3
    ra|=(0x1<<8); //PA2
    ra|=(0x1<<12); //PA3
    PUT32(GPIOABASE+0x20,ra);

    ra=GET32(RCCBASE+0x2C);
    ra|=1<<17; //reset USART2
    PUT32(RCCBASE+0x2C,ra);
    ra&=~(1<<17);
    PUT32(RCCBASE+0x2C,ra);

    PUT32(UART2_BASE+0x0C,138);
    PUT32(UART2_BASE+0x08,1<<12);
    PUT32(UART2_BASE+0x00,(1<<3)|(1<<2)|1);

    return(0);
}

static void uart_send ( unsigned int x )
{
    while(1) if(GET32(UART2_BASE+0x1C)&(1<<7)) break;
    PUT32(UART2_BASE+0x28,x);
}

static unsigned int uart_recv ( void )
{
    while(1) if((GET32(UART2_BASE+0x1C))&(1<<5)) break;
    return(GET32(UART2_BASE+0x24));
}

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
    unsigned int rx;

    uart_init();
    hexstring(0x12345678);

    while(1)
    {
        rx=uart_recv();
        if(rx==0x0D) uart_send(0x0A);
        uart_send(rx);
    }

    return(0);
}
