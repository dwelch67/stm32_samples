
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
#define RCC_APB2ENR (RCC_BASE+0x18)

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



#define SCL 0
#define SDA 1


static void DELAY ( void )
{
    //unsigned int ra;
    //for(ra=0;ra<10;ra++) dummy(ra);
}

static void SCL_WRITE ( unsigned int x )
{
    if(x) PUT32(GPIOA_BSRR,(1<<SCL)<< 0);
    else  PUT32(GPIOA_BSRR,(1<<SCL)<<16);
}

static void SDA_WRITE ( unsigned int x )
{
    if(x) PUT32(GPIOA_BSRR,(1<<SDA)<< 0);
    else  PUT32(GPIOA_BSRR,(1<<SDA)<<16);
}

static unsigned int SDA_READ ( void )
{
    return(GET32(GPIOA_IDR)&(1<<SDA));
}

static void SCL_STROBE ( void )
{
    DELAY();
    SCL_WRITE(1);
    DELAY();
    SCL_WRITE(0);
    DELAY();
}

static void SDA_INPUT ( void )
{
    //unsigned int ra;

    //ra=GET32(GPIOA_CRL);
    //ra&=~(0xF<<(SDA<<2));
    //ra|= (0x4<<(SDA<<2));
    //PUT32(GPIOA_CRL,ra);
    PUT32(GPIOA_BSRR,1<<SDA);

    DELAY();
}

static void SDA_OUTPUT ( void )
{
    //unsigned int ra;

    //ra=GET32(GPIOA_CRL);
    //ra&=~(0xF<<(SDA<<2));
    //ra|= (0x1<<(SDA<<2));
    //PUT32(GPIOA_CRL,ra);

    DELAY();
}

static void STOP ( void )
{
    DELAY();
    SCL_WRITE(1);
    DELAY();
    SDA_WRITE(1);
    DELAY();
}

static void START ( void )
{
    DELAY();
    SDA_WRITE(0);
    DELAY();
    SCL_WRITE(0);
    DELAY();
}

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
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int sadd;
    unsigned int temp;

    uart_init();
    hexstring(0x12345678);

    ra=GET32(RCC_APB2ENR);
    ra|=1<<2;   //GPIOA
    PUT32(RCC_APB2ENR,ra);

    //config as open drain output
    ra=GET32(GPIOA_CRL);
    ra&=~(0xF<<(SCL<<2));
    ra|= (0x7<<(SCL<<2));
    ra&=~(0xF<<(SDA<<2));
    ra|= (0x7<<(SDA<<2));
    PUT32(GPIOA_CRL,ra);

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
