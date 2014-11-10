
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOCBASE 0x48000800
#define RCCBASE 0x40021000
#define RCC_APB1ENR (RCCBASE+0x1C)

#define TIM6BASE  0x40001000
#define TIM6_CR1  (TIM6BASE+0x00)
#define TIM6_CR2  (TIM6BASE+0x04)
#define TIM6_DIER (TIM6BASE+0x0C)
#define TIM6_SR   (TIM6BASE+0x10)
#define TIM6_EGR  (TIM6BASE+0x14)
#define TIM6_CNT  (TIM6BASE+0x24)
#define TIM6_PSC  (TIM6BASE+0x28)
#define TIM6_ARR  (TIM6BASE+0x2C)

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<19; //enable port C
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOCBASE+0x00);
    ra&=~(3<<16); //PC8
    ra&=~(3<<18); //PC9
    ra|=1<<16; //PC8
    ra|=1<<18; //PC9
    PUT32(GPIOCBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOCBASE+0x04);
    ra&=~(1<<8); //PC8
    ra&=~(1<<9); //PC9
    PUT32(GPIOCBASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOCBASE+0x08);
    ra|=3<<16; //PC8
    ra|=3<<18; //PC8
    PUT32(GPIOCBASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOCBASE+0x0C);
    ra&=~(3<<16); //PC8
    ra&=~(3<<18); //PC9
    PUT32(GPIOCBASE+0x0C,ra);

    ra=GET32(RCCBASE+0x1C);
    ra|=1<<4; //tim6 enable
    PUT32(RCCBASE+0x1C,ra);

    PUT16(TIM6_CR1,0x0000);
    PUT16(TIM6_DIER,0x0000);
    PUT16(TIM6_PSC,0xFFFF);
    PUT16(TIM6_ARR,122);
    PUT16(TIM6_CNT,122);
    PUT16(TIM6_CR1,0x0001);

    while(1)
    {
        PUT32(GPIOCBASE+0x18,0x01000200);
        for(rx=0;rx<10;rx++)
        {
            while(1) if(GET16(TIM6_SR)&1) break;
            PUT16(TIM6_SR,0);
        }
        PUT32(GPIOCBASE+0x18,0x02000100);
        while(1) if(GET16(TIM6_SR)&1) break;
        PUT16(TIM6_SR,0);
    }
    return(0);
}
