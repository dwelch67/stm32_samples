
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x50000000

#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF


int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    //SLOOOOW...
    ra=GET32(RCCBASE+0x04);
    ra&=~(7<<13);
    PUT32(RCCBASE+0x04,ra);

    ra=GET32(RCCBASE+0x2C);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x2C,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<8); //PA4
    ra&=~(3<<10); //PA5
    ra|=1<<8; //PA5
    ra|=1<<10; //PA5
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<4); //PA4
    ra&=~(1<<5); //PA5
    PUT32(GPIOABASE+0x04,ra);

    for(rx=0;rx<3;rx++)
    {
        PUT32(GPIOABASE+0x18, (1<<(4+ 0)) | (1<<(5+16)) );
        for(ra=0;ra<10;ra++) dummy(ra);
        PUT32(GPIOABASE+0x18, (1<<(4+16)) | (1<<(5+16)) );
        for(ra=0;ra<4000;ra++) dummy(ra);
        PUT32(GPIOABASE+0x18, (1<<(4+16)) | (1<<(5+ 0)) );
        for(ra=0;ra<10;ra++) dummy(ra);
        PUT32(GPIOABASE+0x18, (1<<(4+16)) | (1<<(5+16)) );
        for(ra=0;ra<4000;ra++) dummy(ra);
    }

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,10000-1);
    PUT32(STK_CVR,0);
    PUT32(STK_CSR,5);

    while(1)
    {
        PUT32(GPIOABASE+0x18, (1<<(4+ 0)) | (1<<(5+16)) );
        while(1)
        {
            if(GET32(STK_CSR)&0x10000) break;
        }
        PUT32(GPIOABASE+0x18, (1<<(4+16)) | (1<<(5+ 0)) );
        while(1)
        {
            if(GET32(STK_CSR)&0x10000) break;
        }
    }
    return(0);
}
