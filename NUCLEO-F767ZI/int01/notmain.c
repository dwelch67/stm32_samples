
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void DOWFI ( void );
void ienable ( void );

#define RCC_BASE        0x40023800
#define RCC_CR          (RCC_BASE+0x00)
#define RCC_PLLCFGR     (RCC_BASE+0x04)
#define RCC_CFGR        (RCC_BASE+0x08)
#define RCC_APB1RSTR    (RCC_BASE+0x20)
#define RCC_AHB1ENR     (RCC_BASE+0x30)
#define RCC_APB1ENR     (RCC_BASE+0x40)

#define GPIODBASE       0x40020C00
#define GPIOD_MODER     (GPIODBASE+0x00)
#define GPIOD_AFRH      (GPIODBASE+0x24)

#define USART3_BASE     0x40004800
#define UART_BASE       USART3_BASE
#define UART_CR1        (UART_BASE+0x00)
#define UART_BRR        (UART_BASE+0x0C)
#define UART_ISR        (UART_BASE+0x1C)
#define UART_RDR        (UART_BASE+0x24)
#define UART_TDR        (UART_BASE+0x28)

#define TIM5_BASE       0x40000C00
#define TIM5_CR1        (TIM5_BASE+0x00)
#define TIM5_DIER       (TIM5_BASE+0x0C)
#define TIM5_SR         (TIM5_BASE+0x10)
#define TIM5_CNT        (TIM5_BASE+0x24)
#define TIM5_PSC        (TIM5_BASE+0x24)
#define TIM5_ARR        (TIM5_BASE+0x2C)

#define CPUID           0xE000ED00
#define VTOR            0xE000ED08
#define ID_MMFR2        0xE000ED58

#define ICTR            0xE000E004
#define NVIC_ISER1      0xE000E104
#define NVIC_ICPR1      0xE000E284

//PD8 USART3 TX alternate function 7
//PD9 USART3 RX alternate function 7

static int clock_init ( void )
{
    //switch from internal rc at 16MHz to external based 16MHz
    unsigned int ra;

    //switch to external 8MHz clock.
    ra=GET32(RCC_CR);
    ra|=1<<16;
    PUT32(RCC_CR,ra);
    while(1) if(GET32(RCC_CR)&(1<<17)) break;
    if(1)
    {
        ra=GET32(RCC_CFGR);
        ra&=~3;
        ra|=1;
        PUT32(RCC_CFGR,ra);
        while(1) if(((GET32(RCC_CFGR)>>2)&3)==1) break;
    }
    //HSE ready
    //PLLM aim for 2mhz so 8/2=4
    //PLLN input is 2, want >=100 and <=432 so between 50 and 216
    //PLLP  16Mhz*8 = 128, 16MHz*6 = 96, not enough
    //so PLLP is 8 VCO 128 so PLLN is 64
    //dont really care about PLLQ but have to have something so 8
    PUT32(RCC_PLLCFGR,0x20000000|(8<<24)|(1<<22)|(3<<16)|(64<<6)|(4<<0));
    ra=GET32(RCC_CR);
    ra|=1<<24;
    PUT32(RCC_CR,ra);
    while(1) if(GET32(RCC_CR)&(1<<25)) break;
    ra=GET32(RCC_CFGR);
    ra&=~3;
    ra|=2;
    PUT32(RCC_CFGR,ra);
    while(1) if(((GET32(RCC_CFGR)>>2)&3)==2) break;

    return(0);
}

static int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<3; //enable port D
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<18; //enable UART_
    PUT32(RCC_APB1ENR,ra);

    ra=GET32(GPIOD_MODER);
    ra&=~(3<<(8<<1)); //PD8
    ra&=~(3<<(9<<1)); //PD9
    ra|= (2<<(8<<1)); //PD8
    ra|= (2<<(9<<1)); //PD9
    PUT32(GPIOD_MODER,ra);

    ra=GET32(GPIOD_AFRH);
    ra&=~(0xF<<((8-8)<<2)); //PD8
    ra&=~(0xF<<((9-8)<<2)); //PD9
    ra|= (0x7<<((8-8)<<2)); //PD8
    ra|= (0x7<<((9-8)<<2)); //PD9
    PUT32(GPIOD_AFRH,ra);

    ra=GET32(RCC_APB1RSTR);
    ra|=1<<18; //reset UART_
    PUT32(RCC_APB1RSTR,ra);
    ra&=~(1<<18);
    PUT32(RCC_APB1RSTR,ra);

    //16000000/115200) = 138.88
    PUT32(UART_BRR,139);
    PUT32(UART_CR1,(1<<3)|(1<<2)|(1<<0));

    return(0);
}

static void uart_send ( unsigned int x )
{
    while(1) if(GET32(UART_ISR)&(1<<7)) break;
    PUT32(UART_TDR,x);
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
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}

static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}

//******** interrupt handler *********
void tim5_handler ( void )
{
    uart_send(0x55);
    PUT32(TIM5_SR,0);
    PUT32(NVIC_ICPR1,0x00040000);
}
//******** interrupt handler *********

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    clock_init();
    uart_init();
    hexstring(0x12345678);
    hexstring(GET32(CPUID));
    //411FC270 r1p0 older than current docs
    hexstring(GET32(ID_MMFR2));
    //01000000
    //wfi is supported
    hexstring(GET32(ICTR));
    //00000003
    //0b0011
    //97-128.
    //st shows 110
    PUT32(VTOR,0x00200000);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<3; //enable TIM5
    PUT32(RCC_APB1ENR,ra);

if(0)
{
    PUT32(TIM5_CR1,0x0000);
    PUT32(TIM5_DIER,0x0000);
    PUT32(TIM5_PSC,0x0000);
    PUT32(TIM5_ARR,16000000-1);
    PUT32(TIM5_CNT,16000000-1);
    PUT32(TIM5_CR1,0x0001);
    PUT32(TIM5_SR,0);
    ra=GET32(TIM5_SR);
    hexstring(ra);
    while(1)
    {
        rb=GET32(TIM5_SR);
        if(rb!=ra)
        {
            ra=rb;
            hexstring(ra);
            PUT32(TIM5_SR,0);
        }
    }
}
if(0)
{
    PUT32(TIM5_CR1,0x0000);
    PUT32(TIM5_DIER,0x0001);
    PUT32(TIM5_PSC,0x0000);
    PUT32(TIM5_ARR,16000000-1);
    PUT32(TIM5_CNT,16000000-1);
    PUT32(TIM5_CR1,0x0001);
    PUT32(TIM5_SR,0);
    while(1)
    {
        ra=GET32(NVIC_ICPR1);
        if(ra)
        {
            hexstring(ra);
            PUT32(TIM5_SR,0);
            PUT32(NVIC_ICPR1,ra);
        }
    }
}
if(0)
{
    PUT32(TIM5_CR1,0x0000);
    PUT32(TIM5_DIER,0x0001);
    PUT32(TIM5_PSC,0x0000);
    PUT32(TIM5_ARR,16000000-1);
    PUT32(TIM5_CNT,16000000-1);
    PUT32(TIM5_CR1,0x0001);
    PUT32(TIM5_SR,0);
    while(1)
    {
        ra=GET32(NVIC_ICPR1);
        if(ra)
        {
            hexstring(ra);
            PUT32(TIM5_SR,0);
            PUT32(NVIC_ICPR1,ra);
        }
    }
}
if(1)
{
    PUT32(TIM5_CR1,0x0000);
    PUT32(TIM5_DIER,0x0001);
    PUT32(TIM5_PSC,0x0000);
    PUT32(TIM5_ARR,16000000-1);
    PUT32(TIM5_CNT,16000000-1);
    PUT32(TIM5_CR1,0x0001);
    PUT32(TIM5_SR,0);
    PUT32(NVIC_ICPR1,0x00040000);
    PUT32(NVIC_ISER1,0x00040000);
    ienable();
    while(1)
    {
        DOWFI();
        uart_send(0x56);
    }
}

    return(0);
}
