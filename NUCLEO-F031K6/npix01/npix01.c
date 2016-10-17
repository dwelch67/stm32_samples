

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

void HOP ( unsigned int, unsigned int, unsigned int, unsigned int );

#define GPIOBBASE 0x48000400
#define RCCBASE 0x40021000

//------------------------------------------------------------------------
static unsigned int hopper;
static void send_24 ( unsigned int x )
{
    unsigned int ra;
    //unsigned int rd;
    unsigned int xa;
    unsigned int xb;
    unsigned int rb;

    for(rb=0x00800000;rb;rb>>=1)
    {
        if(x&rb)
        {
            xa=3;
            xb=1;
        }
        else
        {
            xa=1;
            xb=3;
        }
        PUT16(hopper,0x6002); hopper+=2;
        for(ra=0;ra<xa;ra++) { PUT16(hopper,0x46C0); hopper+=2; }
        PUT16(hopper,0x6001); hopper+=2;
        for(ra=0;ra<xb;ra++) { PUT16(hopper,0x46C0); hopper+=2; }
    }
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rd;
    unsigned int rx;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<18; //enable port b
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOBBASE+0x00);
    ra&=~(3<<6); //PB3
    ra|=1<<6; //PB3
    PUT32(GPIOBBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOBBASE+0x04);
    ra&=~(1<<3); //PB3
    PUT32(GPIOBBASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOBBASE+0x08);
    ra|=3<<6; //PB3
    PUT32(GPIOBBASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOBBASE+0x0C);
    ra&=~(3<<6); //PB3
    PUT32(GPIOBBASE+0x0C,ra);

    PUT32(GPIOBBASE+0x18,(1<<(3+16))); //off
    for(ra=0;ra<100000;ra++) dummy(ra);

    for(rx=0x10;;rx++)
    {
        rd=0;
        if(rx&0x10) rd|=0x000001;
        if(rx&0x20) rd|=0x000100;
        if(rx&0x40) rd|=0x010000;
        hopper=0x20000800;
        if(rx&1) send_24(rd); else send_24(0);
        if(rx&2) send_24(rd); else send_24(0);
        if(rx&4) send_24(rd); else send_24(0);
        if(rx&8) send_24(rd); else send_24(0);
        PUT16(hopper,0x4770);
        HOP(GPIOBBASE+0x18,1<<(3+16),1<<(3+0),0x20000801);
        for(ra=0;ra<100000;ra++) dummy(ra);
    }
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
