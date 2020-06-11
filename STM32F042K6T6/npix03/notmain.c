
void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
void PUT8  ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET8 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE   0x48000000
#define RCCBASE     0x40021000
#define USART1BASE  0x40013800
#define SPI1BASE    0x40013000


#include "lookup.h"

void update_time ( unsigned int h, unsigned int m, unsigned int s );

static int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<14; //enable USART1
    PUT32(RCCBASE+0x18,ra);

    //PA9  UART1_TX NOT USING THIS, NEED IT FOR A GPIO
    //PA10 UART1_RX

    //moder 10
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<( 9<<1)); //PA9
    ra|=  2<<( 9<<1); //PA9
    ra&=~(3<<(10<<1)); //PA10
    ra|=  2<<(10<<1); //PA10
    PUT32(GPIOABASE+0x00,ra);

    //afr 0001
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<4); //PA9
    ra|=  0x1<<4;  //PA9
    ra&=~(0xF<<8); //PA10
    ra|=  0x1<<8;  //PA10
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x0C);
    ra|=1<<14; //reset USART1
    PUT32(RCCBASE+0x0C,ra);
    ra&=~(1<<14);
    PUT32(RCCBASE+0x0C,ra);

    //PUT32(USART1BASE+0x0C,1667); //4800 8MHZ
//    PUT32(USART1BASE+0x0C,833); //9600 8MHZ
    PUT32(USART1BASE+0x0C,2083); //9600 20MHZ
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|(1<<2)|1); //not TE

    return(0);
}

static unsigned int uart_recv ( void )
{
    while(1) if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    return(GET32(USART1BASE+0x24));
}

unsigned int last_tim[6];

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
    unsigned int num[6];
    unsigned char xstring[32];


    state=0;
    off=0;
//$GPRMC,054033.00,V,
    sum=0;
    savesum=0;
    xsum=0;
    while(1)
    {
        ra=uart_recv();
        //uart_send(ra);
        //uart_send(0x30+state);
        //PUT32(USART1BASE+0x28,ra);

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
                if(ra=='G') state++;
                else state=0;
                break;
            }
            case 4:
            {
                if(ra=='G') state++;
                else state=0;
                break;
            }
            case 5:
            {
                if(ra=='A') state++;
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
            //could check for A vs V here...
            case 8:
            {
                if(ra=='*')
                {
                    savesum=sum;
                    state++;
                }
                break;
            }
            case 9:
            {
                if(ra>0x39) ra-=0x37;
                xsum=(ra&0xF)<<4;
                state++;
                break;
            }
            case 10:
            {
                if(ra>0x39) ra-=0x37;
                xsum|=(ra&0xF);
                if(savesum==xsum)
                {
                    num[0]=xstring[0]&0xF;
                    num[1]=xstring[1]&0xF;
                    num[2]=xstring[2]&0xF;
                    num[3]=xstring[3]&0xF;
                    num[4]=xstring[4]&0xF;
                    num[5]=xstring[5]&0xF;

                    rb=0;
                    if(num[0]!=last_tim[0]) rb++;
                    if(num[1]!=last_tim[1]) rb++;
                    if(num[2]!=last_tim[2]) rb++;
                    if(num[3]!=last_tim[3]) rb++;
                    if(num[4]!=last_tim[4]) rb++;
                    if(num[5]!=last_tim[5]) rb++;
                    if(rb)
                    {
                        last_tim[0]=num[0];
                        last_tim[1]=num[1];
                        last_tim[2]=num[2];
                        last_tim[3]=num[3];
                        last_tim[4]=num[4];
                        last_tim[5]=num[5];

                        update_time(
                        (last_tim[0]*10)+last_tim[1],
                        (last_tim[2]*10)+last_tim[3],
                        (last_tim[4]*10)+last_tim[5]
                        );
                    }
                }
                for(rb=0;rb<6;rb++) xstring[rb]=0;
                state=0;
                break;
            }
        }
    }
    return(0);
}

void clock_init ( void )
{
    unsigned int ra;

    //HSI8 based 20MHz.  (8/2)*5
    ra=0;
    ra|=0x3<<18; //times 5
    ra|=0<<15;  //divide by 2
    PUT32(RCCBASE+0x04,ra);
    ra=GET32(RCCBASE+0x00);
    ra|=1<<24;
    PUT32(RCCBASE+0x00,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x00);
        if(ra&(1<<25)) break;
    }
    ra=GET32(RCCBASE+0x04);
    ra&=~3;
    ra|=2;
    PUT32(RCCBASE+0x04,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x04);
        if((ra&3)==2) break;
    }
}

void fifo_wait ( void )
{
    while(1)
    {
        if((GET16(SPI1BASE+0x08)&(1<<12))==0) break;
    }
}

unsigned short data[512];
unsigned int off;
unsigned int bit;

void add_bit ( unsigned int x )
{
    data[off]<<=1;
    data[off]|=(x&1);
    bit++;
    if(bit==16)
    {
        off++;
        bit=0;
    }
}

void add_one_color ( unsigned int c )
{
    unsigned int ra;
    for(ra=0x80;ra;ra>>=1)
    {
        if(c&ra)
        {
            add_bit(1);
            add_bit(1);
            add_bit(0);
        }
        else
        {
            add_bit(1);
            add_bit(0);
            add_bit(0);
        }
    }
}

void add_colors ( unsigned int g, unsigned int r, unsigned int b )
{
    add_one_color(g);
    add_one_color(r);
    add_one_color(b);
}

void run_it ( void )
{
    unsigned int ra;

    PUT16(SPI1BASE+0x0C,0x0000);
    for(ra=0;ra<off;ra++)
    {
        fifo_wait();
        PUT16(SPI1BASE+0x0C,data[ra]);
    }
    PUT16(SPI1BASE+0x0C,0x0000);
    //for(ra=0;ra<40000;ra++) dummy(ra);
}

void round_off ( void )
{
            //add_bit(1);
            add_bit(0);
    while(bit!=0) add_bit(0);
}

void update_time ( unsigned int h, unsigned int m, unsigned int s )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int xb;
    unsigned int xr;
    unsigned int xg;

    if(h>11) h=0;
    if(m>59) m=0;
    if(s>59) s=0;

    ra=h<<1; if(m>29) ra++;
    rb=m<<1; if(s>29) rb++;
    rb=lot[rb];

    off=0;
    bit=0;

    data[off++]=0x0000;
    for(rc=0;rc<24;rc++)
    {
        xr=0;
        xb=0;
        xg=0;
        if(rc==ra) xr=1;
        if(rc==rb) xb=1;
        if((xr==0)&&(xb==0)) xg=1;
        add_colors(xg,xr,xb);
    }
    if(bit==16)
    {
        off++;
        bit=0;
    }
    while(bit!=0) add_bit(0);
    data[off++]=0x0000;
    run_it();
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    clock_init();

//PA4 AF0 SPI1_NSS
//PA5 AF0 SPI1_SCK
//PA6 AF0 SPI1_MISO
//PA7 AF0 SPI1_MOSI <---

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    //MODER
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(7<<1)); //PA7
    ra|= (2<<(7<<1)); //PA7 alternate function
    PUT32(GPIOABASE+0x00,ra);
    //AFRL
    ra=GET32(GPIOABASE+0x20);
    ra&=~(0xF<<(7<<2)); //PA7
    ra|= (0x0<<(7<<2)); //PA7
    PUT32(GPIOABASE+0x20,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<12; //enable spi1
    PUT32(RCCBASE+0x18,ra);

    PUT16(SPI1BASE+0x00,(0<<6)|(2<<3)|(1<<2)|0);
    PUT16(SPI1BASE+0x04,0x0F04);
    PUT16(SPI1BASE+0x00,(1<<6)|(2<<3)|(1<<2)|0);

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


    uart_init();
    //hexstring(0x12345678);
    do_nmea();

    last_tim[0]=9;
    last_tim[1]=9;
    last_tim[2]=9;
    last_tim[3]=9;
    last_tim[4]=9;
    last_tim[5]=9;

    return(0);
}



/*

dfu-util -a 0 -s 0x08000000 -D notmain.bin

*/
