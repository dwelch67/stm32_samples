
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

unsigned int lwtest ( unsigned int, unsigned int, unsigned int);
unsigned int lbtest ( unsigned int, unsigned int, unsigned int);
unsigned int swtest ( unsigned int, unsigned int, unsigned int);
unsigned int shtest ( unsigned int, unsigned int, unsigned int);
unsigned int sbtest ( unsigned int, unsigned int, unsigned int);
unsigned int nwtest ( unsigned int, unsigned int, unsigned int);
unsigned int nbtest ( unsigned int, unsigned int, unsigned int);

void DSB ( void );
void ISB ( void );
void DISB ( void );
void PDISB ( unsigned int, unsigned int );


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

    clock_init();
    uart_init();

    hexstring(GET32(CCSIDR));
    hexstring(GET32(CSSELR));
    hexstring(GET32(DCISW));

    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,0xFFFFFFFF);
    PUT32(STK_CSR,0x00000005);

    hexstring(GET32(STK_CVR));
    hexstring(GET32(STK_CVR));
    hexstring(GET32(STK_CVR));
    hexstring(GET32(STK_CVR));
    //counts down.



/* r0 address */
/* r1 count */
/* r2 timer address */

    hexstrings(0x11111111); hexstring(0x11111111);
    ra=lwtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=lwtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=lbtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=lbtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);

    //PUT32(FLASH_ACR,0x0000000F);


    hexstrings(0x11111111); hexstring(0x11112222);
    ra=lwtest(0x20002000,0x1000,STK_CVR); hexstring(ra%0x00FFFFFF);
    ra=lwtest(0x20002000,0x1000,STK_CVR); hexstring(ra%0x00FFFFFF);
    ra=lbtest(0x20002000,0x1000,STK_CVR); hexstring(ra%0x00FFFFFF);
    ra=lbtest(0x20002000,0x1000,STK_CVR); hexstring(ra%0x00FFFFFF);

    //invalidate cache
    PDISB(ICIALLU,0);
    //enable I-cache
    ra=GET32(CCR);
    ra=ra|(1<<17); //i-cache
    PDISB(CCR,ra);

    hexstrings(0x11111111); hexstring(0x11112222);
    ra=lwtest(0x20002000,0x1000,STK_CVR); hexstring(ra%0x00FFFFFF);
    ra=lwtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=lbtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=lbtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);

    PUT32(CSSELR,0); //make data cache visible.
    for(ra=0;ra<4;ra++)
    {
        for(rb=0;rb<0x200;rb++)
        {
            PUT32(DCISW,(ra<<30)|(rb<<5));
        }
    }
    DISB();

    //enable d-cache
    ra=GET32(CCR);
    ra=ra|(1<<16); //d-cache
    PDISB(CCR,ra);


/*
In an error-free system, the major performance impact is the cost of
the read-modify-write scheme for non-full stores in the data side. If
a store buffer slot does not contain at least a full 32-bit word, it
must read the word to be able to compute the check bits. This can occur
because software only writes to an area of memory with byte or halfword
store instructions. The data can then be written in the RAM. This
additional read can have a negative impact on performance because it
prevents the slot from being used for another write.



The buffering and outstanding capabilities of the memory system mask
part of the additional read, and it is negligible for most codes.
However, ARM recommends that you use as few cacheable STRB and STRH
instructions as possible to reduce the performance impact.
*/

    hexstrings(0x11111111); hexstring(0x11112222);
    ra=lwtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=lwtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=lbtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=lbtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=swtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=swtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=shtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=shtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=sbtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=sbtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);

//0001000B                                                                        
//00010007                                                                        
//0001000B                                                                        
//00010007                                                                        
//0001000C                                                                        
//00010007                                                                        
//0002FFFD                                                                        
//0002FFFD                                                                        
//0002FFFD                                                                        
//0002FFFD

//byte write is 3 times longer

    hexstrings(0x11111111); hexstring(0x11112222);
    ra=nwtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=nwtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=nbtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);
    ra=nbtest(0x20002000,0x1000,STK_CVR);  hexstring(ra%0x00FFFFFF);

//0000C00B                                                                        
//0000C007                                                                        
//0000C00B                                                                        
//0000C007 

    return(0);
}


