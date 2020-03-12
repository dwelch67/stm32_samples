

//comment or uncomment next line
//#define OLED128X32


#ifndef OLED128X32
#define OLED128X64
#endif


void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define GPIOA_MODER (GPIOABASE+0x00)

#define GPIOBBASE 0x48000400
#define GPIOB_MODER     (GPIOBBASE+0x00)
#define GPIOB_OTYPER    (GPIOBBASE+0x04)
#define GPIOB_OSPEEDR   (GPIOBBASE+0x08)
#define GPIOB_PUPDR     (GPIOBBASE+0x0C)
#define GPIOB_IDR       (GPIOBBASE+0x10)
#define GPIOB_BSRR      (GPIOBBASE+0x18)


#define RCCBASE     0x40021000

#define STK_CSR     0xE000E010
#define STK_RVR     0xE000E014
#define STK_CVR     0xE000E018
#define STK_MASK    0x00FFFFFF

#define USART1BASE  0x40013800

#define RCC_CR      (RCCBASE+0x00)
#define RCC_CFGR    (RCCBASE+0x04)

#define PADDR 0x78


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


const unsigned char dfont[10][4][16]=
{
//
//
// **************
//* ************ *
//** ********** **
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//**            **
//
//**            **
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//** ********** **
//* ************ *
// **************
//
//
//
{
  {0xF8,0xF4,0xEC,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0xEC,0xF4,0xF8},
  {0x7F,0x7F,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x7F,0x7F},
  {0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0xFF},
  {0x0F,0x17,0x1B,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1B,0x17,0x0F},
},
//
//
//
//               *
//              **
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//              **
//
//              **
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//              **
//               *
//
//
//
//
{
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xF0,0xF8},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x7F,0x7F},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0xFF},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x0F},
},
//
//
// **************
//  ************ *
//   ********** **
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//   ********** **
//  ************
//** **********
//***
//***
//***
//***
//***
//***
//***
//***
//***
//** **********
//* ************
// **************
//
//
//
{
  {0x00,0x04,0x0C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0xEC,0xF4,0xF8},
  {0x00,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xBF,0x7F,0x7F},
  {0xFF,0xFF,0xFE,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00},
  {0x0F,0x17,0x1B,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x18,0x10,0x00},
},
//
//
// **************
//  ************ *
//   ********** **
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//   ********** **
//  ************
//   ********** **
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//   ********** **
//  ************ *
// **************
//
//
//
{
  {0x00,0x04,0x0C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0xEC,0xF4,0xF8},
  {0x00,0x00,0x80,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xBF,0x7F,0x7F},
  {0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFE,0xFF,0xFF},
  {0x00,0x10,0x18,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1B,0x17,0x0F},
},
//
//
//
//*              *
//**            **
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//** ********** **
//  ************
//   ********** **
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//              **
//               *
//
//
//
//
{
  {0xF8,0xF0,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xF0,0xF8},
  {0x7F,0x7F,0xBF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xBF,0x7F,0x7F},
  {0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFE,0xFF,0xFF},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x0F},
},
//
//
// **************
//* ************
//** **********
//***
//***
//***
//***
//***
//***
//***
//***
//***
//** **********
//  ************
//   ********** **
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//   ********** **
//  ************ *
// **************
//
//
//
{
  {0xF8,0xF4,0xEC,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x0C,0x04,0x00},
  {0x7F,0x7F,0xBF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00},
  {0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFE,0xFF,0xFF},
  {0x00,0x10,0x18,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1B,0x17,0x0F},
},
//
//
// **************
//* ************
//** **********
//***
//***
//***
//***
//***
//***
//***
//***
//***
//** **********
//  ************
//** ********** **
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//** ********** **
//* ************ *
// **************
//
//
//
{
  {0xF8,0xF4,0xEC,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x0C,0x04,0x00},
  {0x7F,0x7F,0xBF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0x00,0x00},
  {0xFF,0xFF,0xFE,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFE,0xFF,0xFF},
  {0x0F,0x17,0x1B,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1B,0x17,0x0F},
},
//
//
// **************
//  ************ *
//   ********** **
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//              **
//
//              **
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//              **
//               *
//
//
//
//
{
  {0x00,0x04,0x0C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0xEC,0xF4,0xF8},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x7F,0x7F},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0xFF},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x07,0x0F},
},
//
//
// **************
//* ************ *
//** ********** **
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//** ********** **
//  ************
//** ********** **
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//** ********** **
//* ************ *
// **************
//
//
//
{
  {0xF8,0xF4,0xEC,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0xEC,0xF4,0xF8},
  {0x7F,0x7F,0xBF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xBF,0x7F,0x7F},
  {0xFF,0xFF,0xFE,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFE,0xFF,0xFF},
  {0x0F,0x17,0x1B,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1B,0x17,0x0F},
},
//
//
// **************
//* ************ *
//** ********** **
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//***          ***
//** ********** **
//  ************
//   ********** **
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//             ***
//   ********** **
//  ************ *
// **************
//
//
//
{
  {0xF8,0xF4,0xEC,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0xEC,0xF4,0xF8},
  {0x7F,0x7F,0xBF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xBF,0x7F,0x7F},
  {0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFE,0xFF,0xFF},
  {0x00,0x10,0x18,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1B,0x17,0x0F},
},
};



#define SCL 0
#define SDA 1


static void i2c_delay ( void )
{
    //unsigned int ra;
    //for(ra=0;ra<2;ra++) dummy(ra);
    //if(uart2_check()) uart2_recv();
}

static void scl_high ( void )
{
    PUT32(GPIOABASE+0x18,(1<<(SCL+ 0)));
}

static void sda_high ( void )
{
    PUT32(GPIOABASE+0x18,(1<<(SDA+ 0)));
}

static void scl_low ( void )
{
    PUT32(GPIOABASE+0x18,(1<<(SCL+16)));
}

static void sda_low ( void )
{
    PUT32(GPIOABASE+0x18,(1<<(SDA+16)));
}

static unsigned int sda_read ( void )
{
    unsigned int ra;

    ra=GET32(GPIOABASE+0x10);
    ra&=(1<<SDA);
    return(ra);
}

static void sda_input ( void )
{
    unsigned int ra;
    //moder
    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(SDA<<1)); //PC1
    //ra|= (1<<(SDA<<1)); //PC1
    PUT32(GPIOA_MODER,ra);
}

static void sda_output ( void )
{
    unsigned int ra;
    //moder
    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(SDA<<1)); //PC1
    ra|= (1<<(SDA<<1)); //PC1
    PUT32(GPIOA_MODER,ra);
}

static void i2c_pin_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(SDA<<1));
    ra&=~(3<<(SCL<<1));
    ra|=1<<(SDA<<1);
    ra|=1<<(SCL<<1);
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<SDA);
    ra&=~(1<<SCL);
    PUT32(GPIOABASE+0x04,ra);
    PUT32(GPIOABASE+0x18,(1<<SDA)|(1<<SCL));
}

static void i2c_start ( void )
{
    i2c_delay();
    i2c_delay();
    sda_low();
    i2c_delay();
    i2c_delay();
    scl_low();
    i2c_delay();
}

static void i2c_stop ( void )
{
    i2c_delay();
    scl_high();
    i2c_delay();
    sda_high();
    i2c_delay();
    i2c_delay();
}

static unsigned int i2c_write_byte ( unsigned int b )
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

static void send_command ( unsigned int cmd )
{
    i2c_start();
    if(i2c_write_byte(PADDR))
    {
        i2c_stop();
        //hexstring(0xBADBAD00);
        return;
    }
    i2c_write_byte(0x00);
    i2c_write_byte(cmd);
    i2c_stop();
}

static void send_data ( unsigned int data )
{
    i2c_start();
    if(i2c_write_byte(PADDR))
    {
        i2c_stop();
        //hexstring(0xBADBAD00);
        return;
    }
    i2c_write_byte(0x40);
    i2c_write_byte(data);
    i2c_stop();
}

static void SetPageStart ( unsigned int x )
{
    send_command(0xB0|(x&0x07));
}

static void SetColumn ( unsigned int x )
{
    send_command(0x00|((x>>0)&0x0F));
    send_command(0x10|((x>>4)&0x0F));
}

static void ClearScreen ( void )
{
    unsigned int ra;
    unsigned int rb;
#ifdef OLED128X32
    for(ra=0;ra<4;ra++) //128x32
#else
    for(ra=0;ra<8;ra++) //128x64
#endif
    {
        SetPageStart(ra);
        SetColumn(0);
        for(rb=0;rb<0x80;rb++) send_data(0);
    }
}

static void timer_delay ( unsigned int d )
{
    unsigned int ra,rb;

    ra=GET32(STK_CVR);
    while(1)
    {
        rb=(ra-GET32(STK_CVR))&STK_MASK;
        if(rb>=d) break;
    }
}

static unsigned int aa,bb,cc,dd;

static unsigned char num[8];
static unsigned char tmpnum[8];
void show_number ( void )
{
    unsigned int ra;
    unsigned int rb;

    for(ra=0;ra<4;ra++)
    {
        SetPageStart(ra);
        SetColumn(0);

        for(rb=0;rb<20;rb++) send_data(0x00);
        for(rb=0;rb<16;rb++) send_data(dfont[num[0]][ra][rb]);
        for(rb=0;rb<4;rb++) send_data(0x00);
        for(rb=0;rb<16;rb++) send_data(dfont[num[1]][ra][rb]);
        for(rb=0;rb<16;rb++) send_data(0x00);
        for(rb=0;rb<16;rb++) send_data(dfont[num[2]][ra][rb]);
        for(rb=0;rb<4;rb++) send_data(0x00);
        for(rb=0;rb<16;rb++) send_data(dfont[num[3]][ra][rb]);
        for(rb=0;rb<20;rb++) send_data(0x00);
    }
    for(ra=0;ra<4;ra++)
    {
        SetPageStart(ra+4);
        SetColumn(0);

        for(rb=0;rb<20;rb++) send_data(0x00);
        for(rb=0;rb<16;rb++) send_data(dfont[tmpnum[0]][ra][rb]);
        for(rb=0;rb<4;rb++) send_data(0x00);
        for(rb=0;rb<16;rb++) send_data(dfont[tmpnum[1]][ra][rb]);
        for(rb=0;rb<16;rb++) send_data(0x00);
        for(rb=0;rb<16;rb++) send_data(dfont[tmpnum[2]][ra][rb]);
        for(rb=0;rb<4;rb++) send_data(0x00);
        for(rb=0;rb<16;rb++) send_data(dfont[tmpnum[3]][ra][rb]);
        for(rb=0;rb<20;rb++) send_data(0x00);
    }
}
void ten_div ( unsigned int ra )
{
    unsigned int num;
    unsigned int res;
    unsigned int acc;
    unsigned int den;
    unsigned int rb;


    num=ra;
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
    aa=res;

    num=acc;
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
    bb=res;

    num=acc;
    res=0;
    acc=0;
    den=10;
    for(rb=0x40;rb;rb>>=1)
    {
        acc<<=1;
        if(rb&num) acc|=1;
        if(acc>=den)
        {
            acc-=den;
            res|=rb;
        }
    }
    cc=res;
    dd=acc;
}

//----------------------------------

#define MYDIVIDE

unsigned int sadd;

#define TMPSCL 3
#define TMPSDA 4

static void TMPDELAY ( void )
{
    unsigned int ra;
    for(ra=0;ra<20;ra++) dummy(ra);
}

static void TMPSCL_WRITE ( unsigned int x )
{
    if(x) PUT32(GPIOB_BSRR,(1<<TMPSCL)<< 0);
    else  PUT32(GPIOB_BSRR,(1<<TMPSCL)<<16);
}

static void TMPSDA_WRITE ( unsigned int x )
{
    if(x) PUT32(GPIOB_BSRR,(1<<TMPSDA)<< 0);
    else  PUT32(GPIOB_BSRR,(1<<TMPSDA)<<16);
}

static unsigned int TMPSDA_READ ( void )
{
    return(GET32(GPIOB_IDR)&(1<<TMPSDA));
}

static void TMPSCL_STROBE ( void )
{
    TMPDELAY();
    TMPSCL_WRITE(1);
    TMPDELAY();
    TMPSCL_WRITE(0);
    TMPDELAY();
}

static void TMPSDA_INPUT ( void )
{
    unsigned int ra;
    ra=GET32(GPIOB_MODER);
    ra&=~(3<<(TMPSDA<<1));
    //ra|= (1<<(TMPSDA<<1));
    PUT32(GPIOB_MODER,ra);
    TMPDELAY();
}

static void TMPSDA_OUTPUT ( void )
{
    unsigned int ra;
    ra=GET32(GPIOB_MODER);
    ra&=~(3<<(TMPSDA<<1)); //dont need this
    ra|= (1<<(TMPSDA<<1));
    PUT32(GPIOB_MODER,ra);
    TMPDELAY();
}

static void TMPSTOP ( void )
{
    TMPDELAY();
    TMPSCL_WRITE(1);
    TMPDELAY();
    TMPSDA_WRITE(1);
    TMPDELAY();
}

static void TMPSTART ( void )
{
    TMPDELAY();
    TMPSDA_WRITE(0);
    TMPDELAY();
    TMPSCL_WRITE(0);
    TMPDELAY();
}

static unsigned int WRITE_BYTE ( unsigned int x )
{
    unsigned int ra;
    unsigned int rb;

    for(ra=0x80;ra;ra>>=1)
    {
        TMPSDA_WRITE(ra&x);
        TMPSCL_STROBE();
    }
    TMPSDA_INPUT();
    TMPSCL_STROBE();
    rb=TMPSDA_READ();
    TMPSDA_OUTPUT();
    return(rb);
}

static unsigned int READ_BYTE ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rc=0;
    TMPSDA_INPUT();
    rb=0x80;
    for(ra=0;ra<8;ra++)
    {
        if(TMPSDA_READ()) rc|=rb;
        TMPSCL_STROBE();
        rb>>=1;
    }
    TMPSDA_OUTPUT();
    TMPSDA_WRITE(0);
    TMPSCL_STROBE();
    return(rc);
}

static unsigned int READ_REGISTER ( unsigned int sadd, unsigned int x)
{
    unsigned int rc;

    TMPSTART();
    WRITE_BYTE(sadd|0);
    WRITE_BYTE(x);
    TMPSTOP();

    TMPSTART();
    WRITE_BYTE(sadd|1);
    rc=READ_BYTE();
    rc<<=8;
    rc|=READ_BYTE();
    TMPSTOP();
    return(rc);
}
#ifdef MYDIVIDE
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
#endif


static void TMP_PIN_INIT ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<18; //port b
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOB_MODER);
    ra&=~(3<<(TMPSDA<<1));
    ra&=~(3<<(TMPSCL<<1));
    ra|=1<<(TMPSDA<<1);
    ra|=1<<(TMPSCL<<1);
    PUT32(GPIOB_MODER,ra);
    //OTYPER
    ra=GET32(GPIOB_OTYPER);
    ra&=~(1<<TMPSDA);
    ra&=~(1<<TMPSCL);
    PUT32(GPIOB_OTYPER,ra);
    //PUPDR
    ra=GET32(GPIOB_PUPDR);
    ra&=~(3<<(TMPSCL<<1));
    ra&=~(3<<(TMPSDA<<1));
    ra|= (1<<(TMPSDA<<1));  //PULL UP
    PUT32(GPIOB_PUPDR,ra);
    PUT32(GPIOB_BSRR,(1<<TMPSDA)|(1<<TMPSCL));

    TMPSTART();
    TMPSTOP();

    sadd=0x90;
    ra=READ_REGISTER(sadd,1);
    ra=READ_REGISTER(sadd,0);
}



static void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
    PUT32(USART1BASE+0x28,x);
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
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}
static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}


static void TMP_UPDATE ( void )
{
    unsigned int ra;
    unsigned int rb;

    ra=READ_REGISTER(sadd,0);
    hexstrings(ra); hexstrings(ra>>4);
    if(ra&0x8000)
    {
        rb=0;
    }
    else
    {
        rb=(ra>>4)*7373;
        rb>>=16;
        rb+=32;
        hexstrings(rb);
        if(rb>99) rb=99;
    }
#ifdef MYDIVIDE
    {
        unsigned int temp;
        temp=base_ten(ra>>8);
        tmpnum[0]=(temp>>4)&0xF;
        tmpnum[1]=(temp>>0)&0xF;
        temp=base_ten(rb);
        tmpnum[2]=(temp>>4)&0xF;
        tmpnum[3]=(temp>>0)&0xF;
        hexstring(temp);
    }
#else
    tmpnum[0]=rb/10;
    tmpnum[1]=rb%10;
#endif
}
//----------------------------------


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
static unsigned int timezone;
//static
int do_nmea ( void )
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
    //unsigned int num[4];
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
                    num[0]=xstring[0]&0xF;
                    num[1]=xstring[1]&0xF;
                    num[2]=xstring[2]&0xF;
                    num[3]=xstring[3]&0xF;

                    //just have the colon show validity state
                    if(validity)
                    {
                        //num[1]|=0x80;
                    }
                    show_number();
                    TMP_UPDATE();
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







    num[0]=0;
    num[1]=0;
    num[2]=0;
    num[3]=0;
    num[4]=0;
    num[5]=0;

    //i2c outputs
    i2c_pin_init();

    scl_high();
    sda_high();

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
#ifdef OLED128X32
    send_command(32-1); //128x32
#else
    send_command(64-1); //128x64
#endif
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
#ifdef OLED128X32
    send_command(0x02); //128x32
#else
    send_command(0x12); //128x64
#endif
    send_command(SETCONTRAST);          // 0x81
    send_command(0x0); //dimmest
//    send_command(0x8F);
    send_command(SETPRECHARGE);         // 0xd9
    send_command(0xF1);
    send_command(SETVCOMDESELECT);          // 0xDB
    send_command(0x00); //lowest/dimmest
    //send_command(0x40);
    send_command(NORMALDISPLAY);            // 0xA6
    send_command(DISPLAYALLONRESUME);   // 0xA4
    ClearScreen();
    send_command(DISPLAYON);                //--turn on oled panel


    num[0]=1;
    num[1]=2;
    num[2]=3;
    num[3]=4;

    tmpnum[0]=1;
    tmpnum[1]=2;
    tmpnum[2]=3;
    tmpnum[3]=4;

    show_number();

    TMP_PIN_INIT();

    uart_init();
    do_nmea();

    while(1)
    {
        TMP_UPDATE();
        show_number();
        for(ra=0;ra<0x80000;ra++) dummy(ra);
    }


    return(0);
}

/*

dfu-util -a 0 -s 0x08000000 -D notmain.bin

*/


