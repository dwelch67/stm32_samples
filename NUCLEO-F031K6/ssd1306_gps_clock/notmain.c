
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

#define USART1BASE 0x40013800



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
//static
void clock_init ( void )
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

    for(ra=0;ra<4;ra++) //128x32
//    for(ra=0;ra<8;ra++) //128x64
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
static unsigned int aa,bb,cc,dd;
//-------------------------------------------------------------------
static unsigned char num[8];
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

    //moder 10
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<18); //PA9
    ra|=2<<18; //PA9
    ra&=~(3<<20); //PA10
    ra|=2<<20; //PA10
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

//    PUT32(USART1BASE+0x0C,833); //8Mhz/9600
    PUT32(USART1BASE+0x0C,5000); //48Mhz/9600
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|(1<<2)|1);

    return(0);
}
//------------------------------------------------------------------------
//static void uart_send ( unsigned int x )
//{
    //while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
    //PUT32(USART1BASE+0x28,x);
//}
//------------------------------------------------------------------------
static unsigned int uart_recv ( void )
{
    //while(1)
    //{
        //if(zhead!=ztail)
        //{
            //ztail=(ztail+1)&ZMASK;
            //return(zbuff[ztail]);
        //}
        //show_time();
    //}
    while(1)
    {
        if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    }
    return(GET32(USART1BASE+0x24));
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------

static unsigned char xstring[32];
static unsigned int tim[4];
static unsigned int lasttim[4];

//------------------------------------------------------------------------
static void show_clock ( void )
{
    unsigned int ra;

    ra=0;
    if(tim[0]!=lasttim[0]) ra++;
    if(tim[1]!=lasttim[1]) ra++;
    if(tim[2]!=lasttim[2]) ra++;
    if(tim[3]!=lasttim[3]) ra++;
    if(ra==0) return;

    lasttim[0]=tim[0];
    lasttim[1]=tim[1];
    lasttim[2]=tim[2];
    lasttim[3]=tim[3];

    num[0]=tim[0];
    num[1]=tim[1];
    num[2]=tim[2];
    num[3]=tim[3];

    show_number();
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
    //unsigned char toggle_seconds;

    //toggle_seconds=0;
    state=0;
    off=0;
//$GPRMC,054033.00,V,
    while(1)
    {
        ra=uart_recv();
        //uart_send(ra);
        //uart_send(0x30+state);
        switch(state)
        {
            case 0:
            {
                if(ra=='$') state++;
                else state=0;
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
                        //zstring[rb++]=0x77;
                        //toggle_seconds^=0x10;
                        //zstring[rb++]=toggle_seconds;
                        //zstring[rb++]=xstring[0];
                        //zstring[rb++]=xstring[1];
                        //zstring[rb++]=xstring[2];
                        //zstring[rb++]=xstring[3];
                        //xstring[rb++]=0x0D;
                        //zstring[rb++]=0;
                        tim[0]=xstring[0]&0xF;
                        tim[1]=xstring[1]&0xF;
                        tim[2]=xstring[2]&0xF;
                        tim[3]=xstring[3]&0xF;
                    }
                    else
                    {
                        //zstring[0]=0x33;
                        //zstring[1]=0x33;
                        //zstring[2]=0x33;
                        //zstring[3]=0x33;
                        //xstring[4]=0x0D;
                        //zstring[5]=0;
                        tim[0]=0;
                        tim[1]=0;
                        tim[2]=0;
                        tim[3]=0;
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
                //if(zstring[off]==0)
                //{
                    //state=0;
                //}
                //else
                //{
                    //uart_send(zstring[off++]);
                //}
                //show_time();
                show_clock();
                state=0;
                break;
            }
        }
    }
    return(0);
}




//-------------------------------------------------------------------
int notmain ( void )
{
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
    //send_command(64-1); //128x64
    send_command(32-1); //128x32
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
    //send_command(0x12); //128x64
    send_command(0x02); //128x32
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



    uart_init();

    timezone=4;
    if(GET32(0x20000D00)==0x12341234) timezone=5;
    PUT32(0x20000D00,0x12341234);

    tim[0]=1;
    tim[1]=2;
    tim[2]=3;
    tim[3]=4;

    lasttim[0]=0;
    lasttim[1]=0;
    lasttim[2]=0;
    lasttim[3]=0;

    show_clock();

    do_nmea();

    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
