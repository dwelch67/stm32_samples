
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x40010800
#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF


const unsigned int led_mask[16]=
{
    0x4E0031,
    0x4D0032,
    0x4B0034,
    0x470038,
    0x2E0051,
    0x2D0052,
    0x2B0054,
    0x270058,
    0x1E0061,
    0x1D0062,
    0x1B0064,
    0x170068,
    0x0000FF,
    0x0000FF,
    0x0000FF,
    0x0000FF
};
    
static int delay ( unsigned int n )
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
    unsigned int rb;
    unsigned int rx;
    unsigned int rz;

    
    ra=GET32(RCCBASE+0x18);
    ra|=1<<2; //enable port a
    PUT32(RCCBASE+0x18,ra);
    //config
    PUT32(GPIOABASE+0x00,0x33333333);
    PUT32(GPIOABASE+0x10,0x00FF0000);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,100-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);

if(0)
{
    rz=0;
    for(rx=0;;rx++)
    {
        if(rx>=0x800)
        {
            rx=0;
            rz++;
            if(rz>11) rz=0;
        }
        PUT32(GPIOABASE+0x10,0x0070000F);
        delay(1);
        PUT32(GPIOABASE+0x10,led_mask[rz&15]);
        delay(1);
        PUT32(GPIOABASE+0x10,0x000000FF);
        delay(20);
    }
}
else
if(0)
{
    //okay if they are not an even frequency, demonstrating
    //every combination, use case is not dependent on overall loop
    //time being even for every combination.
    rz=0;
    for(rx=0;;rx++)
    {
        if(rx>=0x100)
        {
            rx=0;
            rz++;
        }
        PUT32(GPIOABASE+0x10,0x0070000F);
        delay(1);
        for(ra=11,rb=0x800;rb;rb>>=1,ra--)
        {
            if(rz&rb)
            {
                PUT32(GPIOABASE+0x10,led_mask[ra]);
                delay(2);
            }
        }
        PUT32(GPIOABASE+0x10,0x000000FF);
        delay(20);
    }
}
else
if(0)
{
    //okay if they are not an even frequency, demonstrating
    //every combination, use case is not dependent on overall loop
    //time being even for every combination.
    rz=0;
    for(rx=0;;rx++)
    {
        if(rx>=0x100)
        {
            rx=0;
            rz++;
        }
        //PUT32(GPIOABASE+0x10,0x0070000F);
        //delay(1);
        for(ra=11,rb=0x800;rb;rb>>=1,ra--)
        {
            if(rz&rb)
            {
                PUT32(GPIOABASE+0x10,led_mask[ra]);
                delay(5);
            }
        }
        //PUT32(GPIOABASE+0x10,0x000000FF);
        //delay(20);
    }
}
else
{
    //okay if they are not an even frequency, demonstrating
    //every combination, use case is not dependent on overall loop
    //time being even for every combination.
    rz=0;
    rb=0x800;
    ra=11;
    for(rx=0;;rx++)
    {
        if(rx>=0x100)
        {
            rx=0;
            rz++;
        }
        PUT32(GPIOABASE+0x10,0x0070000F);
        delay(1);
        if(rz&rb)
        {
            PUT32(GPIOABASE+0x10,led_mask[ra]);
            delay(10);
        }
        PUT32(GPIOABASE+0x10,0x000000FF);
        delay(20);
        rb>>=1;
        ra--;
        if(rb==0)
        {
            rb=0x800;
            ra=11;
        }
    }
}
    return(0);
}
