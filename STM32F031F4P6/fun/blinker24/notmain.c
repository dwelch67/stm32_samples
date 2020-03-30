
//-------------------------------------------------------------------
//-------------------------------------------------------------------

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
void ienable ( void );
void DOWFI ( void );

#define GPIOA_BASE       0x48000000
#define GPIOA_MODER     (GPIOA_BASE+0x00)
#define GPIOA_OTYPER    (GPIOA_BASE+0x04)
#define GPIOA_BSRR      (GPIOA_BASE+0x18)

#define RCC_BASE        0x40021000
#define RCC_AHBENR      (RCC_BASE+0x14)

#define STK_CSR         0xE000E010
#define STK_RVR         0xE000E014
#define STK_CVR         0xE000E018
#define STK_MASK        0x00FFFFFF

//-------------------------------------------------------------------
//  **** WARNING THE CODE BELOW IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------
#define COUNTMAX 100
volatile unsigned int counter;
volatile unsigned int countmid;
volatile unsigned int countstate;
void systick_handler ( void )
{
    GET32(STK_CSR);

    switch(countstate)
    {
        case 0:
        {
            if(counter==countmid)
            {
                PUT32(GPIOA_BSRR,0x00FF0000);
                counter = 0;
                countstate = 1;
            }
        }
        break;
        case 1:
        {
            if(counter==COUNTMAX)
            {
                PUT32(GPIOA_BSRR,0x000000FF);
                counter = 0;
                countstate = 0;

                countmid++;
                if(countmid==COUNTMAX)
                {
                    countmid--;
                    countstate = 2;
                }
            }
        }
        break;
        case 2:
        {
            if(counter==countmid)
            {
                PUT32(GPIOA_BSRR,0x00FF0000);
                counter = 0;
                countstate = 3;
            }
        }
        break;
        case 3:
        {
            if(counter==COUNTMAX)
            {
                PUT32(GPIOA_BSRR,0x000000FF);
                counter = 0;
                countstate = 2;

                countmid--;
                if(countmid==0)
                {
                    countmid++;
                    countstate = 0;
                }
            }
        }
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

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //enable port a
    PUT32(RCC_AHBENR,ra);

    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    ra&=~(3<<(4<<1)); //PA4
    ra&=~(3<<(5<<1)); //PA5
    ra&=~(3<<(6<<1)); //PA6
    ra&=~(3<<(7<<1)); //PA7
    ra|= (1<<(0<<1)); //PA0
    ra|= (1<<(1<<1)); //PA1
    ra|= (1<<(2<<1)); //PA2
    ra|= (1<<(3<<1)); //PA3
    ra|= (1<<(4<<1)); //PA4
    ra|= (1<<(5<<1)); //PA5
    ra|= (1<<(6<<1)); //PA6
    ra|= (1<<(7<<1)); //PA7
    PUT32(GPIOA_MODER,ra);

    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<0); //PA0
    ra&=~(1<<1); //PA1
    ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    ra&=~(1<<4); //PA4
    ra&=~(1<<5); //PA5
    ra&=~(1<<6); //PA6
    ra&=~(1<<7); //PA7
    PUT32(GPIOA_OTYPER,ra);

    PUT32(GPIOA_BSRR,0x000000FF);

    countstate = 0;
    counter = 0;
    countmid = 1;
    PUT32(STK_CSR,4);
    PUT32(STK_RVR,1000-1);
    PUT32(STK_CVR,1000-1);
    PUT32(STK_CSR,7);
    ienable();

    while(1) continue;

    return(0);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
