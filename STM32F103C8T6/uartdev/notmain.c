
//PA9  TX
//PA10 RX

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define GPIOBBASE 0x40010C00
#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#define PERIPH_BB_BASE 0x42000000

#define USART1_BASE 0x40013800
#define USART1_SR   (USART1_BASE+0x00)
#define USART1_DR   (USART1_BASE+0x04)
#define USART1_BRR  (USART1_BASE+0x08)
#define USART1_CR1  (USART1_BASE+0x0C)
#define USART1_CR2  (USART1_BASE+0x10)
#define USART1_CR3  (USART1_BASE+0x14)
#define USART1_GTPR (USART1_BASE+0x18)

static void uart_putc ( unsigned int c )
{
    while(1)
    {
        if(GET32(USART1_SR)&0x80) break;
    }
    PUT32(USART1_DR,c);
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
        uart_putc(rc);
        if(rb==0) break;
    }
    uart_putc(0x20);
}
static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_putc(0x0D);
    uart_putc(0x0A);
}

static unsigned int b12input;
static unsigned int b12output;
static unsigned int b12odrbb;
static unsigned int b12idrbb;

static void pull_up ( void )
{
    PUT32(GPIOBBASE+0x04,b12input);
    PUT32(b12odrbb,1);
}
static void pull_down ( void )
{
    PUT32(GPIOBBASE+0x04,b12output);
    PUT32(b12odrbb,0);
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;


    hexstring(0xAAAAAAAA);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,0xFFFFFFFF);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<3; //enable port b
    PUT32(RCCBASE+0x18,ra);
    //config
    ra=GET32(GPIOBBASE+0x04);
    b12input =(ra&~(0xF<<16))|(0x8<<16);
    b12output=(ra&~(0xF<<16))|(0x5<<16);
    b12odrbb=PERIPH_BB_BASE|(((GPIOBBASE+0x0C)&0x000FFFFF)<<5)|(12<<2);
    b12idrbb=PERIPH_BB_BASE|(((GPIOBBASE+0x08)&0x000FFFFF)<<5)|(12<<2);

    pull_down();
    //480us 3840
    rb=GET32(STK_CVR);
    while(1)
    {
        ra=GET32(STK_CVR);
        rc=(rb-ra)&STK_MASK; //count down
        if(rc>4000) break;
    }
    pull_up();
    rb=GET32(STK_CVR);
    while(1)
    {
        if(GET32(b12idrbb)==0) break;
    }
    ra=GET32(STK_CVR);
    while(1)
    {
        if(GET32(b12idrbb)) break;
    }
    rc=GET32(STK_CVR);
    hexstring((rb-ra)&STK_MASK); //count down
    hexstring((rb-rc)&STK_MASK); //count down

    return(0);
}
