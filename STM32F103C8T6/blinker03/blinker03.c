
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOCBASE 0x40011000
#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#define RCC_BASE 0x40021000
#define RCC_APB2ENR (RCC_BASE+0x18)
#define RCC_APB1ENR (RCC_BASE+0x1C)
#define RCC_CR      (RCC_BASE+0x00)
#define RCC_CFGR    (RCC_BASE+0x04)
#define FLASH_ACR   0x40022000

static void clock_init ( void )
{
    unsigned int ra;

    //enable the external clock
    ra=GET32(RCC_CR);
    ra=ra|1<<16; //HSEON
    PUT32(RCC_CR,ra);

    //wait for HSE to settle
    while(1) if(GET32(RCC_CR)&(1<<17)) break; //HSERDY
    if(0)
    {
        //select HSE clock
        ra=GET32(RCC_CFGR);
        ra&=~(0x3<<0);
        ra|= (0x1<<0);
        PUT32(RCC_CFGR,ra);
        //wait for it
        while(1) if((GET32(RCC_CFGR)&0xF)==0x5) break;
        return;
    }
    //setup pll source settings
    ra=GET32(RCC_CFGR);
    ra&=~(0xF<<18);
    ra|= (0x7<<18);
    ra&=~(1<<17);
    ra|= (0<<17);
    ra&=~(1<<16);
    ra|= (1<<16);
    PUT32(RCC_CFGR,ra);
    //enable the pll
    ra=GET32(RCC_CR);
    ra=ra|1<<24; //PLLON
    PUT32(RCC_CR,ra);
    //wait for pll to lock
    while(1) if(GET32(RCC_CR)&(1<<25)) break; //HSERDY
    //USB div/1.5 AHB not divided, APB not divided, APB1 div/2, MCO, do you need this?
    ra=GET32(RCC_CFGR);
    ra&=~(0x7<<24);
    ra|= (0x7<<24);
    ra&=~(0x1<<22);
    ra|= (0x0<<22);
    ra&=~(0x7<<11);
    ra|= (0x0<<11);
    ra&=~(0x7<<8);
    ra|= (0x4<<8);
    ra&=~(0xF<<4);
    ra|= (0x0<<4);
    PUT32(RCC_CFGR,ra);
    //add wait states for the flash
    PUT32(FLASH_ACR,0x2);
    //switch to the pll as the source
    ra=GET32(RCC_CFGR);
    ra&=~(0x3<<0);
    ra|= (0x2<<0);
    PUT32(RCC_CFGR,ra);
    //wait for it
    while(1) if((GET32(RCC_CFGR)&0xF)==0xA) break;
}

int delay ( unsigned int n )
{
    unsigned int ra;

    while(n--)
    {
        while(1)
        {
            ra=GET32(STK_CSR);
            if(ra&(1<<16)) break;
        }
    }
    return(0);
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    clock_init();

    ra=GET32(RCCBASE+0x18);
    ra|=1<<4; //enable port c
    PUT32(RCCBASE+0x18,ra);
    //config
    ra=GET32(GPIOCBASE+0x04);
    ra&=~(3<<20);   //PC13
    ra|=1<<20;      //PC13
    ra&=~(3<<22);   //PC13
    ra|=0<<22;      //PC13
    PUT32(GPIOCBASE+0x04,ra);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,1000000-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);

    for(rx=0;;rx++)
    {
        PUT32(GPIOCBASE+0x10,1<<(13+0));
        delay(500);
        PUT32(GPIOCBASE+0x10,1<<(13+16));
        delay(500);
    }
    return(0);
}
