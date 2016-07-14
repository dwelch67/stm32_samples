
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOIBASE 0x40022000
#define RCCBASE 0x40023800

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

int delay ( unsigned int n )
{
    unsigned int ra;

    while(n--)
    {
        while(1)
        {
            ra=GET32(STK_CSR);
            if(ra&(1<<16)) break;
        }
    }
    return(0);
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;


    ra=GET32(RCCBASE+0x30);
    ra|=1<<8; //enable port i
    PUT32(RCCBASE+0x30,ra);
    //moder
    ra=GET32(GPIOIBASE+0x00);
    ra&=~(3<<2); //PI1
    ra|=1<<2; //PI1
    PUT32(GPIOIBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOIBASE+0x04);
    ra&=~(1<<1); //PI1
    PUT32(GPIOIBASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOIBASE+0x08);
    ra|=3<<2; //PI1
    PUT32(GPIOIBASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOIBASE+0x0C);
    ra&=~(3<<2); //PI1
    PUT32(GPIOIBASE+0x0C,ra);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,1000000-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);

    for(rx=0;;rx++)
    {
        PUT32(GPIOIBASE+0x18,(1<<(1+0)));
        delay(200);
        PUT32(GPIOIBASE+0x18,(1<<(1+16)));
        delay(200);
    }
    return(0);
}
