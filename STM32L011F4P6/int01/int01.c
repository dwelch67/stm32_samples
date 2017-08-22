
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
void ienable ( void );
void DOWFI ( void );

#define GPIOABASE 0x50000000
#define GPIOBBASE 0x50000400

#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

//-------------------------------------------------------------------
//  **** WARNING THE CODE BELOW IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------
volatile unsigned int counter;
void systick_handler ( void )
{
    GET32(STK_CSR);

    switch(counter&0xF)
    {
        case 0:
            PUT32(GPIOBBASE+0x18, (1<<(1+16)) );
            PUT32(GPIOABASE+0x18, (1<<(4+ 0)) );
            break;
        case 1:
            PUT32(GPIOBBASE+0x18, (1<<(1+ 0)) );
            PUT32(GPIOABASE+0x18, (1<<(4+16)) );
            break;
        default:
            break;
    }
    counter++;
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


    counter=0;
    PUT32(STK_CSR,4);
    PUT32(STK_RVR,4000-1);
    PUT32(STK_CVR,4000-1);
    PUT32(STK_CSR,7);
    ienable();

    DOWFI();

    return(0);
}
