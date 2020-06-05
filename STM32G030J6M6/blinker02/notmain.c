
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE   0x50000000
#define RCCBASE     0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018

static void do_delay ( unsigned int sec )
{
    unsigned int ra,rb;

    sec<<=2;
    for(ra=0;ra<sec;ra++)
    {
        while(1)
        {
            rb=GET32(STK_CSR);
            if(rb&(1<<16))
            {
                break;
            }
        }

    }
}


int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x34);
    ra|=1<<0; //enable port A
    PUT32(RCCBASE+0x34,ra);

    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(13<<1)); //PB13
    ra|= (1<<(13<<1)); //PB13
    PUT32(GPIOABASE+0x00,ra);

    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,0xFFFFFFFF);
    PUT32(STK_CSR,0x00000005);

    for(rx=0;rx<5;rx++)
    {
        PUT32(GPIOABASE+0x18,((1<<13)<< 0));
        while(1) if((GET32(STK_CVR)&0x200000)!=0) break;
        PUT32(GPIOABASE+0x18,((1<<13)<<16));
        while(1) if((GET32(STK_CVR)&0x200000)==0) break;
    }

    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,4000000-1);
    PUT32(STK_CVR,4000000-1);
    PUT32(STK_CSR,0x00000005);

    while(1)
    {
        PUT32(GPIOABASE+0x18,((1<<13)<< 0));
        do_delay(4);
        PUT32(GPIOABASE+0x18,((1<<13)<<16));
        do_delay(4);
    }

    return(0);
}
