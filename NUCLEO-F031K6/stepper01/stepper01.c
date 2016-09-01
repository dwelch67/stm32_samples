
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define GPIOBBASE 0x48000400
#define RCCBASE 0x40021000


static const char sequence[8]=
{
0x1, //0001
0x5, //0101
0x4, //0100
0x6, //0110
0x2, //0010
0xA, //1010
0x8, //1000
0x9, //1001
};


int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rx;
    unsigned int regs[8];

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);


    //moder 10
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<0); //PA0
    ra|=  1<<0;  //PA0
    ra&=~(3<<2); //PA1
    ra|=  1<<2;  //PA1
    ra&=~(3<<6); //PA3
    ra|=  1<<6;  //PA3
    ra&=~(3<<8); //PA4
    ra|=  1<<8;  //PA4
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER 0
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<0); //PA0
    ra&=~(1<<1); //PA1
    ra&=~(1<<3); //PA3
    ra&=~(1<<4); //PA4
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr 11
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<0; //PA0
    ra|=3<<2; //PA1
    ra|=3<<6; //PA3
    ra|=3<<8; //PA4 
    PUT32(GPIOABASE+0x08,ra);
    //pupdr 00
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<0); //PA0
    ra&=~(3<<2); //PA1
    ra&=~(3<<6); //PA3
    ra&=~(3<<8); //PA4
    PUT32(GPIOABASE+0x0C,ra);

    for(rx=0;rx<8;rx++)
    {
        rb=0;
        if(sequence[rx]&1) rb|=1<<(0); else rb|=1<<(0+16);
        if(sequence[rx]&2) rb|=1<<(1); else rb|=1<<(1+16);
        if(sequence[rx]&4) rb|=1<<(3); else rb|=1<<(3+16);
        if(sequence[rx]&8) rb|=1<<(4); else rb|=1<<(4+16);
        regs[rx]=rb;
    }

    for(rx=0;;rx++)
    {
        if(rx&0x100) PUT32(GPIOABASE+0x18,regs[rx&7]);
        else         PUT32(GPIOABASE+0x18,regs[7-(rx&7)]);
        for(ra=0;ra<0x1000;ra++) dummy(ra);
    }
    return(0);
}
