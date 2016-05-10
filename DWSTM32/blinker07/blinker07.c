
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

#define TIMER14BASE 0x40002000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF


int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<8); //PA4
    ra|=(2<<8); //PA4 alternate function
    PUT32(GPIOABASE+0x00,ra);
    //AFRL
    ra=GET32(GPIOABASE+0x20);
    ra&=~(0xF<<16); //PA4
    ra|=4<<16; //PA4
    PUT32(GPIOABASE+0x20,ra);

    ra=GET32(RCCBASE+0x1c);
    ra|=1<<8; //enable timer 14
    PUT32(RCCBASE+0x1C,ra);


    PUT32(STK_CSR,4);
    PUT32(STK_RVR,100-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);


if(1)
{
    PUT16(TIMER14BASE+0x00,0x0000);
    PUT16(TIMER14BASE+0x2C,0xFFFF);
    PUT16(TIMER14BASE+0x24,0x0000);
    PUT16(TIMER14BASE+0x28,0x0000);
    PUT16(TIMER14BASE+0x18,0x0060);
    PUT16(TIMER14BASE+0x20,0x0001);
    PUT16(TIMER14BASE+0x34,0xFFFF);
    PUT16(TIMER14BASE+0x00,0x0001);

    while(1)
    {
        for(rx=0xFFFF;rx;rx--)
        {
            PUT16(TIMER14BASE+0x34,rx);
            while(1)
            {
                ra=GET32(STK_CSR);
                if(ra&(1<<16)) break;
            }
        }
        for(rx=0x0;rx<0xFFFF;rx++)
        {
            PUT16(TIMER14BASE+0x34,rx);
            while(1)
            {
                ra=GET32(STK_CSR);
                if(ra&(1<<16)) break;
            }
        }
    }
}



    //PUT16(TIMER14BASE+0x00,0x0000);



    return(0);
}
