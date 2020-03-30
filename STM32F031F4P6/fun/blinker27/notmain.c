
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOA_BASE       0x48000000
#define GPIOA_MODER     (GPIOA_BASE+0x00)
#define GPIOA_OTYPER    (GPIOA_BASE+0x04)
#define GPIOA_BSRR      (GPIOA_BASE+0x18)
#define GPIOA_AFRL      (GPIOA_BASE+0x20)
#define GPIOA_AFRH      (GPIOA_BASE+0x24)

#define RCC_BASE        0x40021000
#define RCC_APB2RSTR    (RCC_BASE+0x0C)
#define RCC_APB1RSTR    (RCC_BASE+0x10)
#define RCC_AHBENR      (RCC_BASE+0x14)
#define RCC_APB2ENR     (RCC_BASE+0x18)
#define RCC_APB1ENR     (RCC_BASE+0x1C)

#define USART1_BASE     0x40013800
#define USART1_CR1      (USART1_BASE+0x00)
#define USART1_CR2      (USART1_BASE+0x04)
#define USART1_CR3      (USART1_BASE+0x08)
#define USART1_BRR      (USART1_BASE+0x0C)
#define USART_ISR       (USART1_BASE+0x1C)
#define USART_RDR       (USART1_BASE+0x24)
#define USART_TDR       (USART1_BASE+0x28)

#define TIM2_BASE       0x40000000
#define TIM2_CR1        (TIM2_BASE+0x00)
#define TIM2_CR2        (TIM2_BASE+0x04)
#define TIM2_SR         (TIM2_BASE+0x10)
#define TIM2_CCMR1      (TIM2_BASE+0x18)
#define TIM2_CCMR2      (TIM2_BASE+0x1C)
#define TIM2_CCER       (TIM2_BASE+0x20)
#define TIM2_CNT        (TIM2_BASE+0x24)
#define TIM2_PSC        (TIM2_BASE+0x28)
#define TIM2_ARR        (TIM2_BASE+0x2C)
#define TIM2_CCR1       (TIM2_BASE+0x34)
#define TIM2_CCR2       (TIM2_BASE+0x38)
#define TIM2_CCR3       (TIM2_BASE+0x3C)
#define TIM2_CCR4       (TIM2_BASE+0x40)
#define TIM2_MASK       0xFFFFFFFF

int notmain ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //enable port a
    PUT32(RCC_AHBENR,ra);

    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(1<<1)); //PA1
    ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    ra|= (2<<(1<<1)); //PA1
    ra|= (2<<(2<<1)); //PA2
    ra|= (2<<(3<<1)); //PA3
    PUT32(GPIOA_MODER,ra);

    //ra=GET32(GPIOA_OTYPER);
    //ra&=~(1<<0); //PA0
    //ra&=~(1<<1); //PA1
    //ra&=~(1<<2); //PA2
    //ra&=~(1<<3); //PA3
    //ra&=~(1<<4); //PA4
    //ra&=~(1<<5); //PA5
    //ra&=~(1<<6); //PA6
    //ra&=~(1<<7); //PA7
    //PUT32(GPIOA_OTYPER,ra);

    ra=GET32(GPIOA_AFRL);
    ra&=~(0xF<<(1<<2)); //PA1
    ra&=~(0xF<<(2<<2)); //PA2
    ra&=~(0xF<<(3<<2)); //PA3
    ra|= (0x2<<(1<<2)); //PA1
    ra|= (0x2<<(2<<2)); //PA2
    ra|= (0x2<<(3<<2)); //PA3
    PUT32(GPIOA_AFRL,ra);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<0; //enable timer 2
    PUT32(RCC_APB1ENR,ra);

if(0)
{
    ra=GET32(RCC_APB1RSTR);
    ra|= (1<<0); //reset TIM2
    PUT32(RCC_APB1RSTR,ra);
    ra&=~(1<<0);
    PUT32(RCC_APB1RSTR,ra);

    PUT32(TIM2_CR1,0x00000000);
    PUT32(TIM2_CCR2,2000000-1);
    PUT32(TIM2_CCR3,4000000-1);
    PUT32(TIM2_CCR4,6000000-1);
    PUT32(TIM2_ARR, 8000000-1);
    PUT32(TIM2_CNT,0x00000000);
    PUT32(TIM2_PSC,0x00000000);
    PUT32(TIM2_CCMR1,(6<<12));
    PUT32(TIM2_CCMR2,(6<<12)|(6<<4));
    PUT32(TIM2_CCER,(1<<4)|(1<<8)|(1<<12));
    PUT32(TIM2_CR1,0x00000001);
}
if(1)
{
    unsigned int rb;

    ra=GET32(RCC_APB1RSTR);
    ra|= (1<<0); //reset TIM2
    PUT32(RCC_APB1RSTR,ra);
    ra&=~(1<<0);
    PUT32(RCC_APB1RSTR,ra);

    PUT32(TIM2_CR1,0x00000000);
    PUT32(TIM2_CCR2,200-1);
    PUT32(TIM2_CCR3,400-1);
    PUT32(TIM2_CCR4,600-1);
    PUT32(TIM2_ARR, 800-1);
    PUT32(TIM2_CNT,0x00000000);
    PUT32(TIM2_PSC,0x00000000);
    PUT32(TIM2_CCMR1,(6<<12));
    PUT32(TIM2_CCMR2,(6<<12)|(7<<4));
    PUT32(TIM2_CCER,(1<<4)|(1<<8)|(1<<12));
    PUT32(TIM2_CR1,0x00000001);

    while(1)
    {
        for(ra=0;ra<800;ra++)
        {
            PUT32(TIM2_CCR2,ra);
            PUT32(TIM2_CCR3,ra);
            PUT32(TIM2_CCR4,ra);
            for(rb=0;rb<2000;rb++) dummy(rb);
        }
        for(ra--;ra;ra--)
        {
            PUT32(TIM2_CCR2,ra);
            PUT32(TIM2_CCR3,ra);
            PUT32(TIM2_CCR4,ra);
            for(rb=0;rb<2000;rb++) dummy(rb);
        }
    }
}



    return(0);
}
