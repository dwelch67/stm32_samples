
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

#define TIMER14BASE 0x40002000


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
    ra&=~(3<<14); //PA7
    ra|=(2<<14); //PA7 alternate function
    PUT32(GPIOABASE+0x00,ra);
    //AFRL
    ra=GET32(GPIOABASE+0x20);
    ra&=~(0xF<<16); //PA4
    ra|=4<<16; //PA4
    ra&=~(0xF<<28); //PA7
    ra|=4<<28; //PA7
    PUT32(GPIOABASE+0x20,ra);

    ra=GET32(RCCBASE+0x1c);
    ra|=1<<8; //enable timer 14
    PUT32(RCCBASE+0x1C,ra);


if(1)
{

    PUT16(TIMER14BASE+0x00,0x0000);
    PUT16(TIMER14BASE+0x2C,0xFFFF);
    PUT16(TIMER14BASE+0x24,0x0000);
    PUT16(TIMER14BASE+0x28,0x003F);
    //PUT16(TIMER14BASE+0x18,0x0030);
    PUT16(TIMER14BASE+0x20,0x0001);
    //PUT16(TIMER14BASE+0x34,0x0000);
    PUT16(TIMER14BASE+0x00,0x0001);

    for(rx=0;rx<4;rx++)
    {
        PUT16(TIMER14BASE+0x18,0x0050);
        while(1)
        {
            ra=GET16(TIMER14BASE+0x24);
            if((ra&(1<<15))!=0) break;
        }
        PUT16(TIMER14BASE+0x18,0x0040);
        while(1)
        {
            ra=GET16(TIMER14BASE+0x24);
            if((ra&(1<<15))==0) break;
        }
    }
}



if(1)
{

    PUT16(TIMER14BASE+0x00,0x0000);
    PUT16(TIMER14BASE+0x2C,0xFFFF);
    PUT16(TIMER14BASE+0x24,0x0000);
    PUT16(TIMER14BASE+0x28,0x003F);
    PUT16(TIMER14BASE+0x18,0x0030);
    PUT16(TIMER14BASE+0x20,0x0001);
    //PUT16(TIMER14BASE+0x34,0x0000);
    PUT16(TIMER14BASE+0x00,0x0001);

    for(rx=0;rx<6;rx++)
    {
        while(1)
        {
            ra=GET16(TIMER14BASE+0x10);
            if(ra)
            {
                PUT16(TIMER14BASE+0x10,0);
                break;
            }
        }
    }

}


if(1)
{
    PUT16(TIMER14BASE+0x00,0x0000);
    PUT16(TIMER14BASE+0x2C,0x7FFF);
    PUT16(TIMER14BASE+0x24,0x0000);
    PUT16(TIMER14BASE+0x28,0x003F);
    PUT16(TIMER14BASE+0x18,0x0030);
    PUT16(TIMER14BASE+0x20,0x0001);
    //PUT16(TIMER14BASE+0x34,0x0000);
    PUT16(TIMER14BASE+0x00,0x0001);

    for(rx=0;rx<6;rx++)
    {
        while(1)
        {
            ra=GET16(TIMER14BASE+0x10);
            if(ra)
            {
                PUT16(TIMER14BASE+0x10,0);
                break;
            }
        }
    }
}



if(1)
{
    PUT16(TIMER14BASE+0x00,0x0000);
    PUT16(TIMER14BASE+0x2C,0x7FFF);
    PUT16(TIMER14BASE+0x24,0x0000);
    PUT16(TIMER14BASE+0x28,0x003F);
    PUT16(TIMER14BASE+0x18,0x0030);
    PUT16(TIMER14BASE+0x20,0x0001);
    PUT16(TIMER14BASE+0x34,0x8000);
    PUT16(TIMER14BASE+0x00,0x0001);

    for(rx=0;rx<6;rx++)
    {
        while(1)
        {
            ra=GET16(TIMER14BASE+0x10);
            if(ra)
            {
                PUT16(TIMER14BASE+0x10,0);
                break;
            }
        }
    }
}



    //PUT16(TIMER14BASE+0x00,0x0000);



    return(0);
}
