
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

//#define GPIOABASE 0x40020000
//#define GPIOA_MODER  (GPIOABASE+0x00)
//#define GPIOA_OTYPER (GPIOABASE+0x04)
//#define GPIOA_AFRH   (GPIOABASE+0x24)
//#define GPIOA_BSRR (GPIOABASE+0x18)

//#define GPIOEBASE 0x40021000
//#define GPIOE_MODER  (GPIOEBASE+0x00)
//#define GPIOE_OTYPER (GPIOEBASE+0x04)
//#define GPIOE_BSRR   (GPIOEBASE+0x18)

//#define RCCBASE         0x40023800
//#define RCC_CR          (RCCBASE+0x00)
//#define RCC_PLLCFGR     (RCCBASE+0x04)
//#define RCC_CFGR        (RCCBASE+0x08)
//#define RCC_AHB1ENR     (RCCBASE+0x30)
//#define RCC_APB2ENR     (RCCBASE+0x44)
//#define RCC_APB2RSTR    (RCCBASE+0x24)

//#define FLASH_BASE  0x40023C00
//#define FLASH_ACR   (FLASH_BASE+0x00)

//#define USART1BASE      0x40011000
//#define USART1_SR       (USART1BASE+0x00)
//#define USART1_DR       (USART1BASE+0x04)
//#define USART1_BRR      (USART1BASE+0x08)
//#define USART1_CR1      (USART1BASE+0x0C)



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
#define RCC_APB1ENR (RCC_BASE+0x1C)
#define RCC_CR      (RCC_BASE+0x00)
#define RCC_CFGR    (RCC_BASE+0x04)
#define FLASH_ACR   0x40022000

#define GPIOCBASE 0x40011000
#define GPIOC_BSRR (GPIOCBASE+0x10)


#define CS      13
#define MOSI    14
#define SCK     15

#define SPI_CS_ON    ( PUT32(GPIOC_BSRR,1<<(CS  + 0)) )
#define SPI_CS_OFF   ( PUT32(GPIOC_BSRR,1<<(CS  +16)) )

#define SPI_CLK_ON   ( PUT32(GPIOC_BSRR,1<<(SCK + 0)) )
#define SPI_CLK_OFF  ( PUT32(GPIOC_BSRR,1<<(SCK +16)) )

#define SPI_MOSI_ON  ( PUT32(GPIOC_BSRR,1<<(MOSI+ 0)) )
#define SPI_MOSI_OFF ( PUT32(GPIOC_BSRR,1<<(MOSI+16)) )


const unsigned char font[16][8]=
{
{
0xFE, //#### ###.
0x82, //#... ..#.
0x82, //#... ..#.
0x82, //#... ..#.
0x82, //#... ..#.
0x82, //#... ..#.
0xFE, //#### ###.
0x00, //.........
},
{
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x00, //.... ....
},
{
0xFE, //#### ###.
0x02, //.... ..#.
0x02, //.... ..#.
0xFE, //#### ###.
0x80, //#... ....
0x80, //#... ....
0xFE, //#### ###.
0x00, //.........
},
{
0xFE, //#### ###.
0x02, //.... ..#.
0x02, //.... ..#.
0xFE, //#### ###.
0x02, //.... ..#.
0x02, //.... ..#.
0xFE, //#### ###.
0x00, //.........
},
{
0x82, //#... ..#.
0x82, //#... ..#.
0x82, //#... ..#.
0xFE, //#### ###.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x00, //.........
},
{
0xFE, //#### ###.
0x80, //#... ....
0x80, //#... ....
0xFE, //#### ###.
0x02, //.... ..#.
0x02, //.... ..#.
0xFE, //#### ###.
0x00, //.........
},
{
0xFE, //#### ###.
0x80, //#... ....
0x80, //#... ....
0xFE, //#### ###.
0x82, //#... ..#.
0x82, //#... ..#.
0xFE, //#### ###.
0x00, //.........
},
{
0xFE, //#### ###.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x00, //.........
},
{
0xFE, //#### ###.
0x82, //#... ..#.
0x82, //#... ..#.
0xFE, //#### ###.
0x82, //#... ..#.
0x82, //#... ..#.
0xFE, //#### ###.
0x00, //.........
},
{
0xFE, //#### ###.
0x82, //#... ..#.
0x82, //#... ..#.
0xFE, //#### ###.
0x02, //.... ..#.
0x02, //.... ..#.
0x02, //.... ..#.
0x00, //.........
},

{
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
},

{
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
},

{
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
},

{
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
},

{
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
},

{
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
0x00, //.... ....
},

};


static void spi_delay ( void )
{
    //unsigned short ra;
    //for(ra=0;ra<10;ra++) dummy(ra);
}

static void spi_write_byte ( unsigned char d )
{
    unsigned char ca;

    for(ca=0x80;ca;ca>>=1)
    {
        if(ca&d) SPI_MOSI_ON; else SPI_MOSI_OFF;
        spi_delay();
        SPI_CLK_ON;
        spi_delay();
        SPI_CLK_OFF;
        spi_delay();
    }
}

static void spi_write_sharp_address ( unsigned char d )
{
    unsigned char ca;

    for(ca=0x01;;ca<<=1)
    {
        if(ca&d) SPI_MOSI_ON; else SPI_MOSI_OFF;
        spi_delay();
        SPI_CLK_ON;
        spi_delay();
        SPI_CLK_OFF;
        spi_delay();
        if(ca==0x80) break;
    }
}

static void clear_screen ( void )
{
    SPI_CS_ON;
    spi_delay();
    spi_write_byte(0x20);
    spi_write_byte(0x00);
    spi_delay();
    SPI_CS_OFF;
}

//PA9  USART1_TX
//PA10 USART1_RX


static void clock_init ( void )
{
    unsigned int ra;

    //enable the external clock
    ra=GET32(RCC_CR);
    ra=ra|1<<16; //HSEON
    PUT32(RCC_CR,ra);

    //wait for HSE to settle
    while(1) if(GET32(RCC_CR)&(1<<17)) break; //HSERDY
    if(0)
    {
        //select HSE clock
        ra=GET32(RCC_CFGR);
        ra&=~(0x3<<0);
        ra|= (0x1<<0);
        PUT32(RCC_CFGR,ra);
        //wait for it
        while(1) if((GET32(RCC_CFGR)&0xF)==0x5) break;
        return;
    }
    //setup pll source settings
    ra=GET32(RCC_CFGR);
    ra&=~(0xF<<18);
    ra|= (0x7<<18);
    ra&=~(1<<17);
    ra|= (0<<17);
    ra&=~(1<<16);
    ra|= (1<<16);
    PUT32(RCC_CFGR,ra);
    //enable the pll
    ra=GET32(RCC_CR);
    ra=ra|1<<24; //PLLON
    PUT32(RCC_CR,ra);
    //wait for pll to lock
    while(1) if(GET32(RCC_CR)&(1<<25)) break; //HSERDY
    //USB div/1.5 AHB not divided, APB not divided, APB1 div/2, MCO, do you need this?
    ra=GET32(RCC_CFGR);
    ra&=~(0x7<<24);
    ra|= (0x7<<24);
    ra&=~(0x1<<22);
    ra|= (0x0<<22);
    ra&=~(0x7<<11);
    ra|= (0x0<<11);
    ra&=~(0x7<<8);
    ra|= (0x4<<8);
    ra&=~(0xF<<4);
    ra|= (0x0<<4);
    PUT32(RCC_CFGR,ra);
    //add wait states for the flash
    PUT32(FLASH_ACR,0x2);
    //switch to the pll as the source
    ra=GET32(RCC_CFGR);
    ra&=~(0x3<<0);
    ra|= (0x2<<0);
    PUT32(RCC_CFGR,ra);
    //wait for it
    while(1) if((GET32(RCC_CFGR)&0xF)==0xA) break;
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
    //72000000/(16*115200) = 39.0625
    //0.0625 * 16 = 1
    //39 1/16 0x27
    //PUT32(USART1_BRR,0x0271);

    //72000000/(16*4800) = 937.5
    //0.5 * 16 = 8
    //937 8/16  0x3A9
    PUT32(USART1_BRR,0x3A98);




    PUT32(USART1_CR1,0x200C);
}
static unsigned int uart_recv ( void )
{
    while(1)
    {
        if(GET32(USART1_SR)&0x20) break;
    }
    return(GET32(USART1_DR)&0xFF);
}





static unsigned char xstring[32];
static unsigned char tstring[32];


void show_time ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;

    for(ra=0;ra<64;ra++)
    {
        spi_delay();
        SPI_CS_ON;
        spi_delay();
        spi_write_byte(0x80);
        spi_write_sharp_address((ra+10+1)&0xFF);
        spi_write_byte(0);
        for(rc=0x80,rb=0;rb<8;rb++,rc>>=1)
        {
            if(font[tstring[0]][ra>>3]&rc) rd=0xFF;
            else rd=0;
            spi_write_byte(rd);
        }
        spi_write_byte(0);
        for(rc=0x80,rb=0;rb<8;rb++,rc>>=1)
        {
            if(font[tstring[1]][ra>>3]&rc) rd=0xFF;
            else rd=0;
            spi_write_byte(rd);
        }
        spi_write_byte(0x00);
        spi_write_byte(0x00);
        spi_delay();
        SPI_CS_OFF;
        spi_delay();
    }
    for(ra=0;ra<64;ra++)
    {
        spi_delay();
        SPI_CS_ON;
        spi_delay();
        spi_write_byte(0x80);
        spi_write_sharp_address((ra+94+1)&0xFF);
        spi_write_byte(0);
        for(rc=0x80,rb=0;rb<8;rb++,rc>>=1)
        {
            if(font[tstring[2]][ra>>3]&rc) rd=0xFF;
            else rd=0;
            spi_write_byte(rd);
        }
        spi_write_byte(0);
        for(rc=0x80,rb=0;rb<8;rb++,rc>>=1)
        {
            if(font[tstring[3]][ra>>3]&rc) rd=0xFF;
            else rd=0;
            spi_write_byte(rd);
        }
        spi_write_byte(0x00);
        spi_write_byte(0x00);
        spi_delay();
        SPI_CS_OFF;
        spi_delay();
    }
}


const unsigned char mod3[16]={0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,0};

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
                    tstring[0]=xstring[0]&0xF;
                    tstring[1]=xstring[1]&0xF;
                    tstring[2]=xstring[2]&0xF;
                    tstring[3]=xstring[3]&0xF;
                    if(tstring[0]==0) tstring[0]=10;

                    show_time();
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

    clock_init();
    uart_init();

    ra=GET32(RCC_APB2ENR);
    ra|=1<<4; //enable port c
    PUT32(RCC_APB2ENR,ra);
    //config
    ra=GET32(GPIOCBASE+0x04);
    ra&=~(3<<20);   //PC13
    ra|=  1<<20;    //PC13
    ra&=~(3<<22);   //PC13
    ra|=  0<<22;    //PC13
    ra&=~(3<<24);   //PC14
    ra|=  1<<24;    //PC14
    ra&=~(3<<26);   //PC14
    ra|=  0<<26;    //PC14
    ra&=~(3<<28);   //PC15
    ra|=  1<<28;    //PC15
    ra&=~(3<<30);   //PC15
    ra|=  0<<30;    //PC15
    PUT32(GPIOCBASE+0x04,ra);


    SPI_CS_OFF  ;
    SPI_CLK_OFF ;
    SPI_MOSI_OFF;


    clear_screen();

    //168x144


    for(ra=0;ra<168;ra++)
    {
        spi_delay();
        SPI_CS_ON;
        spi_delay();
        spi_write_byte(0x80);
        spi_write_sharp_address((ra+1)&0xFF);
        for(rb=0;rb<18;rb++)
        {
            spi_write_byte(0);
        }
        spi_write_byte(0x00);
        spi_write_byte(0x00);
        spi_delay();
        SPI_CS_OFF;
        spi_delay();
    }

    do_nmea();



    return(0);
}


/*

halt
flash write_image erase sharp03_gps_clock/notmain.elf
reset

*/

