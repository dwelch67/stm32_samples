
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x50000000
#define GPIOBBASE 0x50000400
#define RCCBASE 0x40021000


#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF


#define TIMER6BASE 0x40001000
//#define TIMER7BASE 0x40001400

void delay ( unsigned int n )
{
    unsigned int ra;

    while(n--)
    {
        while(1)
        {
            ra=GET32(TIMER6BASE+0x24);
            if((ra&(1<<15))!=0) break;
        }
        while(1)
        {
            ra=GET32(TIMER6BASE+0x24);
            if((ra&(1<<15))==0) break;
        }
    }
}


int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x00);
    ra|=1<<0;
    PUT32(RCCBASE+0x00,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x00);
        if(ra&(1<<2)) break;
    }
    //ra=GET32(RCCBASE+0x0C);
    //ra&=3;
    //ra|=1;
    //PUT32(RCCBASE+0x0C,ra);
    PUT32(RCCBASE+0x0C,1);


    ra=GET32(RCCBASE+0x2C);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x2C,ra);
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

    ra=GET32(RCCBASE+0x2C);
    ra|=1<<1; //enable port b
    PUT32(RCCBASE+0x2C,ra);
    //moder
    ra=GET32(GPIOBBASE+0x00);
    ra&=~(3<<8); //PB4
    ra|=1<<8; //PB4
    PUT32(GPIOBBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOBBASE+0x04);
    ra&=~(1<<4); //PB4
    PUT32(GPIOBBASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOBBASE+0x08);
    ra|=3<<8; //PB4
    PUT32(GPIOBBASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOBBASE+0x0C);
    ra&=~(3<<8); //PB4
    PUT32(GPIOBBASE+0x0C,ra);


    ra=GET32(RCCBASE+0x38);
    ra|=1<<4; //enable timer 6
    PUT32(RCCBASE+0x38,ra);
    //assume counter is disabled? and in post-reset state?

    PUT32(TIMER6BASE+0x2C,0xFFFF);
    PUT32(TIMER6BASE+0x24,0x0000);
    PUT32(TIMER6BASE+0x00,0x0001);

    for(rx=0;;rx++)
    {
        PUT32(GPIOABASE+0x18,(1<<5));
        PUT32(GPIOBBASE+0x18,(1<<4));
        delay(1000);
        PUT32(GPIOABASE+0x18,(1<<(5+16)));
        PUT32(GPIOBBASE+0x18,(1<<(4+16)));
        delay(1000);
    }
    return(0);
}
