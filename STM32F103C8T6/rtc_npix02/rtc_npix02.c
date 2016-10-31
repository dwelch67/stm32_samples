

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

void HOP ( unsigned int, unsigned int, unsigned int, unsigned int );

#define GPIOBBASE 0x40010C00

#define FLASH_ACR   0x40022000

#define USART1_BASE 0x40013800
#define USART1_SR   (USART1_BASE+0x00)
#define USART1_DR   (USART1_BASE+0x04)
#define USART1_BRR  (USART1_BASE+0x08)
#define USART1_CR1  (USART1_BASE+0x0C)
#define USART1_CR2  (USART1_BASE+0x10)
#define USART1_CR3  (USART1_BASE+0x14)
#define USART1_GTPR (USART1_BASE+0x18)

#define GPIOABASE 0x40010800
#define GPIOA_BASE 0x40010800
#define GPIOA_CRL  (GPIOA_BASE+0x00)
#define GPIOA_CRH  (GPIOA_BASE+0x04)
#define GPIOA_IDR  (GPIOA_BASE+0x08)
#define GPIOA_ODR  (GPIOA_BASE+0x0C)
#define GPIOA_BSRR (GPIOA_BASE+0x10)
#define GPIOA_BRR  (GPIOA_BASE+0x14)
#define GPIOA_LCKR (GPIOA_BASE+0x18)

#define RCC_BASE 0x40021000
#define RCC_CR      (RCC_BASE+0x00)
#define RCC_CFGR    (RCC_BASE+0x04)
#define RCC_APB2ENR (RCC_BASE+0x18)
#define RCC_APB1ENR (RCC_BASE+0x1C)
#define RCC_BDCR    (RCC_BASE+0x20)

#define PWR_BASE 0x40007000
#define PWR_CR  (PWR_BASE + 0x00)
#define PWR_CSR (PWR_BASE + 0x04)

#define RTC_BASE 0x40002800
#define RTC_CRL (RTC_BASE + 0x04)
#define RTC_PRLH (RTC_BASE + 0x08)
#define RTC_PRLL (RTC_BASE + 0x0C)
#define RTC_CNTL (RTC_BASE + 0x1C)



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


static void uart_init ( void )
{
    //assuming 8MHz clock, 115200 8N1
    unsigned int ra;

    ra=GET32(RCC_APB2ENR);
    ra|=1<<2;   //GPIOA
    ra|=1<<14;  //USART1
    PUT32(RCC_APB2ENR,ra);

    //pa9 TX  alternate function output push-pull
    //pa10 RX configure as input floating
    ra=GET32(GPIOA_CRH);
    ra&=~(0xFF0);
    ra|=0x490;
    PUT32(GPIOA_CRH,ra);

    PUT32(USART1_CR1,0x2000);
    PUT32(USART1_CR2,0x0000);
    PUT32(USART1_CR3,0x0000);
    //8000000/16 = 500000
    //500000/115200 = 4.34
    //4 and 5/16 = 4.3125   8.68   8 10
    //4.3125 * 16 * 115200 = 7948800
    PUT32(USART1_BRR,0x008A);
    PUT32(USART1_CR1,0x200C);
}
//static unsigned int uart_getc ( void )
//{
    //while(1)
    //{
        //if(GET32(USART1_SR)&0x20) break;
    //}
    //return(GET32(USART1_DR)&0xFF);
//}
static void uart_putc ( unsigned int c )
{
    while(1)
    {
        if(GET32(USART1_SR)&0x80) break;
    }
    PUT32(USART1_DR,c);
}
static void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_putc(rc);
        if(rb==0) break;
    }
    uart_putc(0x20);
}
static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_putc(0x0D);
    uart_putc(0x0A);
}


static void rtoff ( void )
{
    unsigned int ra;
    while(1)
    {
        ra=GET32(RTC_CRL);
        if(ra&(1<<5)) break;
    }
}

static void rtc_init ( void )
{
    unsigned int ra;
    unsigned int bdcr;

    ra=GET32(RCC_APB1ENR);
    ra|=1<<28; //PWR
    PUT32(RCC_APB1ENR,ra);

    ra=GET32(PWR_CR);
    ra|=1<<8; //DBP
    PUT32(PWR_CR,ra);


    PUT32(RCC_BDCR,0x10000);
    PUT32(RCC_BDCR,0x00000);

    bdcr=GET32(RCC_BDCR);
//hexstring(bdcr);
    bdcr|=1<<0; //LSEON
    PUT32(RCC_BDCR,bdcr);

    while(1)
    {
        bdcr=GET32(RCC_BDCR);
        if(bdcr&(1<<1)) break; //LSERDY
    }

    //bdcr=GET32(RCC_BDCR);
    bdcr&=~(3<<8); //RTCSEL should be zero
    bdcr|= (1<<8); //RTCSEL LSE
    PUT32(RCC_BDCR,bdcr);

    //bdcr=GET32(RCC_BDCR);
    bdcr|= (1<<15); //RTCEN
    PUT32(RCC_BDCR,bdcr);
//hexstring(bdcr);

    while(1)
    {
        ra=GET32(RTC_CRL);
        if(ra&(1<<3)) break; //RSF
    }
    rtoff();
    PUT32(RTC_CRL,0x38);
    //PUT32(RTC_PRLL,32767);
    PUT32(RTC_PRLH,0x000E);
    PUT32(RTC_PRLL,0xFFFF);    
    PUT32(RTC_CRL,0x28);
    rtoff();
}

static unsigned int ht,ho;
static unsigned int mt,mo;

static void long_div ( unsigned int ra )
{
    unsigned int num;
    unsigned int res;
    unsigned int acc;
    unsigned int den;
    unsigned int rb;
    unsigned int hh;
    unsigned int mm;
    
    num=ra;
    res=0;
    acc=0;
    den=1440;
    for(rb=0x80000000;rb;rb>>=1)
    {
        acc<<=1;
        if(rb&num) acc|=1;
        if(acc>=den)
        {
            acc-=den;
            res|=rb;
        }
    }
    //printf("%u %u %u ",ra,res,acc);
    num=acc;
    res=0;
    acc=0;
    den=120;
    for(rb=0x80000000;rb;rb>>=1)
    {
        acc<<=1;
        if(rb&num) acc|=1;
        if(acc>=den)
        {
            acc-=den;
            res|=rb;
        }
    }
    //printf("%u %u %u\n",res,acc,acc>>1);
    hh=res;
    if(hh==0) hh=12;
    mm=acc>>1;

    if(hh>9)
    {
        ht=1;
        ho=hh-10;
    }
    else
    {
        ht=0;
        ho=hh;
    }
    num=mm;
    res=0;
    acc=0;
    den=10;
    for(rb=0x20;rb;rb>>=1)
    {
        acc<<=1;
        if(rb&num) acc|=1;
        if(acc>=den)
        {
            acc-=den;
            res|=rb;
        }
    }
    mt=res;
    mo=acc;
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

void show_nibble ( unsigned int pin, unsigned int rx )
{
    hopper=0x20000100;
    if(rx&1) send_24(0x010000); else send_24(0);
    if(rx&2) send_24(0x010000); else send_24(0);
    if(rx&4) send_24(0x010000); else send_24(0);
    if(rx&8) send_24(0x010000); else send_24(0);
    PUT16(hopper,0x4770);
    HOP(GPIOBBASE+0x10,1<<(pin+16),1<<(pin+0),0x20000101);
}

void show_clock ( void )
{
    unsigned int hh;

    hh=ho;
    if(ht) hh+=10;
    show_nibble(5,mo);
    show_nibble(6,mt);
    show_nibble(7,hh);
}

int notmain ( void )
{
    //unsigned int rx;
    
    unsigned int ra;
    unsigned int rlast;

    unsigned int epoch;

    clock_init(); 

    uart_init();
    hexstring(0x12345678);

    
    ra=GET32(RCC_BASE+0x18);
    ra|=1<<3; //enable port b
    PUT32(RCC_BASE+0x18,ra);
    //config
    ra=GET32(GPIOBBASE+0x00);
    ra&=~(3<<20);   //PB5
    ra|=  1<<20;    //PB5
    ra&=~(3<<22);   //PB5
    ra|=  0<<22;    //PB5
    ra&=~(3<<24);   //PB6
    ra|=  1<<24;    //PB6
    ra&=~(3<<26);   //PB6
    ra|=  0<<26;    //PB6
    ra&=~(3<<28);   //PB7
    ra|=  1<<28;    //PB7
    ra&=~(3<<30);   //PB7
    ra|=  0<<30;    //PB7
    PUT32(GPIOBBASE+0x00,ra);

    PUT32(GPIOBBASE+0x10,(1<<(5+16))); //off
    PUT32(GPIOBBASE+0x10,(1<<(6+16))); //off
    PUT32(GPIOBBASE+0x10,(1<<(7+16))); //off
    for(ra=0;ra<1000000;ra++) dummy(ra);

    ry=9;
    rz=3;


    //for(rx=0;rx<32;rx++)
    //{
        //rd=0;
        //if(rx&0x10) rd|=0x000001;
        //if(rx&0x20) rd|=0x000100;
        //if(rx&0x40) rd|=0x010000;
        //hopper=0x20000100;
        //if(rx&1) send_24(rd); else send_24(0);
        //if(rx&2) send_24(rd); else send_24(0);
        //if(rx&4) send_24(rd); else send_24(0);
        //if(rx&8) send_24(rd); else send_24(0);
        //PUT16(hopper,0x4770);
        //HOP(GPIOBBASE+0x10,1<<(5+16),1<<(5+0),0x20000101);
        //HOP(GPIOBBASE+0x10,1<<(6+16),1<<(6+0),0x20000101);
        //HOP(GPIOBBASE+0x10,1<<(7+16),1<<(7+0),0x20000101);
        //for(ra=0;ra<200000;ra++) dummy(ra);
    //}


    for(ra=0;ra<200000;ra++) dummy(ra);

    rtc_init();


    epoch=(8*120)+(58<<1);

    rlast=111111;//GET32(RTC_CNTL);
    while(1)
    {
        ra=GET32(RTC_CNTL);
        if(ra!=rlast)
        {
            rlast=ra;
            long_div(epoch+rlast);
            show_clock();
        }
    }
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
