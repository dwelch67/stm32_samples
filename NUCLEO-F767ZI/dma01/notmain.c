
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );

#define RCC_BASE        0x40023800
#define RCC_CR          (RCC_BASE+0x00)
#define RCC_CFGR        (RCC_BASE+0x08)
#define RCC_APB1RSTR    (RCC_BASE+0x20)
#define RCC_AHB1ENR     (RCC_BASE+0x30)
#define RCC_APB1ENR     (RCC_BASE+0x40)

#define GPIOD_BASE      0x40020C00
#define GPIOD_MODER     (GPIOD_BASE+0x00)
#define GPIOD_AFRH      (GPIOD_BASE+0x24)

#define USART3_BASE     0x40004800
#define UART_BASE       USART3_BASE
#define UART_CR1        (UART_BASE+0x00)
#define UART_BRR        (UART_BASE+0x0C)
#define UART_ISR        (UART_BASE+0x1C)
//#define UART_RDR      (UART_BASE+0x24)
#define UART_TDR        (UART_BASE+0x28)

#define DMA2_BASE       0x40026400
#define DMA_BASE        DMA2_BASE
#define DMA_S0CR        (DMA_BASE+0x10)
#define DMA_S0NDTR      (DMA_BASE+0x14)
#define DMA_S0PAR       (DMA_BASE+0x18)
#define DMA_S0M0AR      (DMA_BASE+0x1C)
#define DMA_S0M1AR      (DMA_BASE+0x20)
#define DMA_S0FCR       (DMA_BASE+0x24)


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
    unsigned int rb;
    //unsigned int rc;
    //unsigned int rd;
    unsigned int cr;

    clock_init();
    uart_init();
    hexstring(0x12345678);

    for(ra=0x20001000;ra<0x20003000;ra+=4) PUT32(ra,ra);

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<22; //enable DMA2
    PUT32(RCC_AHB1ENR,ra);

    cr=0;
    cr|=0<<25; //CHSEL
    cr|=0<<23; //MBURST
    cr|=0<<21; //PBURST
    cr|=0<<19; //CT
    cr|=0<<18; //DBM
    cr|=0<<16; //PL
    cr|=0<<15; //PINCOS
    cr|=0<<13; //MSIZE <-- not used
    cr|=2<<11; //PSIZE <--- used for memory to memory
    cr|=1<<10; //MINC <-- used
    cr|=1<< 9; //PINC <-- used
    cr|=2<< 6; //DIR  <--- memory to memory
    cr|=0<< 5; //PFCTRL
    cr|=0<< 4; //TCIE
    cr|=0<< 3; //HTIE
    cr|=0<< 2; //TEIE
    cr|=0<< 1; //DMEIE
    cr|=0<< 0; //EN
    PUT32(DMA_S0CR,cr);

    PUT32(DMA_S0NDTR,16); //number of data items to be transferred
    PUT32(DMA_S0M0AR,0x20001000);
    PUT32(DMA_S0PAR, 0x20002000);
    //PUT32(DMA_S0M1AR,0x20002100);
    //PUT32(DMA_S0FCR,0x00000021); //reset value 0x00000021
    PUT32(DMA_S0CR,cr|1);
    ra=0xFFFFF;
    while(1)
    {
        rb=GET32(DMA_S0NDTR);
        if(rb!=ra)
        {
            ra=rb;
            hexstring(ra);
            if(ra==0) break;
        }
    }

    for(ra=0x20001000;ra<0x20003000;ra+=4)
    {
        rb=GET32(ra);
        if(rb!=ra)
        {
            hexstrings(ra); hexstring(rb);
        }
    }

    return(0);
}
