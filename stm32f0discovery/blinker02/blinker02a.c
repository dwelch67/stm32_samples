
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOCBASE 0x48000800
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
    ra|=1<<19; //enable port C
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOCBASE+0x00);
    ra&=~(3<<16); //PC8
    ra&=~(3<<18); //PC9
    ra|=1<<16; //PC8
    ra|=1<<18; //PC9
    PUT32(GPIOCBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOCBASE+0x04);
    ra&=~(1<<8); //PC8
    ra&=~(1<<9); //PC9
    PUT32(GPIOCBASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOCBASE+0x08);
    ra|=3<<16; //PC8
    ra|=3<<18; //PC8
    PUT32(GPIOCBASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOCBASE+0x0C);
    ra&=~(3<<16); //PC8
    ra&=~(3<<18); //PC9
    PUT32(GPIOCBASE+0x0C,ra);

    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,0xFFFFFFFF);
    PUT32(STK_CSR,0x00000005);

    while(1)
    {
        PUT32(GPIOCBASE+0x18,0x01000200);
        do_delay(10);
        PUT32(GPIOCBASE+0x18,0x02000100);
        do_delay(1);
    }
    return(0);
}
