
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000

#define RCCBASE     0x40021000

#define STK_CSR     0xE000E010
#define STK_RVR     0xE000E014
#define STK_CVR     0xE000E018
#define STK_MASK    0x00FFFFFF

#define USART1BASE  0x40013800

#define RCC_CR      (RCCBASE+0x00)
#define RCC_CFGR    (RCCBASE+0x04)



#define CLK 1
#define DIO 0

static const unsigned int led_table[16]=
{
    0x3F, //0011 1111  0
    0x06, //0000 0110  1
    0x5B, //0101 1011  2
    0x4F, //0100 1111  3
    0x66, //0110 0110  4
    0x6D, //0110 1101  5
    0x7D, //0111 1101  6
    0x07, //0000 0111  7
    0x7F, //0111 1111  8
    0x6F, //0110 1111  9
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};


static void cd_delay ( void )
{
    //unsigned int ra;
    //for(ra=0;ra<100;ra++) dummy(ra);
}

static void clk_high ( void )
{
    PUT32(GPIOABASE+0x18,1<<(CLK+ 0));
}
static void clk_low ( void )
{
    PUT32(GPIOABASE+0x18,1<<(CLK+16));
}
static void dio_high ( void )
{
    PUT32(GPIOABASE+0x18,1<<(DIO+ 0));
}
static void dio_low ( void )
{
    PUT32(GPIOABASE+0x18,1<<(DIO+16));
}
static unsigned int dio_read ( void )
{
    return(GET32(GPIOABASE+0x10)&1<<DIO);
}
static void dio_input ( void )
{
    unsigned int ra;

    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(DIO<<1));
    ra|= (0<<(DIO<<1));
    PUT32(GPIOABASE+0x00,ra);
}
static void dio_output ( void )
{
    unsigned int ra;

    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(DIO<<1));
    ra|= (1<<(DIO<<1));
    PUT32(GPIOABASE+0x00,ra);
}

static void cd_start ( void )
{
    dio_low();
    cd_delay();
}

static void cd_stop ( void )
{
    clk_high();
    cd_delay();
    dio_high();
    cd_delay();
}

static unsigned int cd_byte ( unsigned int data )
{
    unsigned int rm;

    for(rm=0x01;rm<0x100;rm<<=1)
    {
        clk_low();
        cd_delay();
        if(data&rm) dio_high();
        else        dio_low();
        cd_delay();
        clk_high();
        cd_delay();
    }

    clk_low();
    cd_delay();
    dio_input();
    cd_delay();
    clk_high();
    cd_delay();
    rm=dio_read();
    clk_low();
    cd_delay();
    dio_output();
    return(rm);
}










static int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<14; //enable USART1
    PUT32(RCCBASE+0x18,ra);

    //PA9  UART1_TX
    //PA10 UART1_RX

    //moder 10
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<18); //PA9
    ra|=  2<<18; //PA9
    ra&=~(3<<20); //PA10
    ra|=  2<<20; //PA10
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER 0
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<9); //PA9
    ra&=~(1<<10); //PA10
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr 11
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<18; //PA9
    ra|=3<<20; //PA10
    PUT32(GPIOABASE+0x08,ra);
    //pupdr 00
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<18); //PA9
    ra&=~(3<<20); //PA10
    PUT32(GPIOABASE+0x0C,ra);

    //afr 0001
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<4); //PA9
    ra|=0x1<<4; //PA9
    ra&=~(0xF<<8); //PA10
    ra|=0x1<<8; //PA10
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x0C);
    ra|=1<<14; //reset USART1
    PUT32(RCCBASE+0x0C,ra);
    ra&=~(1<<14);
    PUT32(RCCBASE+0x0C,ra);

    //PUT32(USART1BASE+0x0C,1667); //4800
    PUT32(USART1BASE+0x0C,833); //9600
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|(1<<2)|1);

    return(0);
}

static unsigned int uart_recv ( void )
{
    while(1) if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    return(GET32(USART1BASE+0x24));
}

//$GPRMC,000143.00,A,4030.97866,N,07955.13947,W,0.419,,020416,,,A*6E

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
static unsigned int timezone;
//------------------------------------------------------------------------
static int do_nmea ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int state;
    unsigned int off;
    unsigned int sum;
    unsigned int savesum;
    unsigned int xsum;
    unsigned int validity;
    unsigned int num[4];
    unsigned char xstring[32];


    state=0;
    off=0;
//$GPRMC,054033.00,V,
    sum=0;
    savesum=0;
    xsum=0;
    validity=0;
    while(1)
    {
        ra=uart_recv();
        //uart_send(ra);
        //uart_send(0x30+state);
        PUT32(USART1BASE+0x28,ra);

        if(ra!='*') sum^=ra;

        switch(state)
        {
            case 0:
            {
                if(ra=='$') state++;
                else state=0;
                sum=0;
                break;
            }
            case 1:
            {
                if(ra=='G') state++;
                else state=0;
                break;
            }
            case 2:
            {
                if(ra=='P') state++;
                else state=0;
                break;
            }
            case 3:
            {
                if(ra=='R') state++;
                else state=0;
                break;
            }
            case 4:
            {
                if(ra=='M') state++;
                else state=0;
                break;
            }
            case 5:
            {
                if(ra=='C') state++;
                else state=0;
                break;
            }
            case 6:
            {
                off=0;
                if(ra==',') state++;
                else state=0;
                break;
            }
            case 7:
            {
                if(ra==',')
                {
                    if(off>7)
                    {
                        //xstring[0] //tens of hours
                        //xstring[1] //ones of hours
                        //xstring[2] //tens of minutes
                        //xstring[3] //ones of minutes
                        //xstring[4] //tens of seconds
                        //xstring[5] //ones of seconds

                        rb=xstring[0]&0xF;
                        rc=xstring[1]&0xF;
                        //1010
                        rb=/*rb*10*/(rb<<3)+(rb<<1); //times 10
                        rb+=rc;
                        if(rb>12) rb-=12;
                        //ra=5; //time zone adjustment winter
                        //ra=4; //time zone adjustment summer
                        ra=timezone;
                        if(rb<=ra) rb+=12;
                        rb-=ra;
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
                        rb=0;
                        //hour  =((xstring[0]&0xF)*10)+(xstring[1]&0xF);
                        //minute=((xstring[2]&0xF)*10)+(xstring[3]&0xF);
                        //second=((xstring[4]&0xF)*10)+(xstring[5]&0xF);
                        //hexstrings(hour);
                        //hexstrings(minute);
                        //hexstring(second);
                    }
                    off=0;
                    state++;
                }
                else
                {
                    if(off<16)
                    {
                        xstring[off++]=ra;
                    }
                }
                break;
            }
            case 8:
            {
                if(ra=='A') validity=1;
                else validity=0;
                state++;
            }
            case 9:
            {
                if(ra=='*')
                {
                    savesum=sum;
                    state++;
                }
                break;
            }
            case 10:
            {
                if(ra>0x39) ra-=0x37;
                xsum=(ra&0xF)<<4;
                state++;
                break;
            }
            case 11:
            {
                if(ra>0x39) ra-=0x37;
                xsum|=(ra&0xF);
                if(savesum==xsum)
                {
                    num[0]=led_table[xstring[0]&0xF];
                    num[1]=led_table[xstring[1]&0xF];
                    num[2]=led_table[xstring[2]&0xF];
                    num[3]=led_table[xstring[3]&0xF];
                    //herky jerky
                    //if(validity)
                    //{
                        //if(xstring[5]&1) num[1]|=0x80;
                    //}
                    //just have the colon show validity state
                    if(validity)
                    {
                        num[1]|=0x80;
                    }

                    cd_start();
                    cd_byte(0x40);
                    cd_stop();

                    cd_start();
                    cd_byte(0xC0);
                    cd_byte(num[0]);
                    cd_byte(num[1]);
                    cd_byte(num[2]);
                    cd_byte(num[3]);
                    cd_stop();

                }
                for(rb=0;rb<6;rb++) xstring[rb]=0;
                state=0;
                break;
            }
        }
    }
    return(0);
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    timezone=4;
    rb=0x44444444;
    ra=GET32(0x20000D00);
    if(ra==0x44444444)
    {
        timezone=5;
        rb=0x55555555;
    }
    else
    if(ra==0x55555555)
    {
        timezone=4;
        rb=0x44444444;
    }
    PUT32(0x20000D00,rb);

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);


    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(DIO<<1));
    ra|= (1<<(DIO<<1));
    ra&=~(3<<(CLK<<1));
    ra|= (1<<(CLK<<1));
    PUT32(GPIOABASE+0x00,ra);

    //PA0 DATA PA1 CLOCK
    //otyper
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<DIO);
    ra&=~(1<<CLK);
    PUT32(GPIOABASE+0x04,ra);

    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<(DIO<<1));
    ra|= (1<<(DIO<<1)); //pull up
    PUT32(GPIOABASE+0x0C,ra);

    clk_high();
    cd_delay();
    dio_high();
    cd_delay();
    dio_low();
    cd_delay();
    dio_high();
    cd_delay();
    //known state?


    cd_start();
    cd_byte(0x40);
    cd_stop();

    cd_start();
    cd_byte(0xC0);
    cd_byte(led_table[0]);
    cd_byte(led_table[0]);
    cd_byte(led_table[0]);
    cd_byte(led_table[0]);
    cd_stop();

    cd_start();
    if(1)
    {
        cd_byte(0x88);//pulse width 1/16 (dim)
    }
    else
    {
        cd_byte(0x8F);//pulse width 14/16 (bright)
    }
    cd_stop();

    uart_init();
    do_nmea();

    return(0);
}

/*

dfu-util -a 0 -s 0x08000000 -D notmain.bin

*/


