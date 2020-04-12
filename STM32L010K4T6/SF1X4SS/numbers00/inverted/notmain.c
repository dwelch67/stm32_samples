
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

#define GND0  0
#define LEDA  1
#define LEDF  2
#define GND1  3
#define GND2  4
#define LEDB  5
#define GND3  6
#define LEDG  7
#define LEDC  8
#define LEDP 11
#define LEDD 12
#define LEDE 15

static const unsigned int seven[16][4]=
{
{0x08C0913F,0x0890916F,0x08889177,0x0881917E,},//0
{0x19E2801D,0x19B2804D,0x19AA8055,0x19A3805C,},//1
{0x094490BB,0x091490EB,0x090C90F3,0x090590FA,},//2
{0x0960909F,0x093090CF,0x092890D7,0x092190DE,},//3
{0x1862819D,0x183281CD,0x182A81D5,0x182381DC,},//4
{0x8860119F,0x883011CF,0x882811D7,0x882111DE,},//5
{0x884011BF,0x881011EF,0x880811F7,0x880111FE,},//6
{0x09E2901D,0x09B2904D,0x09AA9055,0x09A3905C,},//7
{0x084091BF,0x081091EF,0x080891F7,0x080191FE,},//8
{0x0862919D,0x083291CD,0x082A91D5,0x082391DC,},//9
{0x91A60059,0x91A60059,0x91A60059,0x91A60059,},//10
{0x91A60059,0x91A60059,0x91A60059,0x91A60059,},//11
{0x91A60059,0x91A60059,0x91A60059,0x91A60059,},//12
{0x91A60059,0x91A60059,0x91A60059,0x91A60059,},//13
{0x91A60059,0x91A60059,0x91A60059,0x91A60059,},//14
{0x91A60059,0x91A60059,0x91A60059,0x91A60059,},//15
};
#define NADA 0x91A60059

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
    ra&=~(3<<(GND0<<1));
    ra&=~(3<<(GND1<<1));
    ra&=~(3<<(GND2<<1));
    ra&=~(3<<(GND3<<1));
    ra|= (1<<(LEDA<<1));
    ra|= (1<<(LEDB<<1));
    ra|= (1<<(LEDC<<1));
    ra|= (1<<(LEDD<<1));
    ra|= (1<<(LEDE<<1));
    ra|= (1<<(LEDF<<1));
    ra|= (1<<(LEDG<<1));
    ra|= (1<<(LEDP<<1));
    ra|= (1<<(GND0<<1));
    ra|= (1<<(GND1<<1));
    ra|= (1<<(GND2<<1));
    ra|= (1<<(GND3<<1));
    PUT32(GPIOA_MODER,ra);

    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<LEDA);
    ra&=~(1<<LEDB);
    ra&=~(1<<LEDC);
    ra&=~(1<<LEDD);
    ra&=~(1<<LEDE);
    ra&=~(1<<LEDF);
    ra&=~(1<<LEDG);
    ra&=~(1<<LEDP);
    ra&=~(1<<GND0);
    ra&=~(1<<GND1);
    ra&=~(1<<GND2);
    ra&=~(1<<GND3);
    PUT32(GPIOA_OTYPER,ra);

    PUT32(GPIOA_BSRR,NADA);


    counter=0;
    num[0]=8;
    num[1]=8;
    num[2]=8;
    num[3]=8;

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
        num[0]=newnum[0]&0xF;
        num[1]=newnum[1]&0xF;
        num[2]=newnum[2]&0xF;
        num[3]=newnum[3]&0xF;
    }

    return(0);
}
