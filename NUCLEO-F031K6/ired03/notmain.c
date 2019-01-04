
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );


#define GPIOABASE 0x48000000
#define GPIOBBASE 0x48000400

#define RCCBASE 0x40021000

#define USART1BASE  0x40013800

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<14; //enable USART1
    PUT32(RCCBASE+0x18,ra);

    //PA2 UART1_TX
    //PA15 UART1_RX
    //moder 10
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<4); //PA2
    ra|=2<<4; //PA2
    ra&=~(3<<30); //PA15
    ra|=2<<30; //PA15
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER 0
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<2); //PA2
    ra&=~(1<<15); //PA2
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr 11
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<4; //PA2
    ra|=3<<30; //PA15
    PUT32(GPIOABASE+0x08,ra);
    //pupdr 00
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<4); //PA2
    ra&=~(3<<30); //PA15
    PUT32(GPIOABASE+0x0C,ra);
    //afr 0001
    ra=GET32(GPIOABASE+0x20);
    ra&=~(0xF<<8); //PA2
    ra|=0x1<<8; //PA2
    PUT32(GPIOABASE+0x20,ra);
    //afr 0001
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<28); //PA15
    ra|=0x1<<28; //PA15
    PUT32(GPIOABASE+0x24,ra);


    ra=GET32(RCCBASE+0x0C);
    ra|=1<<14; //reset USART1
    PUT32(RCCBASE+0x0C,ra);
    ra&=~(1<<14);
    PUT32(RCCBASE+0x0C,ra);

    PUT32(USART1BASE+0x0C,69);
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|(1<<2)|1);

    return(0);
}

static void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
    PUT32(USART1BASE+0x28,x);
}

//unsigned int uart_recv ( void )
//{
    //while(1) if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    //return(GET32(USART1BASE+0x24));
//}

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

    uart_init();
    hexstring(0x12345678);

    ra=GET32(RCCBASE+0x14);
    ra|=1<<18; //enable port b
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOBBASE+0x00);
    ra&=~(3<<6); //PB3
    ra|=1<<6; //PB3
    PUT32(GPIOBBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOBBASE+0x04);
    ra&=~(1<<3); //PB3
    PUT32(GPIOBBASE+0x04,ra);
    PUT32(GPIOBBASE+0x18,(1<<3));

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<0); //PA0
    //ra|=0<<6; //PA0
    PUT32(GPIOABASE+0x00,ra);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,0xFFFFFFFF);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);
    //counts down

if(0)
{
    unsigned int max;
    max=0;
    while(1)
    {
        //hexstring(GET32(STK_CVR));
        while((GET32(GPIOABASE+0x10)&1)==1) continue;
        ra=GET32(STK_CVR);
        while((GET32(GPIOABASE+0x10)&1)==0) continue;
        rb=GET32(STK_CVR);
        ra=(ra-rb)&STK_MASK;
        if(ra>max)
        {
            max=ra;
            hexstring(max);
        }
    }
}
if(0)
{
    unsigned int rc;
    unsigned int rd;
    while(1)
    {
        while(1)
        {
            //hexstring(GET32(STK_CVR));
            while((GET32(GPIOABASE+0x10)&1)==1) continue;
            ra=GET32(STK_CVR);
            while((GET32(GPIOABASE+0x10)&1)==0) continue;
            rb=GET32(STK_CVR);
            rc=(ra-rb)&STK_MASK;
            if(rc>0x11000) break;
        }
        while((GET32(GPIOABASE+0x10)&1)==1) continue;
        ra=GET32(STK_CVR);
        rd=(rb-ra)&STK_MASK;
        hexstrings(rc);
        hexstring(rd);
    }
}
if(1)
{
    //8000000*0.000889 = 7112
    
    unsigned int rc;
    unsigned int rd;
    unsigned int re;
    unsigned int data[70];
    while(1)
    {
        while(1)
        {
            //hexstring(GET32(STK_CVR));
            while((GET32(GPIOABASE+0x10)&1)==1) continue;
            ra=GET32(STK_CVR);
            while((GET32(GPIOABASE+0x10)&1)==0) continue;
            rb=GET32(STK_CVR);
            rc=(ra-rb)&STK_MASK;
            while((GET32(GPIOABASE+0x10)&1)==1) continue;
            ra=GET32(STK_CVR);
            re=(rb-ra)&STK_MASK;
            if(rc>7000)
            {
                if(rc<8000)
                {
                    break;
                }
            }
        }
        rd=0;
        data[rd++]=rc;
        data[rd++]=re;
        for(;rd<70;)
        {
            while((GET32(GPIOABASE+0x10)&1)==0) continue;
            rb=GET32(STK_CVR);
            rc=(ra-rb)&STK_MASK;
            data[rd++]=rc;
            while((GET32(GPIOABASE+0x10)&1)==1) continue;
            ra=GET32(STK_CVR);
            rc=(rb-ra)&STK_MASK;
            data[rd++]=rc;
        }
        //for(rc=0;rc<rd;rc++)
        //{
            //hexstrings(rc);
            //hexstring(data[rc]);
        //}
        re=0;
        ra=1;
        for(rc=0;rc<rd;rc++,ra++)
        {
            if(data[rc]>0x4000) break;
            if(data[rc]<7000) break;
            if(data[rc]>10000)
            {
                re<<=1; re|=(ra&1);
                re<<=1; re|=(ra&1);
            }
            else
            {
                re<<=1; re|=(ra&1);
            }
        }
        hexstring(re);
    }
}


if(0)
{
    unsigned int rc;
    unsigned int rd;
    unsigned int re;
    unsigned int data;
    while(1)
    {
        while(1)
        {
            //hexstring(GET32(STK_CVR));
            while((GET32(GPIOABASE+0x10)&1)==1) continue;
            ra=GET32(STK_CVR);
            while((GET32(GPIOABASE+0x10)&1)==0) continue;
            rb=GET32(STK_CVR);
            rc=(ra-rb)&STK_MASK;
            while((GET32(GPIOABASE+0x10)&1)==1) continue;
            ra=GET32(STK_CVR);
            re=(rb-ra)&STK_MASK;
            if((rc&0xFFFF000)==0x11000)
            if((re&0xFFFF000)==0x8000)
                break;
        }
        data=0;
        for(rd=0;rd<32;rd++)
        {
            while((GET32(GPIOABASE+0x10)&1)==0) continue;
            rb=GET32(STK_CVR);
            rc=(ra-rb)&STK_MASK;
            //data[rd++]=rc;
            while((GET32(GPIOABASE+0x10)&1)==1) continue;
            ra=GET32(STK_CVR);
            rc=(rb-ra)&STK_MASK;
            //data[rd++]=rc;
            data<<=1;
            if(rc>0x2000) data|=1;
        }
        hexstring(data);
    }
//JVC protocol.
//10AF906F on
//10AF40BF auto

//8.4ms  0x10680
//4.2ms  0x8340

//526+1574us = 1   0x1070, 0x3130 16800 0x41A0
//526+524 = 0      0x1070, 0x1060  8400 0x20D0
//38Khz carrier
//80 cycles is a 1  20 + 60
//40 cycles is a 0  20 + 20
// 8.4ms AGC burst (equivalent of 320 cycles of the 38kHz carrier)
// 4.2ms space (equivalent to 160 cycles)
}


    return(0);
}
