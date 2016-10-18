

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

void HOP ( unsigned int, unsigned int, unsigned int, unsigned int );

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
    ra|= (0x0<<18);
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
    ra|= (0x0<<8);
    ra&=~(0xF<<4);
    ra|= (0x0<<4);
    PUT32(RCC_CFGR,ra);
    //add wait states for the flash
    //PUT32(FLASH_ACR,0x2);
    //switch to the pll as the source
    ra=GET32(RCC_CFGR);
    ra&=~(0x3<<0);
    ra|= (0x2<<0);
    PUT32(RCC_CFGR,ra);
    //wait for it
    while(1) if((GET32(RCC_CFGR)&0xF)==0xA) break;
}


unsigned int ry,rz;

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
            xa=ry;//10;
            xb=rz;//6;
        }
        else
        {
            xa=rz;//6;
            xb=ry;//10;
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

    PUT32(GPIOCBASE+0x10,(1<<(13+16))); //off
    for(ra=0;ra<1000000;ra++) dummy(ra);

    ry=9;
    rz=3;

////    for(ry=1;ry<20;ry++)
    //{
  ////      for(rz=1;rz<10;rz++)
        //{
            //PUT32(0x20000100,ry);
            //PUT32(0x20000104,rz);
            //hopper=0x20000800;
            //send_24(0x010000);
            //PUT16(hopper,0x4770);
            //HOP(GPIOCBASE+0x10,1<<(13+16),1<<(13+0),0x20000801);
            //for(ra=0;ra<800000;ra++) dummy(ra);
        //}
    //}
        //PUT32(GPIOCBASE+0x10,1<<(13+0));

    for(rx=0;;rx++)
    {
        rd=0;
        if(rx&0x10) rd|=0x000001;
        if(rx&0x20) rd|=0x000100;
        if(rx&0x40) rd|=0x010000;
        hopper=0x20000010;
        if(rx&1) send_24(rd); else send_24(0);
        if(rx&2) send_24(rd); else send_24(0);
        if(rx&4) send_24(rd); else send_24(0);
        if(rx&8) send_24(rd); else send_24(0);
        PUT16(hopper,0x4770);
        HOP(GPIOCBASE+0x10,1<<(13+16),1<<(13+0),0x20000011);
        for(ra=0;ra<200000;ra++) dummy(ra);
    }

    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
