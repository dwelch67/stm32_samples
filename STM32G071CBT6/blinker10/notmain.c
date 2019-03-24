
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

void ienable ( void );
void DOWFI ( void );

#define GPIOBBASE 0x50000400
#define GPIOCBASE 0x50000800
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
    if(counter&1)
    {
        PUT32(GPIOCBASE+0x18,(1<<(6+ 0)) | (1<<(7+16)) );
    }
    else
    {
        PUT32(GPIOCBASE+0x18,(1<<(6+16)) | (1<<(7+ 0)) );
    }
    counter++;
}
//-------------------------------------------------------------------
//  **** WARNING THE CODE ABOVE IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------



int notmain ( void )
{
    unsigned int ra;
    //unsigned int rx;

    ra=GET32(RCCBASE+0x34);
    ra|=1<<1; //enable port b
    PUT32(RCCBASE+0x34,ra);

    //moder
    PUT32(GPIOBBASE+0x00,0xFFAAAAAA);
    //otyper
    PUT32(GPIOBBASE+0x04,0x00000F00);
    //pupdr
    PUT32(GPIOBBASE+0x0C,0x00AA0000);
    //bsrr
    PUT32(GPIOBBASE+0x18,0x0F0000FF);






    ra=GET32(RCCBASE+0x34);
    ra|=1<<2; //enable port c
    PUT32(RCCBASE+0x34,ra);

    //moder
    ra=GET32(GPIOCBASE+0x00);
    ra&=~(3<<(6<<1)); //PC6
    ra&=~(3<<(7<<1)); //PC7
    ra|= (1<<(6<<1)); //PC6
    ra|= (1<<(7<<1)); //PC7
    PUT32(GPIOCBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOCBASE+0x04);
    ra&=~(1<<6); //PC6
    ra&=~(1<<7); //PC7
    PUT32(GPIOCBASE+0x04,ra);






    PUT32(STK_CSR,4);
    PUT32(STK_RVR,16000000-1);
    PUT32(STK_CVR,16000000-1);
    PUT32(STK_CSR,7);
    ienable();

    DOWFI();

    return(0);
}
