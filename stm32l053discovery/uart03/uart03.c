
//------------------------------------------------------------------------
//------------------------------------------------------------------------

void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
unsigned int ASMDELAY ( unsigned int, unsigned int );
unsigned int TESTFUN ( unsigned int, unsigned int );
unsigned int BRANCHTOR2 ( unsigned int, unsigned int, unsigned int );

#define USART1BASE  0x40013800

#define GPIOABASE   0x50000000
#define GPIOBBASE   0x50000400
#define RCCBASE     0x40021000

#define LPTIMER1BASE 0x40007C00
#define FLASHBASE   0x40022000

//------------------------------------------------------------------------
void clock_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x00);
    ra|=1<<0;
    PUT32(RCCBASE+0x00,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x00);
        if(ra&(1<<2)) break;
    }
    PUT32(RCCBASE+0x0C,1);
}
//------------------------------------------------------------------------
void uart_init ( void )
{
    unsigned int ra;

    //need HSI16 up
    //ra=GET32(RCCBASE+0x00);
    //ra|=1<<0;
    //PUT32(RCCBASE+0x00,ra);
    //while(1)
    //{
        //ra=GET32(RCCBASE+0x00);
        //if(ra&(1<<2)) break;
    //}

    ra=GET32(RCCBASE+0x2C);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x2C,ra);

    ra=GET32(RCCBASE+0x34);
    ra|=1<<14; //enable USART1
    PUT32(RCCBASE+0x34,ra);

    ra=GET32(RCCBASE+0x4C);
    ra&=~(3<<0);
    ra|=2<<0; //HSI16 clock for uart
    PUT32(RCCBASE+0x4C,ra);


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
    //afr4 0100
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<4); //PA9
    ra|=0x4<<4; //PA9 AF4 USART1_TX
    ra&=~(0xF<<8); //PA10
    ra|=0x4<<8; //PA10 AF4 USART1_RX
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x24);
    ra|=1<<14; //reset USART1
    PUT32(RCCBASE+0x24,ra);
    ra&=~(1<<14);
    PUT32(RCCBASE+0x24,ra);

    PUT32(USART1BASE+0x0C,139);
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|(1<<2)|1);
}
//------------------------------------------------------------------------
void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
    PUT32(USART1BASE+0x28,x);
}
//------------------------------------------------------------------------
unsigned int uart_recv ( void )
{
    while(1) if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    return(GET32(USART1BASE+0x24));
}
//------------------------------------------------------------------------
void hexstrings ( unsigned int d )
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
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    clock_init();
    uart_init();
    hexstring(0x12345678);


    ra=GET32(RCCBASE+0x38);
    ra|=1<<31; //enable lptimer
    PUT32(RCCBASE+0x38,ra);

    ra=GET32(RCCBASE+0x4C);
    ra&=~(3<<18);
    ra|=2<<18; //HSI16
    PUT32(RCCBASE+0x4C,ra);


    ra=GET32(RCCBASE+0x38);
    ra|=1<<4; //enable timer 6
    PUT32(RCCBASE+0x38,ra);
    //assume counter is disabled? and in post-reset state?

    PUT32(LPTIMER1BASE+0x10,0x0001);
    PUT32(LPTIMER1BASE+0x18,0x8000);
    PUT32(LPTIMER1BASE+0x1C,0x0000);
    PUT32(LPTIMER1BASE+0x10,0x0005);


//08000088 <ASMDELAY>:
 //8000088: 680a        ldr r2, [r1, #0]

//0800008a <ASMDELAY0>:
 //800008a: 46c0        nop         ; (mov r8, r8)
 //800008c: 46c0        nop         ; (mov r8, r8)
 //800008e: 46c0        nop         ; (mov r8, r8)
 //8000090: 3801        subs    r0, #1
 //8000092: d1fa        bne.n   800008a <ASMDELAY0>
 //8000094: 680b        ldr r3, [r1, #0]
 //8000096: 1a9b        subs    r3, r3, r2
 //8000098: 1c18        adds    r0, r3, #0
 //800009a: 4770        bx  lr

    ra=0x20000000;
    PUT16(ra,0x680a); ra+=2;
    PUT16(ra,0x46c0); ra+=2;
    PUT16(ra,0x46c0); ra+=2;
    PUT16(ra,0x46c0); ra+=2;
    PUT16(ra,0x3801); ra+=2;
    PUT16(ra,0xd1fa); ra+=2;
    PUT16(ra,0x680b); ra+=2;
    PUT16(ra,0x1a9b); ra+=2;
    PUT16(ra,0x1c18); ra+=2;
    PUT16(ra,0x4770); ra+=2;

    ra=BRANCHTOR2(100,LPTIMER1BASE+0x1C,0x20000001);
    hexstring(ra&0xFFFF);
    ra=ASMDELAY(100,LPTIMER1BASE+0x1C);
    hexstring(ra&0xFFFF);
    ra=ASMDELAY(200,LPTIMER1BASE+0x1C);
    hexstring(ra&0xFFFF);

    //flash wait state
    ra=GET32(FLASHBASE+0x00);
    ra|=1;
    PUT32(FLASHBASE+0x00,ra);
    ASMDELAY(1000,LPTIMER1BASE+0x1C);

    ra=BRANCHTOR2(100,LPTIMER1BASE+0x1C,0x20000001);
    hexstring(ra&0xFFFF);
    ra=ASMDELAY(100,LPTIMER1BASE+0x1C);
    hexstring(ra&0xFFFF);
    ra=ASMDELAY(200,LPTIMER1BASE+0x1C);
    hexstring(ra&0xFFFF);

    //disable pll
    ra=GET32(RCCBASE+0x00);
    ra&=~(1<<24);
    PUT32(RCCBASE+0x00,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x00);
        ra&=(1<<25);
        if(ra==0) break;
    }
    //setup pll parameters
    ra=GET32(RCCBASE+0x0C);
    ra&=~(3<<22);
    ra|=2<<22;
    ra&=~(0xF<<18);
    ra|=2<<18;
    ra&=~(1<<16);
    PUT32(RCCBASE+0x0C,ra);
    //turn on pll
    ra=GET32(RCCBASE+0x00);
    ra|=1<<24;
    PUT32(RCCBASE+0x00,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x00);
        if(ra&(1<<25)) break;
    }
    //use pll clock
    ra=GET32(RCCBASE+0x0C);
    //ra&=3;
    ra|=3; //PLL
    PUT32(RCCBASE+0x0C,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x0C);
        ra=(ra>>2)&3;
        if(ra==3) break; //PLL
    }
    //now running on pll clock
    //clock init finished

    ra=BRANCHTOR2(100,LPTIMER1BASE+0x1C,0x20000001);
    hexstring(ra&0xFFFF);
    ra=ASMDELAY(100,LPTIMER1BASE+0x1C);
    hexstring(ra&0xFFFF);
    ra=ASMDELAY(200,LPTIMER1BASE+0x1C);
    hexstring(ra&0xFFFF);

    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
