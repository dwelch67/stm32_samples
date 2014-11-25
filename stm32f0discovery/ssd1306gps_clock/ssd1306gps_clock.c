
//------------------------------------------------------------------------
//------------------------------------------------------------------------

//#define DEBUG_UART
#define PADDR 0x78


void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE 0x40021000
#define RCC_CR          (RCCBASE+0x00)
#define RCC_CFGR        (RCCBASE+0x04)
#define RCC_APB2RSTR    (RCCBASE+0x0C)
#define RCC_APB1RSTR    (RCCBASE+0x10)
#define RCC_AHBENR      (RCCBASE+0x14)
#define RCC_APB2ENR     (RCCBASE+0x18)
#define RCC_APB1ENR     (RCCBASE+0x1C)

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

#define USART2BASE 0x40004400
#define USART2_CR1      (USART2BASE+0x00)
#define USART2_BRR      (USART2BASE+0x0C)
#define USART2_ISR      (USART2BASE+0x1C)
#define USART2_RDR      (USART2BASE+0x24)
#define USART2_TDR      (USART2BASE+0x28)


//PA9 is USART1_TX alternate function 1
//PA10 is USART1_RX alternate function 1

//PA2 is USART2_TX alternate function 1
//PA3 is USART2_RX alternate function 1


#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF;

unsigned char xstring[16];
unsigned int lasttime;

unsigned int locked;
unsigned int valid;

#define SETCONTRAST         0x81
#define DISPLAYALLONRESUME  0xA4
#define DISPLAYALLON        0xA5
#define NORMALDISPLAY       0xA6
#define INVERTDISPLAY       0xA7
#define DISPLAYOFF          0xAE
#define DISPLAYON           0xAF
#define SETDISPLAYOFFSET    0xD3
#define SETCOMPINS          0xDA
#define SETVCOMDESELECT     0xDB
#define SETDISPLAYCLOCKDIV  0xD5
#define SETPRECHARGE        0xD9
#define SETMULTIPLEX        0xA8
#define SETLOWCOLUMN        0x00
#define SETHIGHCOLUMN       0x10
#define SETSTARTLINE        0x40
#define MEMORYMODE          0x20
#define COMSCANINC          0xC0
#define COMSCANDEC          0xC8
#define SEGREMAP            0xA0
#define CHARGEPUMP          0x8D
#define EXTERNALVCC         0x01
#define SWITCHCAPVCC        0x02


const unsigned char seven_seg[128]=
{
0xF, //  ####
0x9, //  #  #
0x9, //  #  #
0x9, //  #  #
0x9, //  #  #
0x9, //  #  #
0xF, //  ####
0x0, //
0x1, //  ...#
0x1, //  .  #
0x1, //  .  #
0x1, //  ...#
0x1, //  .  #
0x1, //  .  #
0x1, //  ...#
0x0, //
0xF, //  ####
0x1, //  .  #
0x1, //  .  #
0xf, //  ####
0x8, //  #  .
0x8, //  #  .
0xF, //  ####
0x0, //
0xF, //  ####
0x1, //  .  #
0x1, //  .  #
0xF, //  ####
0x1, //  .  #
0x1, //  .  #
0xF, //  ####
0x0, //
0x9, //  #..#
0x9, //  #  #
0x9, //  #  #
0xF, //  ####
0x1, //  .  #
0x1, //  .  #
0x1, //  ...#
0x0, //
0xF, //  ####
0x8, //  #  .
0x8, //  #  .
0xF, //  ####
0x1, //  .  #
0x1, //  .  #
0xF, //  ####
0x0, //
0xF, //  ####
0x8, //  #  .
0x8, //  #  .
0xF, //  ####
0x9, //  #  #
0x9, //  #  #
0xF, //  ####
0x0, //
0xF, //  ####
0x1, //  .  #
0x1, //  .  #
0x1, //  ...#
0x1, //  .  #
0x1, //  .  #
0x1, //  ...#
0x0, //
0xF, //  ####
0x9, //  #  #
0x9, //  #  #
0xF, //  ####
0x9, //  #  #
0x9, //  #  #
0xF, //  ####
0x0, //
0xF, //  ####
0x9, //  #  #
0x9, //  #  #
0xF, //  ####
0x1, //  .  #
0x1, //  .  #
0x1, //  ...#
0x0, //
0xF, //  ####
0x9, //  #  #
0x9, //  #  #
0xF, //  ####
0x9, //  #  #
0x9, //  #  #
0x9, //  #..#
0x0, //
0xE, //  ###.
0x9, //  #  #
0x9, //  #  #
0xE, //  ###.
0x9, //  #  #
0x9, //  #  #
0xE, //  ###.
0x0, //
0xF, //  ####
0x8, //  #  .
0x8, //  #  .
0x8, //  #...
0x8, //  #  .
0x8, //  #  .
0xF, //  ####
0x0, //
0xE, //  ###.
0x9, //  #  #
0x9, //  #  #
0x9, //  #..#
0x9, //  #  #
0x9, //  #  #
0xE, //  ###.
0x0, //
0xF, //  ####
0x8, //  #  .
0x8, //  #  .
0xF, //  ####
0x8, //  #  .
0x8, //  #  .
0xF, //  ####
0x0, //
0xF, //  ####
0x8, //  #  .
0x8, //  #  .
0xF, //  ####
0x8, //  #  .
0x8, //  #  .
0x8, //  #...
0x0, //

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
    //ra&=~(3<<20); //PA10
    ra|=2<<18; //PA9
    //ra|=2<<20; //PA10
    PUT32(GPIOA_MODER,ra);
    //OTYPER 0
    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<9); //PA9
    //ra&=~(1<<10); //PA10
    PUT32(GPIOA_OTYPER,ra);
    //ospeedr 11
    ra=GET32(GPIOA_OSPEEDR);
    ra|=3<<18; //PA9
    //ra|=3<<20; //PA10
    PUT32(GPIOA_OSPEEDR,ra);
    //pupdr 00
    ra=GET32(GPIOA_PUPDR);
    ra&=~(3<<18); //PA9
    //ra&=~(3<<20); //PA10
    PUT32(GPIOA_PUPDR,ra);
    //afr 0001
    ra=GET32(GPIOA_AFRH);
    ra&=~(0xF<<4); //PA9
    //ra&=~(0xF<<8); //PA10
    ra|=0x1<<4; //PA9
    //ra|=0x1<<8; //PA10
    PUT32(GPIOA_AFRH,ra);




    ra=GET32(RCC_APB2RSTR);
    ra|=1<<14; //reset USART1
    PUT32(RCC_APB2RSTR,ra);
    ra&=~(1<<14);
    PUT32(RCC_APB2RSTR,ra);

    PUT32(USART1_BRR,69);
    PUT32(USART1_CR1,(1<<3)/*|(1<<2)*/|1);

    return(0);
}
//------------------------------------------------------------------------
void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1_ISR)&(1<<7)) break;
    PUT32(USART1_TDR,x);
}
////------------------------------------------------------------------------
//unsigned int uart_recv ( void )
//{
    //while(1) if((GET32(USART1_ISR))&(1<<5)) break;
    //return(GET32(USART1_RDR));
//}
////------------------------------------------------------------------------
//unsigned int uart_check ( void )
//{
    //if((GET32(USART1_ISR))&(1<<5)) return(1);
    //return(0);
//}
//------------------------------------------------------------------------
int uart2_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //enable port A
    PUT32(RCC_AHBENR,ra);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<17; //enable USART2
    PUT32(RCC_APB1ENR,ra);

    //moder 10
    ra=GET32(GPIOA_MODER);
    //ra&=~(3<<4); //PA2
    ra&=~(3<<6); //PA3
    //ra|=2<<4; //PA2
    ra|=2<<6; //PA3
    PUT32(GPIOA_MODER,ra);
    //OTYPER 0
    ra=GET32(GPIOA_OTYPER);
    //ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    PUT32(GPIOA_OTYPER,ra);
    //ospeedr 11
    ra=GET32(GPIOA_OSPEEDR);
    //ra|=3<<4; //PA2
    ra|=3<<6; //PA3
    PUT32(GPIOA_OSPEEDR,ra);
    //pupdr 00
    ra=GET32(GPIOA_PUPDR);
    //ra&=~(3<<4); //PA2
    ra&=~(3<<6); //PA3
    PUT32(GPIOA_PUPDR,ra);
    //afr 0001
    ra=GET32(GPIOA_AFRL);
    //ra&=~(0xF<<8); //PA2
    ra&=~(0xF<<12); //PA3
    //ra|=0x1<<8; //PA2
    ra|=0x1<<12; //PA3
    PUT32(GPIOA_AFRL,ra);

//The uart is setup for 115200 baud, 8000000Hz/115200 = 69.4, so basically
//the main clock is divided by 69 and a fraction to get that speed.
//Usually you want to have an additional divide by 16 so that the
//receiver can oversample the input, but we cant get up to 115200 from
//8MHz doing that.  Later with a faster system clock we can do something
//different.

//8000000Hz/9600 = 833.3


    ra=GET32(RCC_APB1RSTR);
    ra|=1<<17; //reset USART2
    PUT32(RCC_APB1RSTR,ra);
    ra&=~(1<<17);
    PUT32(RCC_APB1RSTR,ra);

    PUT32(USART2_BRR,833);
    PUT32(USART2_CR1,/*(1<<3)|*/(1<<2)|1);

    return(0);
}
////------------------------------------------------------------------------
//void uart2_send ( unsigned int x )
//{
    //while(1) if(GET32(USART2_ISR)&(1<<7)) break;
    //PUT32(USART2_TDR,x);
//}
//------------------------------------------------------------------------
unsigned int uart2_recv ( void )
{
    while(1) if((GET32(USART2_ISR))&(1<<5)) break;
    return(GET32(USART2_RDR));
}
//------------------------------------------------------------------------
unsigned int uart2_check ( void )
{
    if((GET32(USART2_ISR))&(1<<5)) return(1);
    return(0);
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
void i2c_delay ( void )
{
    //unsigned int ra;
    //for(ra=0;ra<2;ra++) dummy(ra);
    if(uart2_check()) uart2_recv();
}
//------------------------------------------------------------------------
void scl_high ( void )
{
    PUT32(GPIOCBASE+0x18,0x00000000|(1<<0));
}
//------------------------------------------------------------------------
void sda_high ( void )
{
    PUT32(GPIOCBASE+0x18,0x00000000|(1<<1));
}
//------------------------------------------------------------------------
void scl_low ( void )
{
    PUT32(GPIOCBASE+0x18,0x00000000|((1<<0)<<16));
}
//------------------------------------------------------------------------
void sda_low ( void )
{
    PUT32(GPIOCBASE+0x18,0x00000000|((1<<1)<<16));
}
//------------------------------------------------------------------------
unsigned int sda_read ( void )
{
    unsigned int ra;

    ra=GET32(GPIOCBASE+0x10);
    ra&=(1<<1);
    return(ra);
}
//------------------------------------------------------------------------
void sda_input ( void )
{
    unsigned int ra;
    //moder
    ra=GET32(GPIOC_MODER);
    ra&=~(3<<2); //PC1
    //ra|= (1<<2); //PC1
    PUT32(GPIOC_MODER,ra);
}
//------------------------------------------------------------------------
void sda_output ( void )
{
    unsigned int ra;
    //moder
    ra=GET32(GPIOC_MODER);
    ra&=~(3<<2); //PC1
    ra|= (1<<2); //PC1
    PUT32(GPIOC_MODER,ra);
}
//------------------------------------------------------------------------
void i2c_start ( void )
{
    i2c_delay();
    i2c_delay();
    sda_low();
    i2c_delay();
    i2c_delay();
    scl_low();
    i2c_delay();
}
//------------------------------------------------------------------------
void i2c_stop ( void )
{
    i2c_delay();
    scl_high();
    i2c_delay();
    sda_high();
    i2c_delay();
    i2c_delay();
}
//------------------------------------------------------------------------
unsigned int i2c_write_byte ( unsigned int b )
{
    unsigned int ra;
    for(ra=0x80;ra;ra>>=1)
    {
        i2c_delay();
        if(ra&b) sda_high();
        else     sda_low();
        i2c_delay();
        scl_high();
        i2c_delay();
        scl_low();
        i2c_delay();
        sda_low();
        i2c_delay();
    }
    i2c_delay();
    sda_input();
    i2c_delay();
    scl_high();
    i2c_delay();
    ra=sda_read();
    i2c_delay();
    scl_low();
    i2c_delay();
    sda_output();
    i2c_delay();
    return(ra);
}
//------------------------------------------------------------------------
unsigned int i2c_read_byte ( unsigned int *b )
{
    unsigned int ra;
    unsigned int rb;
    i2c_delay();
    sda_input();
    rb=0;
    for(ra=0;ra<9;ra++)
    {
        i2c_delay();
        scl_high();
        i2c_delay();
        rb<<=1;
        if(sda_read()) rb|=1;
        i2c_delay();
        scl_low();
        i2c_delay();
    }
    sda_output();
    i2c_delay();
    ra=rb&1;
    *b=rb>>1;
    return(ra);
}
//------------------------------------------------------------------------
void send_command ( unsigned int cmd )
{
    i2c_start();
    if(i2c_write_byte(PADDR))
    {
        i2c_stop();
        hexstring(0xBADBAD00);
        return;
    }
    i2c_write_byte(0x00);
    i2c_write_byte(cmd);
    i2c_stop();
}
//------------------------------------------------------------------------
void send_data ( unsigned int data )
{
    i2c_start();
    if(i2c_write_byte(PADDR))
    {
        i2c_stop();
        hexstring(0xBADBAD00);
        return;
    }
    i2c_write_byte(0x40);
    i2c_write_byte(data);
    i2c_stop();
}
//------------------------------------------------------------------------
void SetPageStart ( unsigned int x )
{
    send_command(0xB0|(x&0x07));
}
//------------------------------------------------------------------------
void SetColumn ( unsigned int x )
{
    send_command(0x00|((x>>0)&0x0F));
    send_command(0x10|((x>>4)&0x0F));
}
//------------------------------------------------------------------------
void ClearScreen ( void )
{
    unsigned int ra;
    unsigned int rb;

    for(ra=0;ra<8;ra++)
    {
        SetPageStart(ra);
        SetColumn(0);
        for(rb=0;rb<0x80;rb++) send_data(0);
    }
}
//------------------------------------------------------------------------
void show_time ( void )
{
    unsigned int nowtime;
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    //unsigned int rd;
    unsigned int re;
    unsigned int rf;

    nowtime =xstring[0]&0xF;
    nowtime<<=4;
    nowtime|=xstring[1]&0xF;
    nowtime<<=4;
    nowtime|=xstring[2]&0xF;
    nowtime<<=4;
    nowtime|=xstring[3]&0xF;
    if(nowtime==lasttime) return;

    lasttime=nowtime;

    for(ra=0;ra<7;ra++)
    {
        SetPageStart(ra);
        SetColumn(0);

        if(xstring[0]&0xF) rf=0xFF; else rf=0x00;
        for(re=0;re<8;re++) send_data(rf);

        for(re=0;re<8;re++) send_data(0);

        rc=seven_seg[( (xstring[1]&0xF) <<3)+ra];
        for(rb=0x8;rb;rb>>=1)
        {
            if(rb&rc) rf=0xFF;
            else      rf=0x00;
            for(re=0;re<8;re++) send_data(rf);
        }

        for(re=0;re<8;re++) send_data(0);

        rc=seven_seg[( (xstring[2]&0xF) <<3)+ra];
        for(rb=0x8;rb;rb>>=1)
        {
            if(rb&rc) rf=0xFF;
            else      rf=0x00;
            for(re=0;re<8;re++) send_data(rf);
        }
        for(re=0;re<8;re++) send_data(0);

        rc=seven_seg[( (xstring[3]&0xF) <<3)+ra];

        for(rb=0x8;rb;rb>>=1)
        {
            if(rb&rc) rf=0xFF;
            else      rf=0x00;
            for(re=0;re<8;re++) send_data(rf);
        }
    }
    SetPageStart(ra);
    SetColumn(0);
    if(valid==0) rf=0x80; else rf=0;
    for(rb=0;rb<0x40;rb++) send_data(rf);
    if(locked==0) rf=0x80; else rf=0;
    for(rb=0;rb<0x40;rb++) send_data(rf);
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
    unsigned int rb;
    unsigned int rc;
    unsigned int off;
    unsigned int state;
    unsigned int ncom;

    clock_init();
    uart_init();
    uart2_init();
    hexstrings(0x1234); hexstring(0xABCD);
    hexstring(GET32(0x1FFFF7AC));
    hexstring(GET32(0x1FFFF7B0));
    hexstring(GET32(0x1FFFF7CC));
    hexstring(GET32(0x40015800));

    //i2c outputs

    ra=GET32(RCC_AHBENR);
    ra|=1<<19; //enable port C
    PUT32(RCC_AHBENR,ra);

    //moder
    ra=GET32(GPIOC_MODER);
    ra&=~(3<<0); //PC0
    ra|= (1<<0); //PC0
    ra&=~(3<<2); //PC1
    ra|= (1<<2); //PC1
    PUT32(GPIOC_MODER,ra);
    //OTYPER
    ra=GET32(GPIOCBASE+0x04);
    ra&=~(1<<0); //PC0
    ra&=~(1<<1); //PC1
    PUT32(GPIOCBASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOCBASE+0x08);
    ra|=3<<0; //PC0
    ra|=3<<2; //PC1
    PUT32(GPIOCBASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOC_PUPDR);
    ra&=~(3<<0); //PC0
    ra|= (1<<0); //PC0
    ra&=~(3<<2); //PC1
    ra|= (1<<2); //PC1
    PUT32(GPIOC_PUPDR,ra);


    scl_high();
    sda_high();
    //find who acks
    //for(ra=0;ra<0x100;ra+=2)
    //{
        //hexstrings(ra);
        //i2c_start();
        //hexstring(i2c_write_byte(ra|0));
        //i2c_stop();
    //}
    //0x78 is the address


    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,0xFFFFFFFF);
    PUT32(STK_CSR,0x00000005);
    //timer counts down, 24 bit

    timer_delay(1000000);




    // Display Init sequence for 64x48 OLED module
    send_command(DISPLAYOFF);           // 0xAE
    send_command(SETDISPLAYCLOCKDIV);   // 0xD5
    send_command(0x80);                 // the suggested ratio 0x80
    send_command(SETMULTIPLEX);         // 0xA8
    send_command(0x3F);
    send_command(SETDISPLAYOFFSET);     // 0xD3
    send_command(0x0);                  // no offset
    send_command(SETSTARTLINE | 0x0);   // line #0
    send_command(CHARGEPUMP);           // enable charge pump
    send_command(0x14);
    send_command(MEMORYMODE);
    send_command(0x02);
    send_command(SEGREMAP | 0x1);
    send_command(COMSCANDEC);
    send_command(SETCOMPINS);           // 0xDA
    send_command(0x12);
    send_command(SETCONTRAST);          // 0x81
    send_command(0x7F);
    send_command(SETPRECHARGE);         // 0xd9
    send_command(0xF1);
    send_command(SETVCOMDESELECT);          // 0xDB
    send_command(0x40);
    send_command(NORMALDISPLAY);            // 0xA6
    send_command(DISPLAYALLONRESUME);   // 0xA4
    ClearScreen();
    send_command(DISPLAYON);                //--turn on oled panel
    //send_command(DISPLAYALLON);



    rc=0;
    for(ra=0;ra<7;ra++)
    {
        SetPageStart(ra);
        SetColumn(0);
        for(rb=0;rb<0x80;rb++) send_data(rc++);
    }


hexstring(0x12345678);

    //while(1)
    //{
        //if(uart2_check())
        //{
            //ra=uart2_recv();
            //uart_send(ra);
        //}
    //}

    lasttime=0;


    //toggle_seconds=0;
    valid=0;
    locked=0;
    state=0;
    off=0;
    ncom=0;
//$GPRMC,001412.799,V,,,,,0.00,0.00,060180,,,N*43
//$GPRMC,054033.00,V,
    while(1)
    {
        ra=uart2_recv();
        //uart_send(ra);
        switch(state)
        {
            case 0:
            {
                if(ra==0x0A) state++;
                break;
            }
            case 1:
            {
                if(ra=='$') state++;
                else state=0;
                break;
            }
            case 2:
            {
                if(ra=='G') state++;
                else state=0;
                break;
            }
            case 3:
            {
                if(ra=='P') state++;
                else state=0;
                break;
            }
            case 4:
            {
                if(ra=='R') state++;
                else if(ra=='G') state=100;
                else state=0;
                break;
            }
            case 5:
            {
                if(ra=='M') state++;
                else state=0;
                break;
            }
            case 6:
            {
                if(ra=='C') state++;
                else state=0;
                break;
            }
            case 7:
            {
                off=0;
                ncom=0;
                if(ra==',') state++;
                else state=0;
                break;
            }
            case 8:
            {
                if((ncom)&&(ra==','))
                {
                    valid=0;
                    for(rb=0;rb<off;rb++)
                    {
                        if(xstring[rb]==',')
                        {
                            if(xstring[rb+1]=='A') valid=1;
                            break;
                        }
                    }
                    //012345678901
                    //002329.799,V
                    if(off>7)
                    {
#ifdef DEBUG_UART
                        for(rb=0;rb<off;rb++)
                        {
                            uart_send(xstring[rb]);
                        }
                        uart_send(0x20);
                        uart_send(0x30+valid);
                        uart_send(0x30+locked);
                        uart_send(0x20);
#endif
                        //if((valid)&&(locked))
                        if(1)
                        {

                            //002329.799,V

                            //need to manipulate the hours to adjust for
                            //time zone

                            rb=xstring[0]&0xF;
                            rc=xstring[1]&0xF;
                            //first digit times 10 plus second is hours gmt
                            //1010
                            rb=/*rb*10*/(rb<<3)+(rb<<1); //times 10
                            rb+=rc;
                            //adjust for 24 hour clock
                            if(rb>12) rb-=12;
                            //======================================
                            ra=5; //time zone adjustment
                            if(rb<=ra) rb+=12;
                            rb-=ra;
                            //======================================
                            //divide by 10 first digit is either 1 or not
                            if(rb>9)
                            {
                                xstring[0]='1';
                                rb-=10;
                            }
                            else
                            {
                                xstring[0]='0';
                            }
                            rb&=0xF;
                            xstring[1]=0x30+rb;
                            xstring[4]=0;
                        }
                        else
                        {
                            xstring[0]=0x31;
                            xstring[1]=0x39;
                            xstring[2]=0x37;
                            xstring[3]=0x30|(valid)|(locked<<1);
                            xstring[4]=0;
                        }
                    }
                    else
                    {
                        xstring[0]=0x31;
                        xstring[1]=0x39;
                        xstring[2]=0x39;
                        xstring[3]=0x39;
                        xstring[4]=0;
                    }
#ifdef DEBUG_UART
                    for(ra=0;ra<4;ra++)
                    {
                        uart_send(xstring[ra]);
                    }
                    uart_send(0x0D);
                    uart_send(0x0A);
#endif
                    show_time();
                    off=0;
                    state++;
                }
                else
                {
                    if(ra==',') ncom++;
                    if(off<16)
                    {
                        xstring[off++]=ra;
                    }
                }
                break;
            }
            case 9:
            {
                state=0;
                //if(zstring[off]==0)
                //{
                    //state=0;
                //}
                //else
                //{
                    //uart_send(zstring[off++]);
                //}
                break;
            }
//$GPGSA,A,1,,,,,,,,,,,,,,,*1E
            case 100:
            {
                if(ra=='S') state++;
                else state=0;
                break;
            }
            case 101:
            {
                if(ra=='A') state++;
                else state=0;
                break;
            }
            case 102:
            {
                if(ra==',') state++;
                else state=0;
                break;
            }
            case 103:
            {
                if(ra=='A') state++;
                else state=0;
                break;
            }
            case 104:
            {
                if(ra==',') state++;
                else state=0;
                break;
            }
            case 105:
            {
                switch(ra)
                {
                    default:
                    case 0x31:
                        locked=0;
                        break;
                    case 0x32:
                    case 0x33:
                        locked=1;
                        break;
                }
                //hexstring(ra|(locked<<16));
                state=0;
                break;
            }
        }
    }

//$GPGSA,A,1,,,,,,,,,,,,,,,*1E
//$GPRMC,001412.799,V,,,,,0.00,0.00,060180,,,N*43
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
