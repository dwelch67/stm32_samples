

#define DEBUGUART

void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
void PUT8  ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET8 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE   0x48000000
#define RCCBASE     0x40021000
#define USART1BASE  0x40013800
#define SPI1BASE    0x40013000

void clock_init ( void )
{
    unsigned int ra;

    //HSI8 based 20MHz.  (8/2)*5
    ra=0;
    ra|=0x3<<18; //times 5
    ra|=0<<15;  //divide by 2
    PUT32(RCCBASE+0x04,ra);
    ra=GET32(RCCBASE+0x00);
    ra|=1<<24;
    PUT32(RCCBASE+0x00,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x00);
        if(ra&(1<<25)) break;
    }
    ra=GET32(RCCBASE+0x04);
    ra&=~3;
    ra|=2;
    PUT32(RCCBASE+0x04,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x04);
        if((ra&3)==2) break;
    }
}

void fifo_wait ( void )
{
    while(1)
    {
        if((GET16(SPI1BASE+0x08)&(1<<12))==0) break;
    }
}

unsigned short data[512];
unsigned int off;
unsigned int bit;

void add_bit ( unsigned int x )
{
    data[off]<<=1;
    data[off]|=(x&1);
    bit++;
    if(bit==16)
    {
        off++;
        bit=0;
    }
}
void add_one_color ( unsigned int c )
{
    unsigned int ra;
    for(ra=0x80;ra;ra>>=1)
    {
        if(c&ra)
        {
            add_bit(1);
            add_bit(1);
            add_bit(0);
        }
        else
        {
            add_bit(1);
            add_bit(0);
            add_bit(0);
        }
    }
}
void add_colors ( unsigned int g, unsigned int r, unsigned int b )
{
    add_one_color(g);
    add_one_color(r);
    add_one_color(b);
}

void run_it ( void )
{
    unsigned int ra;

    PUT16(SPI1BASE+0x0C,0x0000);
    for(ra=0;ra<off;ra++)
    {
        fifo_wait();
        PUT16(SPI1BASE+0x0C,data[ra]);
    }
    PUT16(SPI1BASE+0x0C,0x0000);
    for(ra=0;ra<40000;ra++) dummy(ra);
}
void round_off ( void )
{
            //add_bit(1);
            add_bit(0);
    while(bit!=0) add_bit(0);
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
    unsigned int rx;
    unsigned int rg;
    unsigned int xr,xg,xb;

    clock_init();

//PA4 AF0 SPI1_NSS
//PA5 AF0 SPI1_SCK
//PA6 AF0 SPI1_MISO
//PA7 AF0 SPI1_MOSI <---

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(7<<1)); //PA7
    ra|= (2<<(7<<1)); //PA7 alternate function
    PUT32(GPIOABASE+0x00,ra);
    //AFRL
    ra=GET32(GPIOABASE+0x20);
    ra&=~(0xF<<(7<<2)); //PA7
    ra|= (0x0<<(7<<2)); //PA7
    PUT32(GPIOABASE+0x20,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<12; //enable spi1
    PUT32(RCCBASE+0x18,ra);

    //PUT32(RCCBASE+0x04,(0xF<<4)|(7<<8)); //SLOOOWWWW

    PUT16(SPI1BASE+0x00,(0<<6)|(2<<3)|(1<<2)|0);
    PUT16(SPI1BASE+0x04,0x0F04);
    PUT16(SPI1BASE+0x00,(1<<6)|(2<<3)|(1<<2)|0);

rx=0;
while(1)
{

    for(rg=0;rg<24;rg++)
    {
        for(rb=0;rb<24;rb++)
        {

            off=0;
            bit=0;

            data[off++]=0x0000;
            for(rc=0;rc<24;rc++)
            {
                xr=0;
                xg=0;
                xb=0;
                if(rc==rb) xb|=1;
                if(rc==(23-rb)) xr|=1;
                if(rc==rg) xg|=1;
                add_colors(xg,xr,xb);
            }
            if(bit==16)
            {
                off++;
                bit=0;
            }
            while(bit!=0) add_bit(0);
            data[off++]=0x0000;
            run_it();
        }

    }


}


    return(0);
}


