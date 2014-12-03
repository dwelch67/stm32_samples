
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018

void do_delay ( unsigned int sec )
{
    unsigned int ra,rb,rc,rd;

    rb=GET32(STK_CVR);
    for(rd=0;rd<sec;)
    {
        ra=GET32(STK_CVR);
        rc=(rb-ra)&0x00FFFFFF;
        if(rc>=8000000)
        {
            rb=ra;
            rd++;
        }
    }
}

int notmain ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port A
    PUT32(RCCBASE+0x14,ra);

    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<10); //PA5
    ra|=1<<10; //PA5
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<5); //PA5
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<10; //PA5
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<10); //PA5
    PUT32(GPIOABASE+0x0C,ra);

    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,0xFFFFFFFF);
    PUT32(STK_CSR,0x00000005);

    while(1)
    {
        PUT32(GPIOABASE+0x18,((1<<5)<<0));
        do_delay(10);
        PUT32(GPIOABASE+0x18,((1<<5)<<16));
        do_delay(1);
    }
    return(0);
}
