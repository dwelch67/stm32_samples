
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

void ienable ( void );
void DOWFI ( void );

#define GPIOA_BASE      0x50000000
#define GPIOA_MODER     (GPIOA_BASE+0x00)
#define GPIOA_OTYPER    (GPIOA_BASE+0x04)
#define GPIOA_BSRR      (GPIOA_BASE+0x18)
#define GPIOA_AFRL      (GPIOA_BASE+0x24)

#define RCC_BASE        0x40021000
#define RCC_CR          (RCC_BASE+0x00)
#define RCC_CFGR        (RCC_BASE+0x0C)
#define RCC_IOPENR      (RCC_BASE+0x2C)

#define STK_CSR  0xE000E010
#define STK_RVR  0xE000E014
#define STK_CVR  0xE000E018
#define STK_MASK 0x00FFFFFF

#define LEDG  0
#define LEDA  1
#define LEDC  2
#define LEDF  3
#define LEDX  4
#define VCC3  5
#define LEDP  6
#define VCC2  7
#define LEDE  8
#define LEDL 11
#define LEDD 12
#define VCC1 13
#define VCC0 14
#define LEDB 15

static const unsigned int seven[32][4]=
{
{0xB1BE4841,0xD1BE2841,0xF13E08C1,0xF19E0861,},//0
{0xA0B4594B,0xC0B4394B,0xE03419CB,0xE094196B,},//1
{0xB1B3484C,0xD1B3284C,0xF13308CC,0xF193086C,},//2
{0xB0B74948,0xD0B72948,0xF03709C8,0xF0970968,},//3
{0xA0BD5942,0xC0BD3942,0xE03D19C2,0xE09D1962,},//4
{0x30BFC940,0x50BFA940,0x703F89C0,0x709F8960,},//5
{0x31BFC840,0x51BFA840,0x713F88C0,0x719F8860,},//6
{0xA0B65949,0xC0B63949,0xE03619C9,0xE0961969,},//7
{0xB1BF4840,0xD1BF2840,0xF13F08C0,0xF19F0860,},//8
{0xA0BF5940,0xC0BF3940,0xE03F19C0,0xE09F1960,},//9
{0x68B0914F,0x68B0914F,0x68B0914F,0x68B0914F,},//10
{0x68B0914F,0x68B0914F,0x68B0914F,0x68B0914F,},//11
{0x68B0914F,0x68B0914F,0x68B0914F,0x68B0914F,},//12
{0x68B0914F,0x68B0914F,0x68B0914F,0x68B0914F,},//13
{0x68B0914F,0x68B0914F,0x68B0914F,0x68B0914F,},//14
{0x68B0914F,0x68B0914F,0x68B0914F,0x68B0914F,},//15
{0xB9BE4041,0xD9BE2041,0xF93E00C1,0xF99E0061,},//16
{0xA8B4514B,0xC8B4314B,0xE83411CB,0xE894116B,},//17
{0xB9B3404C,0xD9B3204C,0xF93300CC,0xF993006C,},//18
{0xB8B74148,0xD8B72148,0xF83701C8,0xF8970168,},//19
{0xA8BD5142,0xC8BD3142,0xE83D11C2,0xE89D1162,},//20
{0x38BFC140,0x58BFA140,0x783F81C0,0x789F8160,},//21
{0x39BFC040,0x59BFA040,0x793F80C0,0x799F8060,},//22
{0xA8B65149,0xC8B63149,0xE83611C9,0xE8961169,},//23
{0xB9BF4040,0xD9BF2040,0xF93F00C0,0xF99F0060,},//24
{0xA8BF5140,0xC8BF3140,0xE83F11C0,0xE89F1160,},//25
{0x68B0914F,0x68B0914F,0x68B0914F,0x68B0914F,},//26
{0x68B0914F,0x68B0914F,0x68B0914F,0x68B0914F,},//27
{0x68B0914F,0x68B0914F,0x68B0914F,0x68B0914F,},//28
{0x68B0914F,0x68B0914F,0x68B0914F,0x68B0914F,},//29
{0x68B0914F,0x68B0914F,0x68B0914F,0x68B0914F,},//30
{0x68B0914F,0x68B0914F,0x68B0914F,0x68B0914F,},//31

};
#define NADA 0x68B0914F

//-------------------------------------------------------------------
//  **** WARNING THE CODE BELOW IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------
volatile unsigned int counter;
volatile unsigned int num[4];
void systick_handler ( void )
{
    GET32(STK_CSR);
    switch(counter&3)
    {
        case 0: PUT32(GPIOA_BSRR,seven[num[0]][0]); break;
        case 1: PUT32(GPIOA_BSRR,seven[num[1]][1]); break;
        case 2: PUT32(GPIOA_BSRR,seven[num[2]][2]); break;
        case 3: PUT32(GPIOA_BSRR,seven[num[3]][3]); break;
    }
    counter++;
}
//-------------------------------------------------------------------
//  **** WARNING THE CODE ABOVE IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------


#define UART2_BASE 0x40004400

static void clock_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_CR);
    ra|=1; //HSI16ON
    PUT32(RCC_CR,ra);
    while(1)
    {
        ra=GET32(RCC_CR);
        if(ra&(1<<2)) break; //HSI16RDYF
    }
    ra=GET32(RCC_CFGR);
    ra&=(~3);
    ra|=( 1); //HSI16
    PUT32(RCC_CFGR,ra);
    while(1)
    {
        ra=GET32(RCC_CFGR);
        if(((ra>>2)&3)==1) break;
    }
}

static int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_IOPENR);
    ra|=1<<0; //enable port a
    PUT32(RCC_IOPENR,ra);

    ra=GET32(RCC_BASE+0x38);
    ra|=1<<17; //enable USART2
    PUT32(RCC_BASE+0x38,ra);

    //PA9  UART2_TX
    //PA10 UART2_RX

    //moder
    ra=GET32(GPIOA_MODER+0x00);
    ra&=~(3<<18); //PA9
    ra&=~(3<<20); //PA10
    ra|=2<<18; //PA9
    ra|=2<<20; //PA10
    PUT32(GPIOA_MODER+0x00,ra);

    //AFRL
    ra=GET32(GPIOA_AFRL);
    ra&=~(0xF<<4); //PA9
    ra&=~(0xF<<8); //PA10
    ra|=(0x4<<4); //PA9
    ra|=(0x4<<8); //PA10
    PUT32(GPIOA_AFRL,ra);

    ra=GET32(RCC_BASE+0x28);
    ra|=1<<17; //reset USART2
    PUT32(RCC_BASE+0x28,ra);
    ra&=~(1<<17);
    PUT32(RCC_BASE+0x28,ra);

//    PUT32(UART2_BASE+0x0C,139); //16000000/115200
    PUT32(UART2_BASE+0x0C,1667); //16000000/9600
    PUT32(UART2_BASE+0x08,1<<12);
    PUT32(UART2_BASE+0x00,(1<<3)|(1<<2)|1);

    return(0);
}

//static void uart_send ( unsigned int x )
//{
    //while(1) if(GET32(UART2_BASE+0x1C)&(1<<7)) break;
    //PUT32(UART2_BASE+0x28,x);
//}

static unsigned int uart_recv ( void )
{
    while(1) if((GET32(UART2_BASE+0x1C))&(1<<5)) break;
    return(GET32(UART2_BASE+0x24));
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int newnum[6];

    clock_init();
    uart_init();

    ra=GET32(RCC_IOPENR);
    ra|=1<<0; //GPIOA
    PUT32(RCC_IOPENR,ra);

    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(LEDA<<1));
    ra&=~(3<<(LEDB<<1));
    ra&=~(3<<(LEDC<<1));
    ra&=~(3<<(LEDD<<1));
    ra&=~(3<<(LEDE<<1));
    ra&=~(3<<(LEDF<<1));
    ra&=~(3<<(LEDG<<1));
    ra&=~(3<<(LEDP<<1));
    ra&=~(3<<(LEDL<<1));
    ra&=~(3<<(LEDX<<1));
    ra&=~(3<<(VCC0<<1));
    ra&=~(3<<(VCC1<<1));
    ra&=~(3<<(VCC2<<1));
    ra&=~(3<<(VCC3<<1));
    ra|= (1<<(LEDA<<1));
    ra|= (1<<(LEDB<<1));
    ra|= (1<<(LEDC<<1));
    ra|= (1<<(LEDD<<1));
    ra|= (1<<(LEDE<<1));
    ra|= (1<<(LEDF<<1));
    ra|= (1<<(LEDG<<1));
    ra|= (1<<(LEDP<<1));
    ra|= (1<<(LEDL<<1));
    ra|= (1<<(LEDX<<1));
    ra|= (1<<(VCC0<<1));
    ra|= (1<<(VCC1<<1));
    ra|= (1<<(VCC2<<1));
    ra|= (1<<(VCC3<<1));
    PUT32(GPIOA_MODER,ra);

    PUT32(GPIOA_BSRR,NADA);

    counter=0;
    num[0]=1;
    num[1]=2;
    num[2]=3;
    num[3]=4;

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,0x2000-1);
    PUT32(STK_CVR,0x2000-1);
    PUT32(STK_CSR,7);
    ienable();

    while(1)
    {
        ra=uart_recv();
        if(ra!='X') continue;
        newnum[0]=uart_recv();
        newnum[1]=uart_recv();
        newnum[2]=uart_recv();
        newnum[3]=uart_recv();
        newnum[4]=uart_recv();
        newnum[6]=uart_recv();
        ra=uart_recv();
        if(ra!='Y') continue;
        num[0]=(newnum[0]&0xF)|((newnum[3]&0x40)>>2);
        num[1]=(newnum[1]&0xF)|((newnum[3]&0x40)>>2);
        num[2]=(newnum[2]&0xF)|((newnum[3]&0x40)>>2);
        num[3]=(newnum[3]&0xF)|((newnum[3]&0x40)>>2);
    }

    return(0);
}
