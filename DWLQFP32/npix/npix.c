

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

void HOP ( unsigned int, unsigned int, unsigned int, unsigned int );

#define GPIOB_BASE 0x48000400
#define GPIOA_BASE 0x48000000
#define RCC_BASE 0x40021000
#define USART1_BASE 0x40013800

#define FLASH_ACR 0x40022000

//-------------------------------------------------------------------
void clock_init ( void )
{
    unsigned int ra;

    PUT32(FLASH_ACR,0x11);
    //HSI8 based 48MHz.  (8/2)*12
    ra=0;
    ra|=2<<18; //times 4
    ra|=0<<16;  //divide by 2
    PUT32(RCC_BASE+0x04,ra);
    ra=GET32(RCC_BASE+0x00);
    ra|=1<<24;
    PUT32(RCC_BASE+0x00,ra);
    while(1)
    {
        ra=GET32(RCC_BASE+0x00);
        if(ra&(1<<25)) break;
    }
    ra=GET32(RCC_BASE+0x04);
    ra&=~3;
    ra|=2;
    PUT32(RCC_BASE+0x04,ra);
    while(1)
    {
        ra=GET32(RCC_BASE+0x04);
        if((ra&3)==2) break;
    }
}
//------------------------------------------------------------------------
static unsigned int hopper;
static void send_24 ( unsigned int x )
{
    unsigned int ra;
    //unsigned int rd;
    unsigned int xa;
    unsigned int xb;
    unsigned int rb;
    
    for(rb=0x00800000;rb;rb>>=1)
    {
        if(x&rb)
        {
            xa=10;
            xb=5;
        }
        else
        {
            xa=5;
            xb=10;
        }
        PUT16(hopper,0x6002); hopper+=2;
        for(ra=0;ra<xa;ra++) { PUT16(hopper,0x46C0); hopper+=2; }
        PUT16(hopper,0x6001); hopper+=2;
        for(ra=0;ra<xb;ra++) { PUT16(hopper,0x46C0); hopper+=2; }
    }
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rd;
    unsigned int rx;

    clock_init();
    //uart_init();
    //hexstring(0x12345678);



    ra=GET32(RCC_BASE+0x14);
    ra|=1<<18; //enable port B
    PUT32(RCC_BASE+0x14,ra);
    //moder
    ra=GET32(GPIOB_BASE+0x00);
    ra&=~(3<<2); //PB1
    ra|=1<<2; //PB1
    PUT32(GPIOB_BASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOB_BASE+0x04);
    ra&=~(1<<1); //PB1
    PUT32(GPIOB_BASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOB_BASE+0x08);
    ra|=3<<2; //PB1
    PUT32(GPIOB_BASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOB_BASE+0x0C);
    ra&=~(3<<2); //PB1
    PUT32(GPIOB_BASE+0x0C,ra);

    PUT32(GPIOB_BASE+0x18,(1<<(1+16))); //off
    for(ra=0;ra<200000;ra++) dummy(ra);    

    hopper=0x20000100;
    send_24(0x00010000);
    send_24(0x00000100);
    send_24(0x00000001);
    send_24(0x00000002);
    PUT16(hopper,0x4770);
    HOP(GPIOB_BASE+0x18,1<<(1+16),1<<(1+0),0x20000101);

    for(rx=0;;rx++)
    {
        rd=0;
        if(rx&0x10) rd|=0x000001;
        if(rx&0x20) rd|=0x000100;
        if(rx&0x40) rd|=0x010000;
        hopper=0x20000100;
        if(rx&1) send_24(rd); else send_24(0);
        if(rx&2) send_24(rd); else send_24(0);
        if(rx&4) send_24(rd); else send_24(0);
        if(rx&8) send_24(rd); else send_24(0);
        PUT16(hopper,0x4770);
        HOP(GPIOB_BASE+0x18,1<<(1+16),1<<(1+0),0x20000101);
        for(ra=0;ra<200000;ra++) dummy(ra);
    }
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
