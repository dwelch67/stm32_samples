
//-------------------------------------------------------------------
//-------------------------------------------------------------------

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

#define FLASH_ACR  0x40022000

#define TIM2_BASE 0x40000000
#define TIM2_CR1 (TIM2_BASE+0x00)
#define TIM2_CR2 (TIM2_BASE+0x04)
#define TIM2_SR  (TIM2_BASE+0x10)
#define TIM2_CNT (TIM2_BASE+0x24)
#define TIM2_PSC (TIM2_BASE+0x28)
#define TIM2_ARR (TIM2_BASE+0x2C)
#define TIM2_MASK 0xFFFFFFFF

//-------------------------------------------------------------------
static void clock_init ( void )
{
    unsigned int ra;

    PUT32(FLASH_ACR,0x11);
    //HSI8 based 48MHz.  (8/2)*12
    ra=0;
    ra|=10<<18; //times 12
    ra|=0<<15;  //divide by 2
    PUT32(RCCBASE+0x04,ra);
    ra=GET32(RCCBASE+0x00);
    ra|=1<<24;
    PUT32(RCCBASE+0x00,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x00);
        if(ra&(1<<25)) break;
    }
    ra=GET32(RCCBASE+0x04);
    ra&=~3;
    ra|=2;
    PUT32(RCCBASE+0x04,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x04);
        if((ra&3)==2) break;
    }
}
//-------------------------------------------------------------------
static void led_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<8); //PA4
    ra&=~(3<<10); //PA5
    ra|=1<<8; //PA4
    ra|=1<<10; //PA5
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<4); //PA4
    ra&=~(1<<5); //PA5
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<8; //PA4
    ra|=3<<10; //PA5
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<8); //PA4
    ra&=~(3<<10); //PA5
    PUT32(GPIOABASE+0x0C,ra);    
}
//-------------------------------------------------------------------


int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rx;

    clock_init();
    led_init();
    PUT32(GPIOABASE+0x18,(1<<(5+16))|(1<<(4+16)));

    ra=GET32(RCCBASE+0x1C);
    ra|=1<<0; //enable timer 2
    PUT32(RCCBASE+0x1C,ra);

if(1)
{
    PUT32(TIM2_CR1,0x00000000);
    PUT32(TIM2_ARR,0xFFFFFFFF);
    PUT32(TIM2_CNT,0x00000000);
    PUT32(TIM2_PSC,0x00000000);
    PUT32(TIM2_CR1,0x00000001);
    for(rx=0;rx<5;rx++)
    {
        PUT32(GPIOABASE+0x18,(1<<(5+ 0))|(1<<(4+16)));
        while(1)
        {
            ra=GET32(TIM2_CNT);
            if((ra&0x01000000)!=0) break;
        }
        PUT32(GPIOABASE+0x18,(1<<(5+16))|(1<<(4+ 0)));
        while(1)
        {
            ra=GET32(TIM2_CNT);
            if((ra&0x01000000)==0) break;
        }
    }
}

if(1)
{
    PUT32(TIM2_CR1,0x00000000);
    PUT32(TIM2_ARR,0xFFFFFFFF);
    PUT32(TIM2_CNT,0x00000000);
    PUT32(TIM2_PSC,0x00000000);
    PUT32(TIM2_CR1,0x00000001);
    rb=GET32(TIM2_CNT);
    for(rx=0;rx<3;rx++)
    {
        PUT32(GPIOABASE+0x18,(1<<(5+ 0))|(1<<(4+16)));
        while(1)
        {
            ra=GET32(TIM2_CNT);
            rc=(ra-rb)&TIM2_MASK;
            if(rc>=48000000) break;
        }
        rb=ra;
        PUT32(GPIOABASE+0x18,(1<<(5+16))|(1<<(4+ 0)));
        while(1)
        {
            ra=GET32(TIM2_CNT);
            rc=(ra-rb)&TIM2_MASK;
            if(rc>=48000000) break;
        }
        rb=ra;
    }

}
if(1)
{
    PUT32(TIM2_CR1,0x00000000);
    PUT32(TIM2_ARR,24000000-1);
    PUT32(TIM2_CNT,0x00000000);
    PUT32(TIM2_PSC,0x00000000);
    PUT32(TIM2_CR1,0x00000001);
    rb=GET32(TIM2_CNT);
    //for(rx=0;rx<4;rx++)
    while(1)
    {
        PUT32(GPIOABASE+0x18,(1<<(5+ 0))|(1<<(4+16)));
        while(1)
        {
            ra=GET32(TIM2_SR);
            if(ra)
            {
                PUT32(TIM2_SR,0);
                break;
            }
        }
        PUT32(GPIOABASE+0x18,(1<<(5+16))|(1<<(4+ 0)));
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
}


    PUT32(GPIOABASE+0x18,(1<<(5+16))|(1<<(4+16)));
    return(0);
}
