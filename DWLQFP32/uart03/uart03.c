

//------------------------------------------------------------------------
//------------------------------------------------------------------------

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

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
    ra|=10<<18; //times 12
    ra|=0<<15;  //divide by 2
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
int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_BASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCC_BASE+0x14,ra);

    ra=GET32(RCC_BASE+0x18);
    ra|=1<<14; //enable USART1
    PUT32(RCC_BASE+0x18,ra);

    //moder 10
    ra=GET32(GPIOA_BASE+0x00);
    ra&=~(3<<18); //PA9
    ra|=2<<18; //PA9
    ra&=~(3<<20); //PA10
    ra|=2<<20; //PA10
    PUT32(GPIOA_BASE+0x00,ra);
    //OTYPER 0
    ra=GET32(GPIOA_BASE+0x04);
    ra&=~(1<<9); //PA9
    ra&=~(1<<10); //PA10
    PUT32(GPIOA_BASE+0x04,ra);
    //ospeedr 11
    ra=GET32(GPIOA_BASE+0x08);
    ra|=3<<18; //PA9
    ra|=3<<20; //PA10
    PUT32(GPIOA_BASE+0x08,ra);
    //pupdr 00
    ra=GET32(GPIOA_BASE+0x0C);
    ra&=~(3<<18); //PA9
    ra&=~(3<<20); //PA10
    PUT32(GPIOA_BASE+0x0C,ra);
    //afr 0001
    ra=GET32(GPIOA_BASE+0x24);
    ra&=~(0xF<<4); //PA9
    ra|=0x1<<4; //PA9
    ra&=~(0xF<<8); //PA10
    ra|=0x1<<8; //PA10
    PUT32(GPIOA_BASE+0x24,ra);

    ra=GET32(RCC_BASE+0x0C);
    ra|=1<<14; //reset USART1
    PUT32(RCC_BASE+0x0C,ra);
    ra&=~(1<<14);
    PUT32(RCC_BASE+0x0C,ra);

    PUT32(USART1_BASE+0x0C,417);
    PUT32(USART1_BASE+0x08,1<<12);
    PUT32(USART1_BASE+0x00,(1<<3)|(1<<2)|1);

    return(0);
}
//------------------------------------------------------------------------
void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1_BASE+0x1C)&(1<<7)) break;
    PUT32(USART1_BASE+0x28,x);
}
//------------------------------------------------------------------------
unsigned int uart_recv ( void )
{
    while(1) if((GET32(USART1_BASE+0x1C))&(1<<5)) break;
    return(GET32(USART1_BASE+0x24));
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
int notmain ( void )
{
    unsigned int ra;

    clock_init();
    uart_init();
    while(1)
    {
        ra=uart_recv();
        if(ra==0x0D) uart_send(0x0A);
        uart_send(ra);
    }
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
