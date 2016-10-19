
//PA9  TX
//PA10 RX

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define USART1_BASE 0x40013800
#define USART1_SR   (USART1_BASE+0x00)
#define USART1_DR   (USART1_BASE+0x04)
#define USART1_BRR  (USART1_BASE+0x08)
#define USART1_CR1  (USART1_BASE+0x0C)
#define USART1_CR2  (USART1_BASE+0x10)
#define USART1_CR3  (USART1_BASE+0x14)
#define USART1_GTPR (USART1_BASE+0x18)

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

    //select HSE clock
    ra=GET32(RCC_CFGR);
    ra&=~(0x3<<0);
    ra|= (0x1<<0);
    PUT32(RCC_CFGR,ra);
    //wait for it
    while(1) if((GET32(RCC_CFGR)&0xF)==0x5) break;
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
    //4 and 5/16 = 4.3125
    //4.3125 * 16 * 115200 = 7948800
    PUT32(USART1_BRR,0x0045);
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

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
    unsigned int bdcr;

    unsigned int ht,ho;
    unsigned int mt,mo;
    unsigned int st,so;

    clock_init(); //still 8mhz just more accurate.

    uart_init();
    hexstring(0x12345678);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<28; //PWR
    PUT32(RCC_APB1ENR,ra);

    ra=GET32(PWR_CR);
    ra|=1<<8; //DBP
    PUT32(PWR_CR,ra);

    PUT32(RCC_BDCR,0x10000);
    PUT32(RCC_BDCR,0x00000);

    bdcr=GET32(RCC_BDCR);
    //bdcr&=~(3<<8); //RTCSEL should be zero
    bdcr|= (3<<8); //RTCSEL HSE
    PUT32(RCC_BDCR,bdcr);

    //bdcr=GET32(RCC_BDCR);
    bdcr|= (1<<15); //RTCEN
    PUT32(RCC_BDCR,bdcr);
bdcr=GET32(RCC_BDCR); hexstring(bdcr);

    while(1)
    {
        ra=GET32(RTC_CRL);
        if(ra&(1<<3)) break; //RSF
    }
    rtoff();

    PUT32(RTC_CRL,0x38);
    PUT32(RTC_PRLL,62499);
    PUT32(RTC_CRL,0x28);
    rtoff();

hexstring(GET32(RTC_PRLL));

    ht=1;ho=1;
    mt=5;mo=9;
    st=4;so=7;

    rb=GET32(RTC_CNTL);
    while(1)
    {
        ra=GET32(RTC_CNTL);
        if(ra!=rb)
        {
            rc=ra-rb;
            for(rd=0;rd<rc;rd++)
            {
                so++;
                if(so>9)
                {
                    so=0;
                    st++;
                    if(st>5)
                    {
                        st=0;
                        mo++;
                    }
                }
                if(mo>9)
                {
                    mo=0;
                    mt++;
                    if(mt>5)
                    {
                        mt=0;
                        ho++;
                    }
                }
                if(ht)
                {
                    if(ho>2)
                    {
                        ht=0;
                        ho=1;
                    }
                }
                else
                {
                    if(ho>9)
                    {
                        ho=0;
                        ht++;
                    }
                }
            }
            rb=ra;
            hexstrings(rb);
            hexstring((ht<<20)|(ho<<16)|(mt<<12)|(mo<<8)|(st<<4)|(so<<0));
        }
    }


    return(0);
}
