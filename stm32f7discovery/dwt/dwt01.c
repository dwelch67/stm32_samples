
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x40020000
#define GPIOIBASE 0x40022000
#define RCCBASE 0x40023800

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#define USART1BASE 0x40011000

#define  DWT            0xE00FF004
//0xFFF02003


#define  DWT_CONTROL    0xE0001000
#define  DWT_CYCCNT     0xE0001004
//0xE0001008
#define SCB_DEMCR 0xE000EDFC


//------------------------------------------------------------------------
int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x30);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x30,ra);

    ra=GET32(RCCBASE+0x44);
    ra|=1<<4; //enable USART1
    PUT32(RCCBASE+0x44,ra);

    //moder 10
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<18); //PA9
    ra|=2<<18; //PA9
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER 0
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<9); //PA9
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr 11
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<18; //PA9
    PUT32(GPIOABASE+0x08,ra);
    //pupdr 00
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<18); //PA9
    PUT32(GPIOABASE+0x0C,ra);
    //afr 0111
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<4); //PA9
    ra|=0x7<<4; //PA9
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x24);
    ra|=1<<4; //reset USART1
    PUT32(RCCBASE+0x24,ra);
    ra&=~(1<<4);
    PUT32(RCCBASE+0x24,ra);

    PUT32(USART1BASE+0x0C,139);
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|1);

    return(0);
}
//------------------------------------------------------------------------
void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
    PUT32(USART1BASE+0x28,x);
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
    //unsigned int ra;
    //unsigned int rb;

    uart_init();
    hexstring(0x12345678);

    ////The DWT unit suspends CYCCNT counting when the processor is in Debug state.
    //ra=GET32(DWT); hexstring(ra);
    //ra=GET32(DWT_CONTROL); hexstring(ra);
    //ra=GET32(DWT_CYCCNT); hexstring(ra);
    //ra=GET32(DWT_CYCCNT); hexstring(ra);
    //for(ra=0xE00FFFD0;ra<=0xE00FFFFC;ra+=4)
    //{
        //hexstrings(ra);
        //hexstring(GET32(ra));

    //}
    //for(ra=0xE00FF000;ra<=0xE00FF018;ra+=4)
    //{
        //hexstrings(ra);
        //hexstring(GET32(ra));
    //}
    //for(ra=0xE00FFFCC;ra<=0xE00FFFCC;ra+=4)
    //{
        //hexstrings(ra);
        //hexstring(GET32(ra));
    //}
    //for(ra=0xE000EFD0;ra<=0xE000EFFC;ra+=4)
    //{
        //hexstrings(ra);
        //hexstring(GET32(ra));
    //}
    //for(ra=0xE0001FD0;ra<=0xE0001FFC;ra+=4)
    //{
        //hexstrings(ra);
        //hexstring(GET32(ra));
    //}
    //hexstrings(SCB_DEMCR); hexstring(GET32(SCB_DEMCR));
    //PUT32(SCB_DEMCR,0x01000000);
    //hexstrings(SCB_DEMCR); hexstring(GET32(SCB_DEMCR));
    //PUT32(DWT_CONTROL,0x40000001);
    //ra=GET32(DWT_CONTROL); hexstring(ra);
    //ra=GET32(DWT_CYCCNT); hexstring(ra);
    //ra=GET32(DWT_CYCCNT); hexstring(ra);
    //hexstrings(0xE0000FB4); hexstring(GET32(0xE0000FB4));
    //hexstrings(0xE0001FB4); hexstring(GET32(0xE0001FB4));
    //hexstrings(0xE0002FB4); hexstring(GET32(0xE0002FB4));
    //hexstrings(0xE0003FB4); hexstring(GET32(0xE0003FB4));
    //hexstrings(0xE0000FB4); hexstring(GET32(0xE0000FB4));
    //hexstrings(0xE0001FB4); hexstring(GET32(0xE0001FB4));
    //hexstrings(0xE0002FB4); hexstring(GET32(0xE0002FB4));
    //hexstrings(0xE0003FB4); hexstring(GET32(0xE0003FB4));

    hexstring(GET32(0xE0001FB4));
    hexstring(GET32(0xE0001000));
    hexstring(GET32(0xE0001004));
    hexstring(GET32(0xE0001004));

    PUT32(0xE000EDFC,0x01000000);

    hexstring(GET32(0xE0001FB4));
    hexstring(GET32(0xE0001000));
    hexstring(GET32(0xE0001004));
    hexstring(GET32(0xE0001004));

    PUT32(0xE0001000,0x40000001);

    hexstring(GET32(0xE0001FB4));
    hexstring(GET32(0xE0001000));
    hexstring(GET32(0xE0001004));
    hexstring(GET32(0xE0001004));

    PUT32(0xE0001FB0,0xC5ACCE55);
    PUT32(0xE0001000,0x40000001);

    hexstring(GET32(0xE0001FB4));
    hexstring(GET32(0xE0001000));
    hexstring(GET32(0xE0001004));
    hexstring(GET32(0xE0001004));

   return(0);
}
