
//PA9  TX
//PA10 RX

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define STK_CTRL (0xE000E010+0x00)
#define STK_LOAD (0xE000E010+0x04)
#define STK_VAL  (0xE000E010+0x08)

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
    //72000000/(16*115200) = 39.0625
    //0.0625 * 16 = 1
    //39 1/16 0x27
    PUT32(USART1_BRR,0x0271);
    PUT32(USART1_CR1,0x200C);
}
static unsigned int uart_getc ( void )
{
    while(1)
    {
        if(GET32(USART1_SR)&0x20) break;
    }
    return(GET32(USART1_DR)&0xFF);
}
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
int notmain ( void )
{
    unsigned int ra;

    clock_init();
    uart_init();
    hexstring(0x12345678);
    while(1)
    {
        ra=uart_getc();
        hexstring(ra);
    }
    return(0);
}
