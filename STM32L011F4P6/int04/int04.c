
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
void ienable ( void );
void DOWFI ( void );
void DOVTOR ( unsigned int, unsigned int );
#define GPIOABASE 0x50000000

#define RCCBASE 0x40021000

#define FLASHBASE 0x40022000

#define PWRBASE 0x40007000
#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#define VTOR 0xE000ED08


unsigned int nextprand ( unsigned int x )
{
    if(x&1)
    {
        x=x>>1;
        x=x^0xBF9EC099;
    }
    else
    {
        x=x>>1;
    }
    return(x);
}

//-------------------------------------------------------------------
//  **** WARNING THE CODE BELOW IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------
volatile unsigned int prand;
volatile unsigned int fourfive;
volatile unsigned int counter;
void systick_handler ( void )
{
    unsigned int ra;

    GET32(STK_CSR);
    if(counter==0)
    {
        if(fourfive&1)
        {
            PUT32(GPIOABASE+0x18, (1<<(4+ 0)) );
            for(ra=0;ra<10;ra++) dummy(ra);
            PUT32(GPIOABASE+0x18, (1<<(4+16)) );
        }
        else
        {
            PUT32(GPIOABASE+0x18, (1<<(5+ 0)) );
            for(ra=0;ra<10;ra++) dummy(ra);
            PUT32(GPIOABASE+0x18, (1<<(5+16)) );
        }
        fourfive++;
        prand=nextprand(prand);
        counter=80+(prand&0x1F);
    }
    else
    {
        counter--;
    }
}
//-------------------------------------------------------------------
//  **** WARNING THE CODE ABOVE IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------

int notmain ( void )
{
    unsigned int ra;

    //SLOOOOW...
    //ra=GET32(RCCBASE+0x04);
    //ra&=~(7<<13);
    //PUT32(RCCBASE+0x04,ra);

    //while(1)
    //{
        //if((GET32(PWRBASE+0x04)&(1<<4))==0) break;
    //}
    //ra=GET32(PWRBASE+0x00);
    //ra|=3<<11;
    //PUT32(PWRBASE+0x00,ra);
    //while(1)
    //{
        //if((GET32(PWRBASE+0x04)&(1<<4))==0) break;
    //}

    //shut down flash
    PUT32(FLASHBASE+0x08,0x04152637);
    PUT32(FLASHBASE+0x08,0xFAFBFCFD);
    PUT32(FLASHBASE+0x00,0x00000018);

    //SLOOOOW...
    ra=GET32(RCCBASE+0x04);
    ra&=~(7<<13);
    PUT32(RCCBASE+0x04,ra);

    DOVTOR(VTOR,0x20000000);

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

    PUT32(GPIOABASE+0x18, (1<<(4+16)) || (1<<(5+16)) );

    prand=0x12345;
    fourfive=0;
    counter=0;
    PUT32(STK_CSR,4);
    PUT32(STK_RVR,0x1000-1);
    PUT32(STK_CVR,0x1000-1);
    PUT32(STK_CSR,7);
    ienable();

    DOWFI();

    return(0);
}
