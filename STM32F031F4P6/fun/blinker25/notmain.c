
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
#define TIM2_CNT        (TIM2_BASE+0x24)
#define TIM2_PSC        (TIM2_BASE+0x28)
#define TIM2_ARR        (TIM2_BASE+0x2C)
#define TIM2_MASK       0xFFFFFFFF

int notmain ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //enable port a
    PUT32(RCC_AHBENR,ra);

    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    ra&=~(3<<(4<<1)); //PA4
    ra&=~(3<<(5<<1)); //PA5
    ra&=~(3<<(6<<1)); //PA6
    ra&=~(3<<(7<<1)); //PA7
    ra|= (1<<(0<<1)); //PA0
    ra|= (1<<(1<<1)); //PA1
    ra|= (1<<(2<<1)); //PA2
    ra|= (1<<(3<<1)); //PA3
    ra|= (1<<(4<<1)); //PA4
    ra|= (1<<(5<<1)); //PA5
    ra|= (1<<(6<<1)); //PA6
    ra|= (1<<(7<<1)); //PA7
    PUT32(GPIOA_MODER,ra);

    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<0); //PA0
    ra&=~(1<<1); //PA1
    ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    ra&=~(1<<4); //PA4
    ra&=~(1<<5); //PA5
    ra&=~(1<<6); //PA6
    ra&=~(1<<7); //PA7
    PUT32(GPIOA_OTYPER,ra);


    ra=GET32(RCC_APB1ENR);
    ra|=1<<0; //enable timer 2
    PUT32(RCC_APB1ENR,ra);

    ra=GET32(RCC_APB1RSTR);
    ra|= (1<<0); //reset TIM2
    PUT32(RCC_APB1RSTR,ra);
    ra&=~(1<<0);
    PUT32(RCC_APB1RSTR,ra);

    PUT32(TIM2_CR1,0x00000000);
    PUT32(TIM2_ARR,8000000-1);
    PUT32(TIM2_CNT,0x00000000);
    PUT32(TIM2_PSC,0x00000000);
    PUT32(TIM2_CR1,0x00000001);
    GET32(TIM2_CNT);
    while(1)
    {
        PUT32(GPIOA_BSRR,0x000000FF);
        while(1)
        {
            ra=GET32(TIM2_SR);
            if(ra)
            {
                PUT32(TIM2_SR,0);
                break;
            }
        }
        PUT32(GPIOA_BSRR,0x00FF0000);
        while(1)
        {
            ra=GET32(TIM2_SR);
            if(ra)
            {
                PUT32(TIM2_SR,0);
                break;
            }
        }
    }
    return(0);
}
