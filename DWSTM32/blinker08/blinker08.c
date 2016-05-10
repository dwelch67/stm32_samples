
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

#define TIM14_BASE 0x40002000

#define TIM16_BASE 0x40014400
#define TIM16_CR1   (TIM16_BASE+0x00)
#define TIM16_SR    (TIM16_BASE+0x10)
#define TIM16_CCMR1 (TIM16_BASE+0x18)
#define TIM16_CCER  (TIM16_BASE+0x20)
#define TIM16_CNT   (TIM16_BASE+0x24)
#define TIM16_PSC   (TIM16_BASE+0x28)
#define TIM16_ARR   (TIM16_BASE+0x2C)
#define TIM16_CCR1  (TIM16_BASE+0x34)
#define TIM16_BDTR  (TIM16_BASE+0x44)

#define TIM17_BASE 0x40014800
#define TIM17_CR1   (TIM17_BASE+0x00)
#define TIM17_SR    (TIM17_BASE+0x10)
#define TIM17_CCMR1 (TIM17_BASE+0x18)
#define TIM17_CCER  (TIM17_BASE+0x20)
#define TIM17_CNT   (TIM17_BASE+0x24)
#define TIM17_PSC   (TIM17_BASE+0x28)
#define TIM17_ARR   (TIM17_BASE+0x2C)
#define TIM17_CCR1  (TIM17_BASE+0x34)
#define TIM17_BDTR  (TIM17_BASE+0x44)



#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF


int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<12); //PA6
    ra&=~(3<<14); //PA7
    ra&=~(3<<26); //PA13
    ra|=(2<<12); //PA6 alternate function
    ra|=(2<<14); //PA7 alternate function
    ra|=(2<<26); //PA13 alternate function
    PUT32(GPIOABASE+0x00,ra);
    //AFRL
    ra=GET32(GPIOABASE+0x20);
    ra&=~(0xF<<24); //PA6
    ra&=~(0xF<<28); //PA7
    ra|=5<<24; //PA6
    ra|=5<<28; //PA7
    PUT32(GPIOABASE+0x20,ra);
    //AFRH
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<20); //PA13
    ra|=1<<20; //PA13
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<17; //enable timer 16
    ra|=1<<18; //enable timer 17
    PUT32(RCCBASE+0x18,ra);

    ra=GET32(RCCBASE+0x1C);
    ra|=1<<8; //enable timer 14
    PUT32(RCCBASE+0x1C,ra);

    PUT16(TIM16_CR1,0x0000);
    PUT16(TIM16_ARR,0xFFFF);
    PUT16(TIM16_CNT,0x0000);
    PUT16(TIM16_PSC,0x00FF);
    PUT16(TIM16_CCMR1,0x0030);
    PUT16(TIM16_CCER,0x0001);
    PUT16(TIM16_CCR1,0x0000);
    PUT16(TIM16_BDTR,0x8000);
    PUT16(TIM16_CR1,0x0001);

    PUT16(TIM17_CR1,0x0000);
    PUT16(TIM17_ARR,0x0FFF);
    PUT16(TIM17_CNT,0x0000);
    PUT16(TIM17_PSC,0x00FF);
    PUT16(TIM17_CCMR1,0x0030);
    PUT16(TIM17_CCER,0x0001);
    PUT16(TIM17_CCR1,0x0000);
    PUT16(TIM17_BDTR,0x8000);
    PUT16(TIM17_CR1,0x0001);

    return(0);
}
