
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

unsigned int TEST ( unsigned int, unsigned int );
unsigned int swtest ( unsigned int, unsigned int, unsigned int);
unsigned int lwtest ( unsigned int, unsigned int, unsigned int);
unsigned int HOP ( unsigned int, unsigned int, unsigned int );

#define ICIALLU 0xE000EF50
#define CCR 0xE000ED14

#define RCC_BASE 0x40023800
#define RCC_CR          (RCC_BASE+0x00)
#define RCC_CFGR        (RCC_BASE+0x08)
#define RCC_APB1RSTR    (RCC_BASE+0x20)
#define RCC_AHB1ENR     (RCC_BASE+0x30)
#define RCC_APB1ENR     (RCC_BASE+0x40)

#define GPIODBASE 0x40020C00
#define GPIOD_MODER     (GPIODBASE+0x00)
#define GPIOD_AFRH      (GPIODBASE+0x24)

#define USART3_BASE 0x40004800
#define UART_BASE USART3_BASE
#define UART_CR1      (UART_BASE+0x00)
#define UART_BRR      (UART_BASE+0x0C)
#define UART_ISR      (UART_BASE+0x1C)
#define UART_RDR      (UART_BASE+0x24)
#define UART_TDR      (UART_BASE+0x28)

#define CCSIDR 0xE000ED80
#define CSSELR 0xE000ED84
#define DCISW  0xE000EF60

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018

#define FLASH_BASE 0x40023C00
#define FLASH_ACR (FLASH_BASE+0x00)

//PD8 USART3 TX alternate function 7
//PD9 USART3 RX alternate function 7

static int clock_init ( void )
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
    while(1) if(((GET32(RCC_CFGR)>>2)&3)==1) break;

    return(0);
}

static int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHB1ENR);
    ra|=1<<3; //enable port D
    PUT32(RCC_AHB1ENR,ra);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<18; //enable UART_
    PUT32(RCC_APB1ENR,ra);

    ra=GET32(GPIOD_MODER);
    ra&=~(3<<(8<<1)); //PD8
    ra&=~(3<<(9<<1)); //PD9
    ra|= (2<<(8<<1)); //PD8
    ra|= (2<<(9<<1)); //PD9
    PUT32(GPIOD_MODER,ra);

    //ra=GET32(GPIOD_OTYPER);
    //ra&=~(1<<8); //PD8
    //ra&=~(1<<9); //PD9
    //PUT32(GPIOD_OTYPER,ra);

    ra=GET32(GPIOD_AFRH);
    ra&=~(0xF<<((8-8)<<2)); //PD8
    ra&=~(0xF<<((9-8)<<2)); //PD9
    ra|= (0x7<<((8-8)<<2)); //PD8
    ra|= (0x7<<((9-8)<<2)); //PD9
    PUT32(GPIOD_AFRH,ra);

    ra=GET32(RCC_APB1RSTR);
    ra|=1<<18; //reset UART_
    PUT32(RCC_APB1RSTR,ra);
    ra&=~(1<<18);
    PUT32(RCC_APB1RSTR,ra);

    //8000000/115200) = 69.444
    PUT32(UART_BRR,69);
    PUT32(UART_CR1,(1<<3)|(1<<2)|(1<<0));

    return(0);
}

static void uart_send ( unsigned int x )
{
    while(1) if(GET32(UART_ISR)&(1<<7)) break;
    PUT32(UART_TDR,x);
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

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;

    clock_init();
    uart_init();

    hexstrings(0x12345678); hexstring(0x12345678);
    hexstring(GET32(0xE000ED14));
    PUT32(0xE000ED14,0x00000200);
    hexstring(GET32(0xE000ED14));

    hexstring(GET32(0xE000E008));
    PUT32(0xE000E008,0x00003000);
    hexstring(GET32(0xE000E008));

    //hexstring(GET32(CCSIDR));
    //hexstring(GET32(CSSELR));
    //hexstring(GET32(DCISW));

    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,0xFFFFFFFF);
    PUT32(STK_CSR,0x00000005);

    hexstring(GET32(STK_CVR));
    hexstring(GET32(STK_CVR));
    hexstring(GET32(STK_CVR));
    hexstring(GET32(STK_CVR));
    //counts down.

    hexstrings(0x11111111); hexstring(0x11111111);

if(0)
{
    ra=TEST(0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=TEST(0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=TEST(0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=TEST(0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
}


if(1)
{
    ra=swtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=swtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=swtest(0x20002002,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=swtest(0x20002002,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);

    ra=lwtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=lwtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=lwtest(0x20002002,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=lwtest(0x20002002,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
}



//////08000100 <TEST>:
////// 8000100: 46c0        nop         ; (mov r8, r8)
////// 8000102: 46c0        nop         ; (mov r8, r8)
////// 8000104: 46c0        nop         ; (mov r8, r8)
////// 8000106: 46c0        nop         ; (mov r8, r8)
////// 8000108: 46c0        nop         ; (mov r8, r8)
////// 800010a: b430        push    {r4, r5}
////// 800010c: 680c        ldr r4, [r1, #0]
//////
//////0800010e <loop>:
////// 800010e: 3801        subs    r0, #1
////// 8000110: d1fd        bne.n   800010e <loop>
////// 8000112: 680d        ldr r5, [r1, #0]
////// 8000114: 1b60        subs    r0, r4, r5
////// 8000116: bc30        pop {r4, r5}
////// 8000118: 4770        bx  lr
//////


if(0)
{

    for(rd=0;rd<8;rd++)
    {
        rb=0x20002000;
        for(rc=0;rc<rd;rc++)
        {
            PUT32(rb,0xb430); rb+=2; //46c0         nop         ; (mov r8, r8)
        }

        PUT32(rb,0xb430); rb+=2; // 800010a:    b430        push    {r4, r5}
        PUT32(rb,0x680c); rb+=2; // 800010c:    680c        ldr r4, [r1, #0]
                                 //0800010e <loop>:
        PUT32(rb,0x3801); rb+=2; // 800010e:    3801        subs    r0, #1
        PUT32(rb,0xd1fd); rb+=2; // 8000110:    d1fd        bne.n   800010e <loop>
        PUT32(rb,0x680d); rb+=2; // 8000112:    680d        ldr r5, [r1, #0]
        PUT32(rb,0x1b60); rb+=2; // 8000114:    1b60        subs    r0, r4, r5
        PUT32(rb,0xbc30); rb+=2; // 8000116:    bc30        pop {r4, r5}
        PUT32(rb,0x4770); rb+=2; // 8000118:    4770        bx  lr
        PUT32(rb,0x46c0); rb+=2;
        PUT32(rb,0x46c0); rb+=2;
        PUT32(rb,0x46c0); rb+=2;
        PUT32(rb,0x46c0); rb+=2;
        PUT32(rb,0x46c0); rb+=2;
        PUT32(rb,0x46c0); rb+=2;

        ra=HOP(0x1000,STK_CVR,0x20002001);  hexstrings(rd); hexstring(ra%0x00FFFFFF);
        ra=HOP(0x1000,STK_CVR,0x20002001);  hexstrings(rd); hexstring(ra%0x00FFFFFF);
        ra=HOP(0x1000,STK_CVR,0x20002001);  hexstrings(rd); hexstring(ra%0x00FFFFFF);
        ra=HOP(0x1000,STK_CVR,0x20002001);  hexstrings(rd); hexstring(ra%0x00FFFFFF);

    }

}













    return(0);
}


