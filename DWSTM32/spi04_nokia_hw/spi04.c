
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void PUT8 ( unsigned int, unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

#define SPI1BASE 0x40013000

#define SET_DC      (1<<(3+0))
#define RESET_DC    (1<<(3+16))

static const unsigned char font_data[]=
{
0xE, //###.
0xA, //#.#.
0xA, //#.#.
0xA, //#.#.
0xE, //###.

0x2, //..#.
0x2, //..#.
0x2, //..#.
0x2, //..#.
0x2, //..#.

0xE, //###.
0x2, //..#.
0xE, //###.
0x8, //#...
0xE, //###.

0xE, //###.
0x2, //..#.
0xE, //###.
0x2, //..#.
0xE, //###.

0xA, //#.#.
0xA, //#.#.
0xE, //###.
0x2, //..#.
0x2, //..#.

0xE, //###.
0x8, //#...
0xE, //###.
0x2, //..#.
0xE, //###.

0xE, //###.
0x8, //#...
0xE, //###.
0xA, //#.#.
0xE, //###.

0xE, //###.
0x2, //..#.
0x2, //..#.
0x2, //..#.
0x2, //..#.

0xE, //###.
0xA, //#.#.
0xE, //###.
0xA, //#.#.
0xE, //###.

0xE, //###.
0xA, //#.#.
0xE, //###.
0x2, //..#.
0x2, //..#.

0x0, //....
0x0, //....
0x0, //....
0x0, //....
0x0, //....

0x0, //....
0x0, //....
0x0, //....
0x0, //....
0x0, //....

0x0, //....
0x0, //....
0x0, //....
0x0, //....
0x0, //....

0x0, //....
0x0, //....
0x0, //....
0x0, //....
0x0, //....

0x0, //....
0x0, //....
0x0, //....
0x0, //....
0x0, //....

0x0, //....
0x0, //....
0x0, //....
0x0, //....
0x0, //....
};


static void spi_dc ( unsigned int x )
{
    if(x) PUT32(GPIOABASE+0x18,SET_DC);
    else  PUT32(GPIOABASE+0x18,RESET_DC);
}

static void spi_command ( unsigned char cmd )
{
    spi_dc(0);
    PUT8(SPI1BASE+0x0C,cmd);
    while(1)
    {
        if((GET16(SPI1BASE+0x08)&(1<<7))==0) break;
    }
}

static void spi_data ( unsigned char data )
{
    spi_dc(1);
    PUT8(SPI1BASE+0x0C,data);
    while(1)
    {
        if((GET16(SPI1BASE+0x08)&(1<<7))==0) break;
    }
}

static unsigned int base_ten ( unsigned int x )
{
    unsigned int rb;
    unsigned int num,res,acc,den;

    num=x;
    res=0;
    acc=0;
    den=10;
    for(rb=0x80000000;rb;rb>>=1)
    {
        acc<<=1;
        if(rb&num) acc|=1;
        if(acc>=den)
        {
            acc-=den;
            res|=rb;
        }
    }
    //printf("%u %u %u ",ra,res,acc);
    return((res<<4)|acc);
    //return(0);
}
int notmain ( void )
{
    unsigned int ra;

//PA3 GPIO D/C

//PA4 AF0 SPI1_NSS
//PA5 AF0 SPI1_SCK
//PA6 AF0 SPI1_MISO
//PA7 AF0 SPI1_MOSI

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<8); //PA4
    ra|=(2<<8); //PA4 alternate function
    ra&=~(3<<10); //PA5
    ra|=(2<<10); //PA5 alternate function
    ra&=~(3<<12); //PA6
    ra|=(2<<12); //PA6 alternate function
    ra&=~(3<<14); //PA7
    ra|=(2<<14); //PA7 alternate function
    PUT32(GPIOABASE+0x00,ra);
    //AFRL
    ra=GET32(GPIOABASE+0x20);
    ra&=~(0xF<<16); //PA4
    ra|=0<<16; //PA4
    ra&=~(0xF<<20); //PA5
    ra|=0<<20; //PA5
    ra&=~(0xF<<24); //PA6
    ra|=0<<24; //PA6
    ra&=~(0xF<<28); //PA7
    ra|=0<<28; //PA7
    PUT32(GPIOABASE+0x20,ra);


    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<6); //PA3
    ra|=1<<6; //PA3
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<3); //PA3
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<6; //PA3
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<6); //PA3
    PUT32(GPIOABASE+0x0C,ra);


    ra=GET32(RCCBASE+0x18);
    ra|=1<<12; //enable spi1
    PUT32(RCCBASE+0x18,ra);

    //PUT32(RCCBASE+0x04,(0xF<<4)|(7<<8)); //SLOOOWWWW

    PUT16(SPI1BASE+0x00,(0<<6)|(1<<3)|(1<<2)|0);
    PUT16(SPI1BASE+0x04,0x070C);
    PUT16(SPI1BASE+0x00,(1<<6)|(1<<3)|(1<<2)|0);
    PUT8(SPI1BASE+0x0C,0xf0);

    spi_command(0x21); //extended commands
//    spi_command(0xB0); //vop less contrast
    spi_command(0xBF); //vop more contrast
    spi_command(0x04); //temp coef
    spi_command(0x14); //bias mode 1:48
    spi_command(0x20); //extended off
    spi_command(0x0C); //display on

if(0)
{
    spi_command(0x80); //column
    spi_command(0x40); //row
    for(ra=0;ra<504;ra++) spi_data(ra);
}
if(0)
{
    spi_command(0x80); //column
    spi_command(0x40); //row
    for(ra=0;ra<504;ra++)
    {
        if(ra&1) spi_data(0xAA);
        else     spi_data(0x55);
    }
}
if(0)
{
    spi_command(0x80); //column
    spi_command(0x40); //row
    for(ra=0;ra<504;ra++)
    {
        if(ra&2) spi_data(0xCC);
        else     spi_data(0x33);
    }
}
if(1)
{
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
    unsigned int re;
    unsigned int rx;
    unsigned int ry;
    unsigned int rz;

    rd=0;
    for(rx=0;;rx++)
    {
        if(rx==100) rx=0;

        spi_command(0x80); //column
        spi_command(0x40); //row

        rd=base_ten(rx);
        for(ra=0;ra<5;ra++)
        {
            for(rb=0;rb<10;rb++) spi_data(0);
            re=(rd>>4)&0xF;
            //rc=font_data[((((rd>>4)&0xF)<<2)+rx)+ra];
            rc=font_data[((re<<2)+re)+ra];
            for(rb=0x8;rb;rb>>=1)
            {
                if(rb&rc) ry=0xFF;
                else      ry=0x00;
                for(rz=0;rz<8;rz++) spi_data(ry);
            }
            re=(rd>>0)&0xF;
            //rc=font_data[((((rd>>0)&0xF)<<2)+rx)+ra];
            rc=font_data[((re<<2)+re)+ra];
            for(rb=0x8;rb;rb>>=1)
            {
                if(rb&rc) ry=0xFF;
                else      ry=0x00;
                for(rz=0;rz<8;rz++) spi_data(ry);
            }
            for(rb=0;rb<10;rb++) spi_data(0);
        }
        for(rb=0;rb<84;rb++) spi_data(0);
        for(ra=0;ra<0x20000;ra++) dummy(ra);
    }
}






    return(0);
}
