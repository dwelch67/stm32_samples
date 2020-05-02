
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );

#define RCC_BASE        0x40023800
#define RCC_CR          (RCC_BASE+0x00)
#define RCC_CFGR        (RCC_BASE+0x08)
#define RCC_APB1RSTR    (RCC_BASE+0x20)
#define RCC_AHB1ENR     (RCC_BASE+0x30)
#define RCC_APB1ENR     (RCC_BASE+0x40)

#define GPIODBASE       0x40020C00
#define GPIOD_MODER     (GPIODBASE+0x00)
#define GPIOD_AFRH      (GPIODBASE+0x24)

#define USART3_BASE     0x40004800
#define UART_BASE       USART3_BASE
#define UART_CR1      (UART_BASE+0x00)
#define UART_BRR      (UART_BASE+0x0C)
#define UART_ISR      (UART_BASE+0x1C)
#define UART_RDR      (UART_BASE+0x24)
#define UART_TDR      (UART_BASE+0x28)

//PD8 USART3 TX alternate function 7
//PD9 USART3 RX alternate function 7

static int clock_init ( void )
{
    unsigned int ra;

    //switch to external 8MHz clock.
    ra=GET32(RCC_CR);
    ra|=1<<16;
    PUT32(RCC_CR,ra);
    while(1) if(GET32(RCC_CR)&(1<<17)) break;
    ra=GET32(RCC_CFGR);
    ra&=~3;
    ra|=1;
    PUT32(RCC_CFGR,ra);
    while(1) if(((GET32(RCC_CFGR)>>2)&3)==1) break;

    return(0);
}

static int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<3; //enable port D
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<18; //enable UART_
    PUT32(RCC_APB1ENR,ra);

    ra=GET32(GPIOD_MODER);
    ra&=~(3<<(8<<1)); //PD8
    ra&=~(3<<(9<<1)); //PD9
    ra|= (2<<(8<<1)); //PD8
    ra|= (2<<(9<<1)); //PD9
    PUT32(GPIOD_MODER,ra);

    ra=GET32(GPIOD_AFRH);
    ra&=~(0xF<<((8-8)<<2)); //PD8
    ra&=~(0xF<<((9-8)<<2)); //PD9
    ra|= (0x7<<((8-8)<<2)); //PD8
    ra|= (0x7<<((9-8)<<2)); //PD9
    PUT32(GPIOD_AFRH,ra);

    ra=GET32(RCC_APB1RSTR);
    ra|=1<<18; //reset UART_
    PUT32(RCC_APB1RSTR,ra);
    ra&=~(1<<18);
    PUT32(RCC_APB1RSTR,ra);

    //8000000/115200) = 69.444
    PUT32(UART_BRR,69);
    PUT32(UART_CR1,(1<<3)|(1<<2)|(1<<0));

    return(0);
}

static void uart_send ( unsigned int x )
{
    while(1) if(GET32(UART_ISR)&(1<<7)) break;
    PUT32(UART_TDR,x);
}

static unsigned int uart_recv ( void )
{
    while(1) if((GET32(UART_ISR))&(1<<5)) break;
    return(GET32(UART_RDR));
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

    clock_init();
    uart_init();
    hexstring(0x12345678);
    while(1)
    {
        rx=uart_recv();
        hexstring(rx);
    }
    return(0);
}


