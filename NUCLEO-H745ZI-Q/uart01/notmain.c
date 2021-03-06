
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
#define RCCBASE 0x58024400
#define RCC_APB4ENR (RCCBASE+0F4)
#define RCC_C1_APB4ENR (RCCBASE+154)
#define RCC_C2_APB4ENR (RCCBASE+1B4)
#define RCC_AHB4ENR (RCCBASE+0x0E0)
#define RCC_APB1LENR (RCCBASE+0x0E8)
#define RCC_APB1LRSTR (RCCBASE+0x090)

#define SYSCFG_BASE 0x58000400
#define CPUID 0xE000ED00

#define GPIOD_BASE 0x58020C00
#define GPIOD_MODER     (GPIOD_BASE+0x00)
#define GPIOD_OTYPER    (GPIOD_BASE+0x04)
#define GPIOD_OSPEEDR   (GPIOD_BASE+0x08)
#define GPIOD_PUPDR     (GPIOD_BASE+0x0C)
#define GPIOD_BSRR      (GPIOD_BASE+0x18)
#define GPIOD_AFRL      (GPIOD_BASE+0x20)
#define GPIOD_AFRH      (GPIOD_BASE+0x24)

#define USART3_BASE 0x40004800
#define UART_BASE     USART3_BASE
#define UART_CR1      (UART_BASE+0x00)
#define UART_BRR      (UART_BASE+0x0C)
#define UART_ISR      (UART_BASE+0x1C)
#define UART_RDR      (UART_BASE+0x24)
#define UART_TDR      (UART_BASE+0x28)

//PD8/9  AF7

//PB14

static void uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHB4ENR);
    ra|=1<<3; //GPIOD
    PUT32(RCC_AHB4ENR,ra);

    ra=GET32(RCC_APB1LENR);
    ra|=1<<18; //USART3EN
    PUT32(RCC_APB1LENR,ra);

    ra=GET32(GPIOD_MODER);
    ra&=~(3<<(8<<1)); //PD8
    ra&=~(3<<(9<<1)); //PD9
    ra|=  2<<(8<<1) ; //PD8
    ra|=  2<<(9<<1) ; //PD9
    PUT32(GPIOD_MODER,ra);

    ra=GET32(GPIOD_AFRH);
    ra&=~(0xF<<((8-8)<<2)); //PD8
    ra&=~(0xF<<((9-8)<<2)); //PD9
    ra|= (0x7<<((8-8)<<2)); //PD8
    ra|= (0x7<<((9-8)<<2)); //PD9
    PUT32(GPIOD_AFRH,ra);

    ra=GET32(RCC_APB1LRSTR);
    ra|= (1<<18); //reset USART3
    PUT32(RCC_APB1LRSTR,ra);
    ra&=~(1<<18);
    PUT32(RCC_APB1LRSTR,ra);

    //64000000/115200 = 347
    PUT32(UART_BRR,0x22B);
    PUT32(UART_CR1,(1<<3)|(1<<2)|(1<<0));
}

static void uart_send ( unsigned int x )
{
    while(1) if(GET32(UART_ISR)&(1<<7)) break;
    PUT32(UART_TDR,x);
}

//static unsigned int uart_recv ( void )
//{
    //while(1) if((GET32(UART_ISR))&(1<<5)) break;
    //return(GET32(UART_RDR));
//}

int notmain ( void )
{
    unsigned int ra;

    ra=GET32(CPUID)&0xFFF0;
    //comment one of these
    //if(ra==0xC270) return(0); //cortex-m7
    if(ra==0xC240) return(0); //cortex-m4

    uart_init();

    for(ra=0x30;;ra++)
    {
        ra&=0x37;
        uart_send(ra);
    }

    return(0);
}
