
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
void ienable ( void );
void DOWFI ( void );

#define GPIOA_BASE       0x48000000
#define GPIOA_MODER     (GPIOA_BASE+0x00)
#define GPIOA_OTYPER    (GPIOA_BASE+0x04)
#define GPIOA_BSRR      (GPIOA_BASE+0x18)
#define GPIOA_AFRL      (GPIOA_BASE+0x20)
#define GPIOA_AFRH      (GPIOA_BASE+0x24)

#define RCC_BASE        0x40021000
#define RCC_APB2RSTR    (RCC_BASE+0x0C)
#define RCC_AHBENR      (RCC_BASE+0x14)
#define RCC_APB2ENR     (RCC_BASE+0x18)

#define USART1_BASE     0x40013800
#define USART1_CR1      (USART1_BASE+0x00)
#define USART1_CR2      (USART1_BASE+0x04)
#define USART1_CR3      (USART1_BASE+0x08)
#define USART1_BRR      (USART1_BASE+0x0C)
#define USART_ISR       (USART1_BASE+0x1C)

#define USART_RDR       (USART1_BASE+0x24)
#define USART_TDR       (USART1_BASE+0x28)

static int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //enable port a
    PUT32(RCC_AHBENR,ra);

    ra=GET32(RCC_APB2ENR);
    ra|=1<<14; //enable USART1
    PUT32(RCC_APB2ENR,ra);

    ra=GET32(GPIOA_MODER);
    ra&=~(3<<18); //PA9
    ra|=2<<18; //PA9
    ra&=~(3<<20); //PA10
    ra|=2<<20; //PA10
    PUT32(GPIOA_MODER,ra);

    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<9); //PA9
    ra&=~(1<<10); //PA10
    PUT32(GPIOA_OTYPER,ra);
    //afr 0001
    ra=GET32(GPIOA_AFRH);
    ra&=~(0xF<<4); //PA9
    ra|=0x1<<4; //PA9
    ra&=~(0xF<<8); //PA10
    ra|=0x1<<8; //PA10
    PUT32(GPIOA_AFRH,ra);

    ra=GET32(RCC_APB2RSTR);
    ra|=1<<14; //reset USART1
    PUT32(RCC_APB2RSTR,ra);
    ra&=~(1<<14);
    PUT32(RCC_APB2RSTR,ra);

    PUT32(USART1_BRR,69); //8000000/115200
    PUT32(USART1_CR3,1<<12);
    PUT32(USART1_CR1,(1<<3)|(1<<2)|1);

    return(0);
}

static void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART_ISR)&(1<<7)) break;
    PUT32(USART_TDR,x);
}

static unsigned int uart_recv ( void )
{
    while(1)
    {
        if((GET32(USART_ISR))&(1<<5)) break;
    }
    return(GET32(USART_RDR));
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
    uart_init();
    hexstring(0x12345678);
    while(1)
    {
        hexstring(uart_recv());
    }
    return(0);
}
