
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
void ienable ( void );
void DOWFI ( void );
void DOVTOR ( unsigned int, unsigned int );

#define GPIOABASE 0x50000000
#define GPIOBBASE 0x50000400

#define RCCBASE 0x40021000

#define FLASHBASE 0x40022000

#define PWRBASE 0x40007000
#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#define VTOR 0xE000ED08
//-------------------------------------------------------------------
//  **** WARNING THE CODE BELOW IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------
void systick_handler ( void )
{
    unsigned int ra;

    PUT32(GPIOBBASE+0x18, (1<<(1+16)) );
    PUT32(GPIOABASE+0x18, (1<<(4+ 0)) | (1<<(5+ 0)) );
    for(ra=0;ra<10;ra++) dummy(ra);
    PUT32(GPIOBBASE+0x18, (1<<(1+ 0)) );
    PUT32(GPIOABASE+0x18, (1<<(4+16)) | (1<<(5+16)) );
    GET32(STK_CSR);
}
//-------------------------------------------------------------------
//  **** WARNING THE CODE ABOVE IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------

int notmain ( void )
{
    unsigned int ra;

    //SLOOOOW...
    ra=GET32(RCCBASE+0x04);
    ra&=~(7<<13);
    PUT32(RCCBASE+0x04,ra);



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



    DOVTOR(VTOR,0x20000000);

    ra=GET32(RCCBASE+0x2C);
    ra|=1<<1; //enable port b
    PUT32(RCCBASE+0x2C,ra);
    //moder
    ra=GET32(GPIOBBASE+0x00);
    ra&=~(3<<2); //PB1
    ra|=  1<<2; //PB1
    PUT32(GPIOBBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOBBASE+0x04);
    ra&=~(1<<1); //PB1
    PUT32(GPIOBBASE+0x04,ra);
    PUT32(GPIOBBASE+0x18, (1<<(1+ 0)) );



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


    PUT32(STK_CSR,4);
    PUT32(STK_RVR,0x40000-1);
    PUT32(STK_CVR,0x40000-1);
    PUT32(STK_CSR,7);
    ienable();

    DOWFI();

    return(0);
}
