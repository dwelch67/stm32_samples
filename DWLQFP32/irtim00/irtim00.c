
void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define GPIOB_BASE 0x48000400

#define GPIOA_BASE 0x48000000
#define GPIOA_MODER     (GPIOA_BASE+0x00)
#define GPIOA_AFRL      (GPIOA_BASE+0x20)
#define GPIOA_AFRH      (GPIOA_BASE+0x24)

#define TIM16_BASE 0x40014400
#define TIM16_CR1       (TIM16_BASE+0x00)
#define TIM16_DIER      (TIM16_BASE+0x0C)
#define TIM16_SR        (TIM16_BASE+0x10)
//#define TIM16_EGR       (TIM16_BASE+0x14)
#define TIM16_CCMR1     (TIM16_BASE+0x18)
#define TIM16_CCER      (TIM16_BASE+0x20)
#define TIM16_CNT       (TIM16_BASE+0x24)
#define TIM16_PSC       (TIM16_BASE+0x28)
#define TIM16_ARR       (TIM16_BASE+0x2C)
//#define TIM16_CCR1      (TIM16_BASE+0x34)
#define TIM16_BDTR      (TIM16_BASE+0x44)

#define TIM17_BASE 0x40014800
#define TIM17_CR1       (TIM17_BASE+0x00)
//#define TIM17_SR        (TIM17_BASE+0x10)
#define TIM17_EGR       (TIM17_BASE+0x14)
#define TIM17_CCMR1     (TIM17_BASE+0x18)
#define TIM17_CCER      (TIM17_BASE+0x20)
#define TIM17_CNT       (TIM17_BASE+0x24)
#define TIM17_PSC       (TIM17_BASE+0x28)
#define TIM17_ARR       (TIM17_BASE+0x2C)
#define TIM17_CCR1      (TIM17_BASE+0x34)
#define TIM17_BDTR      (TIM17_BASE+0x44)


#define RCC_BASE        0x40021000
#define RCC_AHBENR      (RCC_BASE+0x14) 
#define RCC_APB2ENR     (RCC_BASE+0x18)

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCC_APB2ENR);
    ra|=1<<17; //TIM16
    ra|=1<<18; //TIM17
    PUT32(RCC_APB2ENR,ra);

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //IOPA
    PUT32(RCC_AHBENR,ra);

if(1)
{
    ra=GET32(GPIOA_MODER);
    ra&=(~(3<<(7<<1))); //PA7
    ra|=  (2<<(7<<1));  //PA7 alternate function mode
    PUT32(GPIOA_MODER,ra);

    ra=GET32(GPIOA_AFRL);
    ra&=(~(0xF<<((7-0)<<2))); //PA7
    ra|=  (0x5<<((7-0)<<2));  //PA7 AF5 TIM17_CH1
    PUT32(GPIOA_AFRL,ra);
}

    ra=GET32(GPIOA_MODER);
    ra&=(~(3<<(13<<1))); //PA13
    ra|=  (2<<(13<<1));  //PA13 alternate function mode
    PUT32(GPIOA_MODER,ra);

    ra=GET32(GPIOA_AFRH);
    ra&=(~(0xF<<((13-8)<<2))); //PA13
    ra|=  (0x1<<((13-8)<<2));  //PA13 AF1 IR_OUT
    PUT32(GPIOA_AFRH,ra);

    //carrier
    PUT16(TIM17_PSC,0x10-1); 
    PUT16(TIM17_ARR,0x7A12);
    PUT16(TIM17_CCR1,0x7A12);
    PUT16(TIM17_CCMR1,(3<<4)|(1<<3)); 
    PUT16(TIM17_CCER,(1<<0)); 
    PUT16(TIM17_BDTR,(1<<15)); 
    PUT16(TIM17_CR1,(1<<0));

    //data
    PUT16(TIM16_PSC,0x1000-1);
    PUT16(TIM16_ARR,0x2000);
    PUT16(TIM16_CCMR1,(4<<4));
    PUT16(TIM16_CCER,(1<<0));
    PUT16(TIM16_BDTR,(1<<15)); 
    PUT16(TIM16_DIER,(1<<0)); 
    PUT16(TIM16_CR1,(1<<0));

    rx=0;
    while(1)
    {
        ra=GET16(TIM16_SR);
        if(ra&1)
        {
            rx++;
            if(rx&1)
            {
                PUT16(TIM16_CCMR1,(5<<4));
            }
            else
            {
                PUT16(TIM16_CCMR1,(4<<4));
            }
            PUT16(TIM16_SR,0);
        }
    }

    return(0);
}
