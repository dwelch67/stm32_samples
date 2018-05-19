

//------------------------------------------------------------------------
//------------------------------------------------------------------------

#define DEBUGUART

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE     0x40021000
#define USART1BASE  0x40013800

#define GPIOABASE       0x48000000
#define GPIOA_MODER     (GPIOABASE+0x00)
#define GPIOA_OTYPER    (GPIOABASE+0x04)
#define GPIOA_OSPEEDR   (GPIOABASE+0x08)
#define GPIOA_PUPDR     (GPIOABASE+0x0C)
#define GPIOA_IDR       (GPIOABASE+0x10)
#define GPIOA_BSRR      (GPIOABASE+0x18)

//------------------------------------------------------------------------
static int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<14; //enable USART1
    PUT32(RCCBASE+0x18,ra);

#ifdef DEBUGUART
    //PA2 UART1_TX
    //PA15 UART1_RX
    //moder 10
    ra=GET32(GPIOA_MODER);
    ra&=~(3<<4); //PA2
    ra|=2<<4; //PA2
    ra&=~(3<<30); //PA15
    ra|=2<<30; //PA15
    PUT32(GPIOA_MODER,ra);
    //OTYPER 0
    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<2); //PA2
    ra&=~(1<<15); //PA2
    PUT32(GPIOA_OTYPER,ra);
    //ospeedr 11
    ra=GET32(GPIOA_OSPEEDR);
    ra|=3<<4; //PA2
    ra|=3<<30; //PA15
    PUT32(GPIOA_OSPEEDR,ra);
    //pupdr 00
    ra=GET32(GPIOA_PUPDR);
    ra&=~(3<<4); //PA2
    ra&=~(3<<30); //PA15
    PUT32(GPIOA_PUPDR,ra);
    //afr 0001
    ra=GET32(GPIOABASE+0x20);
    ra&=~(0xF<<8); //PA2
    ra|=0x1<<8; //PA2
    PUT32(GPIOABASE+0x20,ra);
    //afr 0001
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<28); //PA15
    ra|=0x1<<28; //PA15
    PUT32(GPIOABASE+0x24,ra);

#else
    //PA9 UART1_TX
    //PA10 UART1_RX

    //moder 10
    ra=GET32(GPIOA_MODER);
    ra&=~(3<<18); //PA9
    ra|=2<<18; //PA9
    ra&=~(3<<20); //PA10
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
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<4); //PA9
    ra|=0x1<<4; //PA9
    ra&=~(0xF<<8); //PA10
    ra|=0x1<<8; //PA10
    PUT32(GPIOABASE+0x24,ra);

#endif

    ra=GET32(RCCBASE+0x0C);
    ra|=1<<14; //reset USART1
    PUT32(RCCBASE+0x0C,ra);
    ra&=~(1<<14);
    PUT32(RCCBASE+0x0C,ra);

    PUT32(USART1BASE+0x0C,69);
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|(1<<2)|1);

    return(0);
}
//------------------------------------------------------------------------
static void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
    PUT32(USART1BASE+0x28,x);
}
////------------------------------------------------------------------------
//unsigned int uart_recv ( void )
//{
    //while(1) if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    //return(GET32(USART1BASE+0x24));
//}
//------------------------------------------------------------------------
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
//------------------------------------------------------------------------
static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
//------------------------------------------------------------------------

#define SCL 0
#define SDA 1

//------------------------------------------------------------------------
static void DELAY ( void )
{
    //unsigned int ra;
    //for(ra=0;ra<10;ra++) dummy(ra);
}
//------------------------------------------------------------------------
static void SCL_WRITE ( unsigned int x )
{
    if(x) PUT32(GPIOA_BSRR,(1<<SCL)<< 0);
    else  PUT32(GPIOA_BSRR,(1<<SCL)<<16);
}
//------------------------------------------------------------------------
static void SDA_WRITE ( unsigned int x )
{
    if(x) PUT32(GPIOA_BSRR,(1<<SDA)<< 0);
    else  PUT32(GPIOA_BSRR,(1<<SDA)<<16);
}
//------------------------------------------------------------------------
static unsigned int SDA_READ ( void )
{
    return(GET32(GPIOA_IDR)&(1<<SDA));
}
//------------------------------------------------------------------------
static void SCL_STROBE ( void )
{
    DELAY();
    SCL_WRITE(1);
    DELAY();
    SCL_WRITE(0);
    DELAY();
}
//------------------------------------------------------------------------
static void SDA_INPUT ( void )
{
    unsigned int ra;
    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(SDA<<1));
    //ra|= (1<<(SDA<<1));
    PUT32(GPIOA_MODER,ra);
    DELAY();
}
//------------------------------------------------------------------------
static void SDA_OUTPUT ( void )
{
    unsigned int ra;
    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(SDA<<1)); //dont need this
    ra|= (1<<(SDA<<1));
    PUT32(GPIOA_MODER,ra);
    DELAY();
}
//------------------------------------------------------------------------
static void STOP ( void )
{
    DELAY();
    SCL_WRITE(1);
    DELAY();
    SDA_WRITE(1);
    DELAY();
}
//------------------------------------------------------------------------
static void START ( void )
{
    DELAY();
    SDA_WRITE(0);
    DELAY();
    SCL_WRITE(0);
    DELAY();
}
//------------------------------------------------------------------------
static unsigned int WRITE_BYTE ( unsigned int x )
{
    unsigned int ra;
    unsigned int rb;

    for(ra=0x80;ra;ra>>=1)
    {
        SDA_WRITE(ra&x);
        SCL_STROBE();
    }
    SDA_INPUT();
    SCL_STROBE();
    rb=SDA_READ();
    SDA_OUTPUT();
    return(rb);
}
//------------------------------------------------------------------------
static unsigned int READ_BYTE ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rc=0;
    SDA_INPUT();
    rb=0x80;
    for(ra=0;ra<8;ra++)
    {
        if(SDA_READ()) rc|=rb;
        SCL_STROBE();
        rb>>=1;
    }
    SDA_OUTPUT();
    SDA_WRITE(0);
    SCL_STROBE();
    return(rc);
}
//------------------------------------------------------------------------
static unsigned int READ_REGISTER ( unsigned int sadd, unsigned int x)
{
    unsigned int rc;

    START();
    WRITE_BYTE(sadd|0);
    WRITE_BYTE(x);
    STOP();

    START();
    WRITE_BYTE(sadd|1);
    rc=READ_BYTE();
    rc<<=8;
    rc|=READ_BYTE();
    STOP();
    return(rc);
}
////------------------------------------------------------------------------
//static unsigned int read_register ( unsigned int sadd, unsigned int x )
//{
//    unsigned int ra;
//    unsigned int rb;
//    unsigned int rc;
//
//    START();
//    ra=sadd|0;
//    for(rb=0x80;rb;rb>>=1)
//    {
//        SDA_WRITE(ra&rb);
//        SCL_STROBE();
//    }
//    SDA_INPUT();
//    SCL_STROBE();
//    //SDA_READ();
//    SDA_OUTPUT();
//    ra=x;
//    for(rb=0x80;rb;rb>>=1)
//    {
//        SDA_WRITE(ra&rb);
//        SCL_STROBE();
//    }
//    SDA_INPUT();
//    SCL_STROBE();
//    //SDA_READ();
//    SDA_OUTPUT();
//    STOP();
//
//    START();
//    ra=sadd|1;
//    for(rb=0x80;rb;rb>>=1)
//    {
//        SDA_WRITE(ra&rb);
//        SCL_STROBE();
//    }
//    SDA_INPUT();
//    SCL_STROBE();
//    rb=0x8000;
//    rc=0;
//    for(ra=0;ra<8;ra++)
//    {
//        if(SDA_READ()) rc|=rb;
//        SCL_STROBE();
//        rb>>=1;
//    }
//    SDA_OUTPUT();
//    SDA_WRITE(0);
//    SCL_STROBE();
//    SDA_INPUT();
//    for(ra=0;ra<8;ra++)
//    {
//        if(SDA_READ()) rc|=rb;
//        SCL_STROBE();
//        rb>>=1;
//    }
//    SDA_OUTPUT();
//    SDA_WRITE(0);
//    SCL_STROBE();
//    STOP();
//    return(rc);
//}

static unsigned int base_ten ( unsigned int x )
{
    unsigned int rb;
    unsigned int num,res,acc,den;

    num=x;
    res=0;
    acc=0;
    den=10;
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
    return((res<<4)|acc);
    //return(0);
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int sadd;
    unsigned int temp;

    uart_init();
    hexstring(0x12345678);

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(SCL<<1));
    ra&=~(3<<(SDA<<1));
    ra|= (1<<(SCL<<1));
    ra|= (1<<(SDA<<1));
    PUT32(GPIOA_MODER,ra);
    //OTYPER
    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<SCL);
    ra&=~(1<<SDA);
    PUT32(GPIOA_OTYPER,ra);
    ////ospeedr
    //ra=GET32(GPIOA_OSPEEDR);
    //ra|= (3<<(SCL<<1));
    //ra|= (3<<(SDA<<1));
    //PUT32(GPIOA_OSPEEDR,ra);
    //pupdr
    ra=GET32(GPIOA_PUPDR);
    ra&=~(3<<(SCL<<1));
    ra&=~(3<<(SDA<<1));
    ra|= (1<<(SDA<<1));  //PULL UP
    PUT32(GPIOA_PUPDR,ra);

    START();
    STOP();

    //for(sadd=0x00;sadd<0xFF;sadd+=2)
    //{
        //START();
        //ra=WRITE_BYTE(sadd|1);
        //STOP();
        //hexstrings(sadd); hexstring(ra);
    //}
    //00000048 00000000
    sadd=0x90;
    ra=READ_REGISTER(sadd,1); hexstring(ra);
    ra=READ_REGISTER(sadd,0); hexstring(ra);
    while(1)
    {
        ra=READ_REGISTER(sadd,0);
        hexstrings(ra); hexstrings(ra>>4);
        if(ra&0x8000)
        {
            temp=0;
        }
        else
        {
            rb=(ra>>4)*7373;
            rb>>=16;
            rb+=32;
            hexstrings(rb);
            if(rb>99) rb=99;
            temp=base_ten(rb);
        }
        hexstring(temp);
        for(ra=0;ra<0x80000;ra++) dummy(ra);
    }

    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
