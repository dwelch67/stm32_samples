
//------------------------------------------------------------------------
//------------------------------------------------------------------------


void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE 0x40021000
#define RCC_CR          (RCCBASE+0x00)
#define RCC_CFGR        (RCCBASE+0x04)
#define RCC_APB2RSTR    (RCCBASE+0x0C)
#define RCC_AHBENR      (RCCBASE+0x14)
#define RCC_APB2ENR     (RCCBASE+0x18)

#define GPIOABASE 0x48000000
#define GPIOA_MODER     (GPIOABASE+0x00)
#define GPIOA_OTYPER    (GPIOABASE+0x04)
#define GPIOA_OSPEEDR   (GPIOABASE+0x08)
#define GPIOA_PUPDR     (GPIOABASE+0x0C)
#define GPIOA_IDR       (GPIOABASE+0x10)
#define GPIOA_ODR       (GPIOABASE+0x14)
#define GPIOA_BSRR      (GPIOABASE+0x18)
#define GPIOA_LCKR      (GPIOABASE+0x1C)
#define GPIOA_AFRL      (GPIOABASE+0x20)
#define GPIOA_AFRH      (GPIOABASE+0x24)
#define GPIOA_BRR       (GPIOABASE+0x28)

#define GPIOCBASE 0x48000800
#define GPIOC_MODER     (GPIOCBASE+0x00)
#define GPIOC_OTYPER    (GPIOCBASE+0x04)
#define GPIOC_OSPEEDR   (GPIOCBASE+0x08)
#define GPIOC_PUPDR     (GPIOCBASE+0x0C)
#define GPIOC_IDR       (GPIOCBASE+0x10)
#define GPIOC_ODR       (GPIOCBASE+0x14)
#define GPIOC_BSRR      (GPIOCBASE+0x18)
#define GPIOC_LCKR      (GPIOCBASE+0x1C)
#define GPIOC_AFRL      (GPIOCBASE+0x20)
#define GPIOC_AFRH      (GPIOCBASE+0x24)
#define GPIOC_BRR       (GPIOCBASE+0x28)

#define USART1BASE 0x40013800
#define USART1_CR1      (USART1BASE+0x00)
#define USART1_BRR      (USART1BASE+0x0C)
#define USART1_ISR      (USART1BASE+0x1C)
#define USART1_RDR      (USART1BASE+0x24)
#define USART1_TDR      (USART1BASE+0x28)

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF;

const unsigned char numpat[10]=
{
            //xGFE DCBA
    0x3F,   //0011 1111 0
    0x06,   //0000 0110 1
    0x5B,   //0101 1011 2
    0x4F,   //0100 1111 3
    0x66,   //0110 0110 4
    0x6D,   //0110 1101 5
    0x7D,   //0111 1101 6
    0x07,   //0000 0111 7
    0x7F,   //0111 1111 8
    0x67,   //0110 0111 9
};

//------------------------------------------------------------------------
int clock_init ( void )
{
    unsigned int ra;

    //switch to external clock.
    ra=GET32(RCC_CR);
    ra|=1<<16;
    PUT32(RCC_CR,ra);
    while(1) if(GET32(RCC_CR)&(1<<17)) break;
    ra=GET32(RCC_CFGR);
    ra&=~3;
    ra|=1;
    PUT32(RCC_CFGR,ra);
    while(1) if((GET32(RCC_CFGR)&3)==1) break;
    return(0);
}
//------------------------------------------------------------------------
int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //enable port A
    PUT32(RCC_AHBENR,ra);

    ra=GET32(RCC_APB2ENR);
    ra|=1<<14; //enable USART1
    PUT32(RCC_APB2ENR,ra);

    //moder 10
    ra=GET32(GPIOA_MODER);
    ra&=~(3<<18); //PA9
    ra&=~(3<<20); //PA10
    ra|=2<<18; //PA9
    ra|=2<<20; //PA10
    PUT32(GPIOA_MODER,ra);
    //OTYPER 0
    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<9); //PA9
    ra&=~(1<<10); //PA10
    PUT32(GPIOA_OTYPER,ra);
    //ospeedr 11
    ra=GET32(GPIOA_OSPEEDR);
    ra|=3<<18; //PA9
    ra|=3<<20; //PA10
    PUT32(GPIOA_OSPEEDR,ra);
    //pupdr 00
    ra=GET32(GPIOA_PUPDR);
    ra&=~(3<<18); //PA9
    ra&=~(3<<20); //PA10
    PUT32(GPIOA_PUPDR,ra);
    //afr 0001
    ra=GET32(GPIOA_AFRH);
    ra&=~(0xF<<4); //PA9
    ra&=~(0xF<<8); //PA10
    ra|=0x1<<4; //PA9
    ra|=0x1<<8; //PA10
    PUT32(GPIOA_AFRH,ra);




    ra=GET32(RCC_APB2RSTR);
    ra|=1<<14; //reset USART1
    PUT32(RCC_APB2RSTR,ra);
    ra&=~(1<<14);
    PUT32(RCC_APB2RSTR,ra);

    PUT32(USART1_BRR,69);
    PUT32(USART1_CR1,(1<<3)|(1<<2)|1);

    return(0);
}
//------------------------------------------------------------------------
void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1_ISR)&(1<<7)) break;
    PUT32(USART1_TDR,x);
}
//------------------------------------------------------------------------
unsigned int uart_recv ( void )
{
    while(1) if((GET32(USART1_ISR))&(1<<5)) break;
    return(GET32(USART1_RDR));
}
//------------------------------------------------------------------------
void hexstrings ( unsigned int d )
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
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
//------------------------------------------------------------------------
void blink_one ( unsigned int bank )
{
    PUT32(GPIOCBASE+0x18,0x00000000|(1<<(bank+0)));
    while(1) if(GET32(STK_CVR)&0x400) break;
    while(1) if((GET32(STK_CVR)&0x400)==0) break;
    PUT32(GPIOCBASE+0x18,0x00000000|(1<<(bank+16)));
}
//------------------------------------------------------------------------
void timer_delay ( unsigned int d )
{
    unsigned int ra,rb;

    ra=GET32(STK_CVR);
    while(1)
    {
        rb=(ra-GET32(STK_CVR))&STK_MASK;
        if(rb>=d) break;
    }
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    unsigned int pattern[10];

    for(ra=0;ra<10;ra++)
    {
        pattern[ra]=numpat[ra]<<16;
        pattern[ra]|=(~(numpat[ra])&0xFF);
    }


    clock_init();
    uart_init();
    hexstrings(0x1234); hexstring(0xABCD);
    hexstring(GET32(0x1FFFF7AC));
    hexstring(GET32(0x1FFFF7B0));
    hexstring(GET32(0x1FFFF7CC));
    hexstring(GET32(0x40015800));

    //PA0-7 control each segment

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //enable port A
    PUT32(RCC_AHBENR,ra);

    //moder
    ra=GET32(GPIOA_MODER);
    ra&=~(3<< 0); //PA0
    ra|= (1<< 0); //PA0
    ra&=~(3<< 2); //PA1
    ra|= (1<< 2); //PA1
    ra&=~(3<< 4); //PA2
    ra|= (1<< 4); //PA2
    ra&=~(3<< 6); //PA3
    ra|= (1<< 6); //PA3
    ra&=~(3<< 8); //PA4
    ra|= (1<< 8); //PA4
    ra&=~(3<<10); //PA5
    ra|= (1<<10); //PA5
    ra&=~(3<<12); //PA6
    ra|= (1<<12); //PA6
    ra&=~(3<<14); //PA7
    ra|= (1<<14); //PA7
    PUT32(GPIOA_MODER,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<0); //PA0
    ra&=~(1<<1); //PA1
    ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    ra&=~(1<<4); //PA4
    ra&=~(1<<5); //PA5
    ra&=~(1<<6); //PA6
    ra&=~(1<<7); //PA7
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<< 0; //PA0
    ra|=3<< 2; //PA1
    ra|=3<< 4; //PA2
    ra|=3<< 6; //PA3
    ra|=3<< 8; //PA4
    ra|=3<<10; //PA5
    ra|=3<<12; //PA6
    ra|=3<<14; //PA7
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOA_PUPDR);
    ra&=~(3<< 0); //PA0
    ra&=~(3<< 2); //PA1
    ra&=~(3<< 4); //PA2
    ra&=~(3<< 6); //PA3
    ra&=~(3<< 8); //PA4
    ra&=~(3<<10); //PA5
    ra&=~(3<<12); //PA6
    ra&=~(3<<14); //PA7
    PUT32(GPIOA_PUPDR,ra);


    ra=GET32(RCC_AHBENR);
    ra|=1<<19; //enable port C
    PUT32(RCC_AHBENR,ra);

    //pc0-4 select which digit

    //moder
    ra=GET32(GPIOC_MODER);
    ra&=~(3<<0); //PC0
    ra|= (1<<0); //PC0
    ra&=~(3<<2); //PC1
    ra|= (1<<2); //PC1
    ra&=~(3<<4); //PC2
    ra|= (1<<4); //PC2
    ra&=~(3<<6); //PC3
    ra|= (1<<6); //PC3
    PUT32(GPIOC_MODER,ra);
    //OTYPER
    ra=GET32(GPIOCBASE+0x04);
    ra&=~(1<<0); //PC0
    ra&=~(1<<1); //PC1
    ra&=~(1<<2); //PC2
    ra&=~(1<<3); //PC3
    PUT32(GPIOCBASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOCBASE+0x08);
    ra|=3<<0; //PC0
    ra|=3<<2; //PC1
    ra|=3<<4; //PC2
    ra|=3<<6; //PC3
    PUT32(GPIOCBASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOC_PUPDR);
    ra&=~(3<<0); //PC0
    ra&=~(3<<2); //PC1
    ra&=~(3<<4); //PC2
    ra&=~(3<<6); //PC3
    PUT32(GPIOC_PUPDR,ra);

    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,0xFFFFFFFF);
    PUT32(STK_CSR,0x00000005);
    //timer counts down, 24 bit

    //check to see all the numbers are right
    PUT32(GPIOCBASE+0x18,0x0000000F);
    while(1)
    {
        for(ra=0;ra<10;ra++)
        {
            PUT32(GPIOABASE+0x18,pattern[ra]);
            timer_delay(0x400000);
        }
    }

    hexstring(0x12341234);

    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
