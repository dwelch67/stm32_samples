
//-------------------------------------------------------------------
//-------------------------------------------------------------------

//#define DEBUG_UART
#define PADDR 0x78


void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define GPIOA_MODER (GPIOABASE+0x00)

#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF;

#define TIM2_BASE 0x40000000
#define TIM2_CR1 (TIM2_BASE+0x00)
#define TIM2_CR2 (TIM2_BASE+0x04)
#define TIM2_SR  (TIM2_BASE+0x10)
#define TIM2_CNT (TIM2_BASE+0x24)
#define TIM2_PSC (TIM2_BASE+0x28)
#define TIM2_ARR (TIM2_BASE+0x2C)
#define TIM2_MASK 0xFFFFFFFF


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
//
//
{
  {0xFE,0xFD,0xFB,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0xFB,0xFD,0xFE},
  {0xDF,0xDF,0x8F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x8F,0xDF,0xDF},
  {0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF},
  {0x03,0x05,0x06,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x06,0x05,0x03},
},
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
//
//
{
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFC,0xFE},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x8F,0xDF,0xDF},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03},
},
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
//
//
{
  {0x00,0x01,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0xFB,0xFD,0xFE},
  {0xC0,0xC0,0xA0,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x2F,0x1F,0x1F},
  {0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x03,0x05,0x06,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x06,0x04,0x00},
},
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
//
//
{
  {0x00,0x01,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0xFB,0xFD,0xFE},
  {0x00,0x00,0x20,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0xAF,0xDF,0xDF},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF},
  {0x00,0x04,0x06,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x06,0x05,0x03},
},
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
//
//
{
  {0xFE,0xFC,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFC,0xFE},
  {0x1F,0x1F,0x2F,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0xAF,0xDF,0xDF},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03},
},
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
//
//
{
  {0xFE,0xFD,0xFB,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x01,0x00},
  {0x1F,0x1F,0x2F,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0xA0,0xC0,0xC0},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF},
  {0x00,0x04,0x06,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x06,0x05,0x03},
},
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
//
//
{
  {0xFE,0xFD,0xFB,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x01,0x00},
  {0xDF,0xDF,0xAF,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0xA0,0xC0,0xC0},
  {0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF},
  {0x03,0x05,0x06,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x06,0x05,0x03},
},
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
//
//
{
  {0x00,0x01,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0xFB,0xFD,0xFE},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x8F,0xDF,0xDF},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03},
},
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
//
//
{
  {0xFE,0xFD,0xFB,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0xFB,0xFD,0xFE},
  {0xDF,0xDF,0xAF,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0xAF,0xDF,0xDF},
  {0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF},
  {0x03,0x05,0x06,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x06,0x05,0x03},
},
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
//
//
{
  {0xFE,0xFD,0xFB,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0xFB,0xFD,0xFE},
  {0x1F,0x1F,0x2F,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0x70,0xAF,0xDF,0xDF},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF},
  {0x00,0x04,0x06,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x06,0x05,0x03},
},
};

//const unsigned char seven_seg[128]=
//{
//0xF, //  ####
//0x9, //  #  #
//0x9, //  #  #
//0x9, //  #  #
//0x9, //  #  #
//0x9, //  #  #
//0xF, //  ####
//0x0, //
//0x1, //  ...#
//0x1, //  .  #
//0x1, //  .  #
//0x1, //  ...#
//0x1, //  .  #
//0x1, //  .  #
//0x1, //  ...#
//0x0, //
//0xF, //  ####
//0x1, //  .  #
//0x1, //  .  #
//0xf, //  ####
//0x8, //  #  .
//0x8, //  #  .
//0xF, //  ####
//0x0, //
//0xF, //  ####
//0x1, //  .  #
//0x1, //  .  #
//0xF, //  ####
//0x1, //  .  #
//0x1, //  .  #
//0xF, //  ####
//0x0, //
//0x9, //  #..#
//0x9, //  #  #
//0x9, //  #  #
//0xF, //  ####
//0x1, //  .  #
//0x1, //  .  #
//0x1, //  ...#
//0x0, //
//0xF, //  ####
//0x8, //  #  .
//0x8, //  #  .
//0xF, //  ####
//0x1, //  .  #
//0x1, //  .  #
//0xF, //  ####
//0x0, //
//0xF, //  ####
//0x8, //  #  .
//0x8, //  #  .
//0xF, //  ####
//0x9, //  #  #
//0x9, //  #  #
//0xF, //  ####
//0x0, //
//0xF, //  ####
//0x1, //  .  #
//0x1, //  .  #
//0x1, //  ...#
//0x1, //  .  #
//0x1, //  .  #
//0x1, //  ...#
//0x0, //
//0xF, //  ####
//0x9, //  #  #
//0x9, //  #  #
//0xF, //  ####
//0x9, //  #  #
//0x9, //  #  #
//0xF, //  ####
//0x0, //
//0xF, //  ####
//0x9, //  #  #
//0x9, //  #  #
//0xF, //  ####
//0x1, //  .  #
//0x1, //  .  #
//0x1, //  ...#
//0x0, //
//0xF, //  ####
//0x9, //  #  #
//0x9, //  #  #
//0xF, //  ####
//0x9, //  #  #
//0x9, //  #  #
//0x9, //  #..#
//0x0, //
//0xE, //  ###.
//0x9, //  #  #
//0x9, //  #  #
//0xE, //  ###.
//0x9, //  #  #
//0x9, //  #  #
//0xE, //  ###.
//0x0, //
//0xF, //  ####
//0x8, //  #  .
//0x8, //  #  .
//0x8, //  #...
//0x8, //  #  .
//0x8, //  #  .
//0xF, //  ####
//0x0, //
//0xE, //  ###.
//0x9, //  #  #
//0x9, //  #  #
//0x9, //  #..#
//0x9, //  #  #
//0x9, //  #  #
//0xE, //  ###.
//0x0, //
//0xF, //  ####
//0x8, //  #  .
//0x8, //  #  .
//0xF, //  ####
//0x8, //  #  .
//0x8, //  #  .
//0xF, //  ####
//0x0, //
//0xF, //  ####
//0x8, //  #  .
//0x8, //  #  .
//0xF, //  ####
//0x8, //  #  .
//0x8, //  #  .
//0x8, //  #...
//0x0, //

//};




//const unsigned char numfontdata[12][8]=
//{
////---- 0x30 ----
////[ #####  ]
////[##  ### ]
////[## #### ]
////[#### ## ]
////[###  ## ]
////[###  ## ]
////[ #####  ]
////[
////{0x7C,0xFE,0x9E,0xB2,0xE2,0xFE,0x7C,0x00},
//{0x3E,0x7F,0x79,0x4D,0x47,0x7F,0x3E,0x00},
////---- 0x31 ----
////[   ##   ]
////[  ###   ]
////[ ####   ]
////[   ##   ]
////[   ##   ]
////[   ##   ]
////[ ###### ]
////[
////{0x00,0x22,0x62,0xFE,0xFE,0x02,0x02,0x00},
//{0x00,0x44,0x46,0x7F,0x7F,0x40,0x40,0x00},
////---- 0x32 ----
////[ #####  ]
////[##   ## ]
////[     ## ]
////[   ###  ]
////[ ###    ]
////[##   ## ]
////[####### ]
////[
////{0x46,0xCE,0x8A,0x9A,0x92,0xF6,0x66,0x00},
//{0x62,0x73,0x51,0x59,0x49,0x6F,0x66,0x00},
////---- 0x33 ----
////[ #####  ]
////[##   ## ]
////[     ## ]
////[  ####  ]
////[     ## ]
////[##   ## ]
////[ #####  ]
////[
////{0x44,0xC6,0x92,0x92,0x92,0xFE,0x6C,0x00},
//{0x22,0x63,0x49,0x49,0x49,0x7F,0x36,0x00},
////---- 0x34 ----
////[   ###  ]
////[  ####  ]
////[ ## ##  ]
////[##  ##  ]
////[####### ]
////[    ##  ]
////[   #### ]
////[
////{0x18,0x38,0x68,0xCA,0xFE,0xFE,0x0A,0x00},
//{0x18,0x1C,0x16,0x53,0x7F,0x7F,0x50,0x00},
////---- 0x35 ----
////[####### ]
////[##      ]
////[######  ]
////[     ## ]
////[     ## ]
////[##   ## ]
////[ #####  ]
////[
////{0xE4,0xE6,0xA2,0xA2,0xA2,0xBE,0x9C,0x00},
//{0x27,0x67,0x45,0x45,0x45,0x7D,0x39,0x00},
////---- 0x36 ----
////[ #####  ]
////[##   ## ]
////[##      ]
////[######  ]
////[##   ## ]
////[##   ## ]
////[ #####  ]
////[
////{0x7C,0xFE,0x92,0x92,0x92,0xDE,0x4C,0x00},
//{0x3E,0x7F,0x49,0x49,0x49,0x7B,0x32,0x00},
////---- 0x37 ----
////[####### ]
////[##   ## ]
////[    ##  ]
////[   ##   ]
////[  ##    ]
////[  ##    ]
////[  ##    ]
////[
////{0xC0,0xC0,0x8E,0x9E,0xB0,0xE0,0xC0,0x00},
//{0x03,0x03,0x71,0x79,0x0D,0x07,0x03,0x00},
////---- 0x38 ----
////[ #####  ]
////[##   ## ]
////[##   ## ]
////[ #####  ]
////[##   ## ]
////[##   ## ]
////[ #####  ]
////[
////{0x6C,0xFE,0x92,0x92,0x92,0xFE,0x6C,0x00},
//{0x36,0x7F,0x49,0x49,0x49,0x7F,0x36,0x00},
////---- 0x39 ----
////[ #####  ]
////[##   ## ]
////[##   ## ]
////[ ###### ]
////[     ## ]
////[##   ## ]
////[ #####  ]
////[
////{0x64,0xF6,0x92,0x92,0x92,0xFE,0x7C,0x00},
//{0x26,0x6F,0x49,0x49,0x49,0x7F,0x3E,0x00},
////---- 0x3A ----
////[        ]
////[  ##    ]
////[        ]
////[        ]
////[        ]
////[  ##    ]
////[        ]
////[
////{0x00,0x00,0x44,0x44,0x00,0x00,0x00,0x00},
//{0x00,0x00,0x22,0x22,0x00,0x00,0x00,0x00},
////---- 0x2D ----
////[        ]
////[        ]
////[        ]
////[######  ]
////[        ]
////[        ]
////[        ]
////[
////{0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},
//{0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00}
//};
#define FLASH_ACR  0x40022000

//-------------------------------------------------------------------
static void clock_init ( void )
{
    unsigned int ra;

    PUT32(FLASH_ACR,0x11);
    //HSI8 based 48MHz.  (8/2)*12
    ra=0;
    ra|=10<<18; //times 12
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


//-------------------------------------------------------------------





#define SCL 0
#define SDA 1

//-------------------------------------------------------------------
static void i2c_delay ( void )
{
    //unsigned int ra;
    //for(ra=0;ra<2;ra++) dummy(ra);
    //if(uart2_check()) uart2_recv();
}
//-------------------------------------------------------------------
static void scl_high ( void )
{
    PUT32(GPIOABASE+0x18,(1<<(SCL+ 0)));
}
//-------------------------------------------------------------------
static void sda_high ( void )
{
    PUT32(GPIOABASE+0x18,(1<<(SDA+ 0)));
}
//-------------------------------------------------------------------
static void scl_low ( void )
{
    PUT32(GPIOABASE+0x18,(1<<(SCL+16)));
}
//-------------------------------------------------------------------
static void sda_low ( void )
{
    PUT32(GPIOABASE+0x18,(1<<(SDA+16)));
}
//-------------------------------------------------------------------
static unsigned int sda_read ( void )
{
    unsigned int ra;

    ra=GET32(GPIOABASE+0x10);
    ra&=(1<<SDA);
    return(ra);
}
//-------------------------------------------------------------------
static void sda_input ( void )
{
    unsigned int ra;
    //moder
    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(SDA<<1)); //PC1
    //ra|= (1<<(SDA<<1)); //PC1
    PUT32(GPIOA_MODER,ra);
}
//-------------------------------------------------------------------
static void sda_output ( void )
{
    unsigned int ra;
    //moder
    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(SDA<<1)); //PC1
    ra|= (1<<(SDA<<1)); //PC1
    PUT32(GPIOA_MODER,ra);
}
//-------------------------------------------------------------------
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
//-------------------------------------------------------------------
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
//-------------------------------------------------------------------
static void i2c_stop ( void )
{
    i2c_delay();
    scl_high();
    i2c_delay();
    sda_high();
    i2c_delay();
    i2c_delay();
}
//-------------------------------------------------------------------
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
//-------------------------------------------------------------------
//static unsigned int i2c_read_byte ( unsigned int *b )
//{
    //unsigned int ra;
    //unsigned int rb;
    //i2c_delay();
    //sda_input();
    //rb=0;
    //for(ra=0;ra<9;ra++)
    //{
        //i2c_delay();
        //scl_high();
        //i2c_delay();
        //rb<<=1;
        //if(sda_read()) rb|=1;
        //i2c_delay();
        //scl_low();
        //i2c_delay();
    //}
    //sda_output();
    //i2c_delay();
    //ra=rb&1;
    //*b=rb>>1;
    //return(ra);
//}
//-------------------------------------------------------------------
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
//-------------------------------------------------------------------
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
//-------------------------------------------------------------------
static void SetPageStart ( unsigned int x )
{
    send_command(0xB0|(x&0x07));
}
//-------------------------------------------------------------------
static void SetColumn ( unsigned int x )
{
    send_command(0x00|((x>>0)&0x0F));
    send_command(0x10|((x>>4)&0x0F));
}
//-------------------------------------------------------------------
static void ClearScreen ( void )
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
//-------------------------------------------------------------------
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
//-------------------------------------------------------------------
static unsigned int mt,mo;
static unsigned int st,so;
static void long_div ( unsigned int ra )
{
    unsigned int num;
    unsigned int res;
    unsigned int acc;
    unsigned int den;
    unsigned int rb;
    unsigned int mm;
    unsigned int ss;

    //8000000/50000 = 160 ticks per second
    //divide by 160
    num=ra;
    res=0;
    acc=0;
    den=960;
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
    num=res;
    res=0;
    acc=0;
    den=60;
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
    mm=res;
    ss=acc;

    num=mm;
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
    mt=res;
    mo=acc;

    num=ss;
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
    st=res;
    so=acc;
}
//-------------------------------------------------------------------
static unsigned char num[8];
void show_number ( void )
{
    unsigned int ra;
    unsigned int rb;

    for(ra=0;ra<4;ra++)
    {
        SetPageStart(ra+4);
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

}
void show_number_invert ( void )
{
    unsigned int ra;
    unsigned int rb;

    for(ra=0;ra<4;ra++)
    {
        SetPageStart(ra+4);
        SetColumn(0);

        for(rb=0;rb<20;rb++) send_data(~0x00);
        for(rb=0;rb<16;rb++) send_data(~dfont[num[0]][ra][rb]);
        for(rb=0;rb< 4;rb++) send_data(~0x00);
        for(rb=0;rb<16;rb++) send_data(~dfont[num[1]][ra][rb]);
        for(rb=0;rb<16;rb++) send_data(~0x00);
        for(rb=0;rb<16;rb++) send_data(~dfont[num[2]][ra][rb]);
        for(rb=0;rb< 4;rb++) send_data(~0x00);
        for(rb=0;rb<16;rb++) send_data(~dfont[num[3]][ra][rb]);
        for(rb=0;rb<20;rb++) send_data(~0x00);
    }

}
//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int lastso;

    num[0]=0;
    num[1]=0;
    num[2]=0;
    num[3]=0;
    num[4]=0;
    num[5]=0;

    clock_init();

    //i2c outputs
    i2c_pin_init();


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

    ra=GET32(RCCBASE+0x1C);
    ra|=1<<0; //enable timer 2
    PUT32(RCCBASE+0x1C,ra);

    PUT32(TIM2_CR1,0x00000000);
    PUT32(TIM2_ARR,(960*5*60)+960);
//PUT32(TIM2_ARR,(960*2*60)+(10*960));
//PUT32(TIM2_ARR,(960*1*60)+(5*960));
//PUT32(TIM2_ARR,(35*960));
PUT32(TIM2_ARR,(5*960));
    PUT32(TIM2_CNT,0x00000000);
    PUT32(TIM2_PSC,50000-1);
    PUT32(TIM2_CR1,0x00000011);
    PUT32(TIM2_SR,0);
    rc=0;
    lastso=99;
    while(1)
    {
        ra=GET32(TIM2_SR);
        if(ra&1) break;
        ra=GET32(TIM2_CNT);
        long_div(ra);
        num[0]=mt;
        num[1]=mo;
        num[2]=st;
        num[3]=so;
        if(so==lastso) continue;
        lastso=so;
        if(ra<(960*30))
        {
            unsigned int rx;

            rc++;
            for(rx=0;rx<2;rx++)
            {
                SetPageStart(rx);
                SetColumn(0);
                if((rc&3)==0)
                {
                    for(rb=0;rb<128;rb++) send_data(0x00);
                }
                else
                {
                    for(rb=0;rb<128;rb++) send_data(0xFF);
                }
            }
        }
        show_number();
    }
    //PUT32(TIM2_CR1,0x00000000);
    //PUT32(TIM2_ARR,0xFFFFFFFF);
    //PUT32(TIM2_CNT,0x00000960);
////PUT32(TIM2_CNT,960*9*60);
    //PUT32(TIM2_PSC,50000-1);
    PUT32(TIM2_CR1,0x00000001);
    lastso=99;
    while(1)
    {
        ra=GET32(TIM2_CNT);
        long_div(ra);
        num[0]=mt;
        num[1]=mo;
        num[2]=st;
        num[3]=so;
        if(so==lastso) continue;
        lastso=so;
        if(ra<(960*60*60))
        {
            if(ra<960*30)
            {
                unsigned int rx;

                for(rx=0;rx<4;rx++)
                {
                    SetPageStart(rx);
                    SetColumn(0);
                    if(so&1)
                    {
                        for(rb=0;rb<128;rb++) send_data(0xFF);
                    }
                    else
                    {
                        for(rb=0;rb<128;rb++) send_data(0x00);
                    }
                }
                if(so&1) show_number_invert();
                else     show_number();
            }
            else
            {
                show_number_invert();
            }
        }
    }



    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
