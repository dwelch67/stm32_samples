
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

#define GPIOB_BASE 0x40010C00
#define GPIOB_CRL  (GPIOB_BASE+0x00)
#define GPIOB_CRH  (GPIOB_BASE+0x04)
#define GPIOB_IDR  (GPIOB_BASE+0x08)
#define GPIOB_ODR  (GPIOB_BASE+0x0C)
#define GPIOB_BSRR (GPIOB_BASE+0x10)
#define GPIOB_BRR  (GPIOB_BASE+0x14)
#define GPIOB_LCKR (GPIOB_BASE+0x18)

#define RCC_BASE 0x40021000
#define RCC_APB2ENR (RCC_BASE+0x18)
#define RCC_CR      (RCC_BASE+0x00)
#define RCC_CFGR    (RCC_BASE+0x04)

#define GPIOBBASE 0x40010C00
#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#define PERIPH_BB_BASE 0x42000000

#define TIM1BASE    0x40012C00
#define TIM1_CR1    (TIM1BASE+0x00)
#define TIM1_DIER   (TIM1BASE+0x0C)
#define TIM1_SR     (TIM1BASE+0x10)

#define NVIC_BASE   0xE000E100
#define NVIC_ISER0  (NVIC_BASE+0x000)
#define NVIC_ISER1  (NVIC_BASE+0x004)
#define NVIC_ISER2  (NVIC_BASE+0x008)
#define NVIC_ICER0  (NVIC_BASE+0x080)
#define NVIC_ICER1  (NVIC_BASE+0x084)
#define NVIC_ICER2  (NVIC_BASE+0x088)
#define NVIC_ISPR0  (NVIC_BASE+0x100)
#define NVIC_ISPR1  (NVIC_BASE+0x104)
#define NVIC_ISPR2  (NVIC_BASE+0x108)
#define NVIC_ICPR0  (NVIC_BASE+0x180)
#define NVIC_ICPR1  (NVIC_BASE+0x184)
#define NVIC_ICPR2  (NVIC_BASE+0x188)
#define NVIC_IABR0  (NVIC_BASE+0x200)
#define NVIC_IABR1  (NVIC_BASE+0x204)
#define NVIC_IABR2  (NVIC_BASE+0x208)
#define NVIC_IPR00  (NVIC_BASE+0x300)
#define NVIC_IPR01  (NVIC_BASE+0x304)
#define NVIC_IPR02  (NVIC_BASE+0x308)
#define NVIC_IPR03  (NVIC_BASE+0x30C)
#define NVIC_IPR04  (NVIC_BASE+0x310)
#define NVIC_IPR05  (NVIC_BASE+0x314)
#define NVIC_IPR06  (NVIC_BASE+0x318)
#define NVIC_IPR07  (NVIC_BASE+0x31C)
#define NVIC_IPR08  (NVIC_BASE+0x320)
#define NVIC_IPR09  (NVIC_BASE+0x324)
#define NVIC_IPR10  (NVIC_BASE+0x328)
#define NVIC_IPR11  (NVIC_BASE+0x32C)
#define NVIC_IPR12  (NVIC_BASE+0x330)
#define NVIC_IPR13  (NVIC_BASE+0x334)
#define NVIC_IPR14  (NVIC_BASE+0x338)
#define NVIC_IPR15  (NVIC_BASE+0x33C)
#define NVIC_IPR16  (NVIC_BASE+0x340)
#define NVIC_IPR17  (NVIC_BASE+0x344)
#define NVIC_IPR18  (NVIC_BASE+0x348)
#define NVIC_IPR19  (NVIC_BASE+0x34C)
#define NVIC_IPR20  (NVIC_BASE+0x350)

#define NVIC_STIR   (NVIC_BASE+0xE00)


#define EXTI_BASE   0x40010400
#define EXTI_IMR    (EXTI_BASE+0x00)
#define EXTI_EMR    (EXTI_BASE+0x04)
#define EXTI_RTSR   (EXTI_BASE+0x08)
#define EXTI_FTSR   (EXTI_BASE+0x0C)
#define EXTI_SWIER  (EXTI_BASE+0x10)
#define EXTI_PR     (EXTI_BASE+0x14)


#define AFIO_BASE 0x40010000

#define AFIO_EXTICR1 (AFIO_BASE+0x08)
#define AFIO_EXTICR2 (AFIO_BASE+0x0C)
#define AFIO_EXTICR3 (AFIO_BASE+0x10)
#define AFIO_EXTICR4 (AFIO_BASE+0x14)










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
    return;
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

void int_handler ( void )
{
    uart_putc(0x55);
    PUT32(EXTI_PR,1<<8);
    PUT32(NVIC_ICPR0,1<<23);
}

static int delay ( unsigned int n )
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
    //unsigned int rb;

    clock_init();
    uart_init();
    hexstring(0x11223344);

    ra=GET32(RCC_APB2ENR);
    ra|=1<<3;   //GPIOB
    ra|=1<<0;   //AFIO
    PUT32(RCC_APB2ENR,ra);
    hexstring(GET32(GPIOB_CRL));

if(0)
{
    //PB4 OUTPUT PB3 INPUT
    ra=GET32(GPIOB_CRH);
    ra&=~(0xF<<0); //PB8
    ra|= (0x4<<0); //PB8 input
    ra&=~(0xF<<4); //PB9
    ra|= (0x1<<4); //PB9 output
    PUT32(GPIOB_CRH,ra);

    hexstring(GET32(GPIOB_IDR));
    PUT32(GPIOB_BSRR,(1<<9)<< 0);
    hexstring(GET32(GPIOB_IDR));
    PUT32(GPIOB_BSRR,(1<<9)<<16);
    hexstring(GET32(GPIOB_IDR));
}
if(0)
{
    ra=GET32(GPIOB_CRH);
    ra&=~(0xF<<0); //PB8
    ra|= (0x4<<0); //PB8 input
    ra&=~(0xF<<4); //PB9
    ra|= (0x1<<4); //PB9 output
    PUT32(GPIOB_CRH,ra);

    hexstring(GET32(GPIOB_IDR)&(1<<8));
    PUT32(GPIOB_BSRR,(1<<9)<< 0);
    hexstring(GET32(GPIOB_IDR)&(1<<8));

    PUT32(AFIO_EXTICR3,1<<0);
    PUT32(EXTI_RTSR,1<<8);
    //PUT32(EXTI_FTSR,1<<8);
    //PUT32(EXTI_SWIER,1<<8);
    PUT32(EXTI_IMR,1<<8);
    //PUT32(EXTI_EMR,1<<8);

    PUT32(GPIOB_BSRR,(1<<9)<<16);
    hexstring(GET32(GPIOB_IDR)&(1<<8));
    PUT32(GPIOB_BSRR,(1<<9)<< 0);
    hexstring(GET32(GPIOB_IDR)&(1<<8));
    hexstring(0xaabbccde);
    //hexstring(GET32(EXTI_SWIER));
    hexstring(GET32(EXTI_PR));
    hexstring(GET32(NVIC_ICPR0));
    hexstring(GET32(NVIC_ICPR1));
    hexstring(GET32(NVIC_ICPR2));
    hexstring(0xaabbccdf);
    PUT32(EXTI_PR,1<<8);
    hexstring(GET32(EXTI_PR));
    hexstring(GET32(NVIC_ICPR0));
    hexstring(GET32(NVIC_ICPR1));
    hexstring(GET32(NVIC_ICPR2));
    hexstring(0xaabbccef);
    PUT32(NVIC_ICPR0,1<<23);
    hexstring(GET32(EXTI_PR));
    hexstring(GET32(NVIC_ICPR0));
    hexstring(GET32(NVIC_ICPR1));
    hexstring(GET32(NVIC_ICPR2));
}


    ra=GET32(GPIOB_CRH);
    ra&=~(0xF<<0); //PB8
    ra|= (0x4<<0); //PB8 input
    ra&=~(0xF<<4); //PB9
    ra|= (0x1<<4); //PB9 output
    PUT32(GPIOB_CRH,ra);

    //hexstring(GET32(GPIOB_IDR)&(1<<8));
    //PUT32(GPIOB_BSRR,(1<<9)<< 0);
    //hexstring(GET32(GPIOB_IDR)&(1<<8));

    PUT32(AFIO_EXTICR3,1<<0);
    PUT32(EXTI_RTSR,1<<8);
    //PUT32(EXTI_FTSR,1<<8);
    //PUT32(EXTI_SWIER,1<<8);
    PUT32(EXTI_IMR,1<<8);
    //PUT32(EXTI_EMR,1<<8);
    PUT32(NVIC_ISER0,0x00800000);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,1000000-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);

    while(1)
    {
        PUT32(GPIOB_BSRR,(1<<9)<<16);
        delay(8);
        PUT32(GPIOB_BSRR,(1<<9)<< 0);
        delay(8);
    }

    return(0);
}
