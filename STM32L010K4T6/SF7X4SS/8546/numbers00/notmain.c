
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

static const unsigned int seven[32][4]=
{
{0x997C0083,0x197D8082,0x995D00A2,0x993D00C2,},//0
{0x81681897,0x01699896,0x814918B6,0x812918D6,},//1
{0x987A0185,0x187B8184,0x985B01A4,0x983B01C4,},//2
{0x917A0885,0x117B8884,0x915B08A4,0x913B08C4,},//3
{0x816E1891,0x016F9890,0x814F18B0,0x812F18D0,},//4
{0x91760889,0x11778888,0x915708A8,0x913708C8,},//5
{0x99760089,0x19778088,0x995700A8,0x993700C8,},//6
{0x81781887,0x01799886,0x815918A6,0x813918C6,},//7
{0x997E0081,0x197F8080,0x995F00A0,0x993F00C0,},//8
{0x817E1881,0x017F9880,0x815F18A0,0x813F18C0,},//9
{0x8061199E,0x8061199E,0x8061199E,0x8061199E,},//10
{0x8061199E,0x8061199E,0x8061199E,0x8061199E,},//11
{0x8061199E,0x8061199E,0x8061199E,0x8061199E,},//12
{0x8061199E,0x8061199E,0x8061199E,0x8061199E,},//13
{0x8061199E,0x8061199E,0x8061199E,0x8061199E,},//14
{0x8061199E,0x8061199E,0x8061199E,0x8061199E,},//15
{0x99FC0003,0x19FD8002,0x99DD0022,0x99BD0042,},//16
{0x81E81817,0x01E99816,0x81C91836,0x81A91856,},//17
{0x98FA0105,0x18FB8104,0x98DB0124,0x98BB0144,},//18
{0x91FA0805,0x11FB8804,0x91DB0824,0x91BB0844,},//19
{0x81EE1811,0x01EF9810,0x81CF1830,0x81AF1850,},//20
{0x91F60809,0x11F78808,0x91D70828,0x91B70848,},//21
{0x99F60009,0x19F78008,0x99D70028,0x99B70048,},//22
{0x81F81807,0x01F99806,0x81D91826,0x81B91846,},//23
{0x99FE0001,0x19FF8000,0x99DF0020,0x99BF0040,},//24
{0x81FE1801,0x01FF9800,0x81DF1820,0x81BF1840,},//25
{0x8061199E,0x8061199E,0x8061199E,0x8061199E,},//26
{0x8061199E,0x8061199E,0x8061199E,0x8061199E,},//27
{0x8061199E,0x8061199E,0x8061199E,0x8061199E,},//28
{0x8061199E,0x8061199E,0x8061199E,0x8061199E,},//29
{0x8061199E,0x8061199E,0x8061199E,0x8061199E,},//30
{0x8061199E,0x8061199E,0x8061199E,0x8061199E,},//31
};
#define NADA 0x8061199E

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
        num[0]=(newnum[0]&0xF)|((newnum[0]&0x40)>>2);
        num[1]=(newnum[1]&0xF)|((newnum[1]&0x40)>>2);
        num[2]=(newnum[2]&0xF)|((newnum[2]&0x40)>>2);
        num[3]=(newnum[3]&0xF)|((newnum[3]&0x40)>>2);
    }

    return(0);
}
