

//------------------------------------------------------------------------
//------------------------------------------------------------------------

#define DEBUGUART

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE   0x48000000
#define RCCBASE     0x40021000
#define USART1BASE  0x40013800

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#define GPIOABASE       0x48000000
#define GPIOA_MODER     (GPIOABASE +0x00)
#define GPIOA_OTYPER    (GPIOABASE +0x04)
#define GPIOA_OSPEEDR   (GPIOABASE +0x08)
#define GPIOA_PUPDR     (GPIOABASE +0x0C)
#define GPIOA_IDR       (GPIOABASE +0x10)
#define GPIOA_BSRR      (GPIOABASE +0x18)


#define LED00OUT  (0x1)
#define LED01OUT  (0x1)
#define LED02OUT  (0x1)
#define LED00MODER ((1<<(0<<1))|(1<<(1<<1)))
#define LED01MODER ((1<<(0<<1))|(1<<(3<<1)))
#define LED02MODER ((1<<(0<<1))|(1<<(4<<1)))

#define LED10OUT  (0x2)
#define LED11OUT  (0x2)
#define LED12OUT  (0x2)
#define LED10MODER ((1<<(1<<1))|(1<<(0<<1)))
#define LED11MODER ((1<<(1<<1))|(1<<(3<<1)))
#define LED12MODER ((1<<(1<<1))|(1<<(4<<1)))

#define LED20OUT  (0x8)
#define LED21OUT  (0x8)
#define LED22OUT  (0x8)
#define LED20MODER ((1<<(3<<1))|(1<<(0<<1)))
#define LED21MODER ((1<<(3<<1))|(1<<(1<<1)))
#define LED22MODER ((1<<(3<<1))|(1<<(4<<1)))

#define LED30OUT  (0x10)
#define LED31OUT  (0x10)
#define LED32OUT  (0x10)
#define LED30MODER ((1<<(4<<1))|(1<<(0<<1)))
#define LED31MODER ((1<<(4<<1))|(1<<(1<<1)))
#define LED32MODER ((1<<(4<<1))|(1<<(3<<1)))


//[y][x]
static const unsigned int ledout[4][4]=
{
    {LED00OUT,LED01OUT,LED02OUT,0},
    {LED10OUT,LED11OUT,LED12OUT,0},
    {LED20OUT,LED21OUT,LED22OUT,0},
    {LED30OUT,LED31OUT,LED32OUT,0},
};
static const unsigned int ledmoder[4][4]=
{
    {LED00MODER,LED01MODER,LED02MODER,0},
    {LED10MODER,LED11MODER,LED12MODER,0},
    {LED20MODER,LED21MODER,LED22MODER,0},
    {LED30MODER,LED31MODER,LED32MODER,0},
};

//------------------------------------------------------------------------
int uart_init ( void )
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
void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
    PUT32(USART1BASE+0x28,x);
}
//------------------------------------------------------------------------
unsigned int uart_recv ( void )
{
    while(1) if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    return(GET32(USART1BASE+0x24));
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

//-------------------------------------------------------------------
//  **** WARNING THE CODE BELOW IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------
static volatile unsigned int counter;
static volatile unsigned int rx,ry;
static volatile unsigned int moder;
static volatile unsigned int ten,one,val,bit;
void systick_handler ( void )
{
    if(counter==0)
    {
        PUT32(GPIOABASE+0x00,moder);
        ry++;
        bit<<=1;
        if(ry>3)
        {
            ry=0;
            bit=1;
            rx++;
            if(rx>1)
            {
                rx=0;
                val=one;
            }
            else
            {
                val=ten;
            }
        }
        if(bit&val) counter=5;
        //else counter=2; //others glow
        else counter=1; //others off
    }
    else
    {
        PUT32(GPIOABASE+0x14,ledout[ry][rx]);
        PUT32(GPIOABASE+0x00,moder|ledmoder[ry][rx]);
    }
    counter--;
    GET32(STK_CSR);
}
//-------------------------------------------------------------------
//  **** WARNING THE CODE ABOVE IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------

static int charlie_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    //ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    ra&=~(3<<(4<<1)); //PA4
moder=ra;    
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<0); //PA0
    ra&=~(1<<1); //PA1
    //ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    ra&=~(1<<4); //PA4
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<(0<<1); //PA0
    ra|=3<<(1<<1); //PA1
    //ra|=3<<(2<<1); //PA2
    ra|=3<<(3<<1); //PA3
    ra|=3<<(4<<1); //PA4
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    //ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    ra&=~(3<<(4<<1)); //PA4
    PUT32(GPIOABASE+0x0C,ra);

hexstring(moder);

    //PUT32(STK_CSR,4);
    //PUT32(STK_RVR,100-1);
    //PUT32(STK_CVR,0x00000000);
    //PUT32(STK_CSR,5);
    //moder=GET32(GPIOABASE+0x00);

    PUT32(STK_CSR,4);
    counter=0;
    rx=0;
    ry=0;
    val=0;
    bit=0;
    PUT32(STK_RVR,2000-1);
    PUT32(STK_CVR,0);
    PUT32(STK_CSR,7);
    return(0);
}

//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    
    uart_init();
    hexstring(0x12345678);

    counter=0;

    ten=5;
    one=4;
    val=one;
    charlie_init();


    while(1)
    {
        for(ten=0;ten<10;ten++)
        {
            for(one=0;one<10;one++)
            {
                hexstring((ten<<4)|one);
                for(ra=0;ra<100000;ra++) dummy(ra);
            }
        }
    }
    hexstring(GET32(STK_CSR));
    PUT32(STK_CSR,4);


    hexstring(0x12345678);

    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
