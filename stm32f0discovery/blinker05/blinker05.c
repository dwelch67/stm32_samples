
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOCBASE 0x48000800
#define RCCBASE 0x40021000

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;


    ra=GET32(RCCBASE+0x14);
    ra|=1<<19; //enable port C
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOCBASE+0x00);
    ra&=~(3<<16); //PC8
    ra&=~(3<<18); //PC9
    ra|=1<<16; //PC8
    ra|=1<<18; //PC9
    PUT32(GPIOCBASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOCBASE+0x04);
    ra&=~(1<<8); //PC8
    ra&=~(1<<9); //PC9
    PUT32(GPIOCBASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOCBASE+0x08);
    ra|=3<<16; //PC8
    ra|=3<<18; //PC8
    PUT32(GPIOCBASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOCBASE+0x0C);
    ra&=~(3<<16); //PC8
    ra&=~(3<<18); //PC9
    PUT32(GPIOCBASE+0x0C,ra);

    //make sure hse is up, external 8mhz clock
    ra=GET32(RCCBASE+0x00);
    ra|=1<<16;
    PUT32(RCCBASE+0x00,ra);
    while(1) if(GET32(RCCBASE+0x00)&(1<<17)) break;
    //use the hse clock for now
    ra=GET32(RCCBASE+0x04);
    ra&=~3;
    ra|=1;
    PUT32(RCCBASE+0x04,ra);
    while(1) if((GET32(RCCBASE+0x04)&3)==1) break;
//blink a few times using this clock
for(rx=0;rx<4;rx++)
{
    PUT32(GPIOCBASE+0x18,0x01000200);
    for(ra=0;ra<200000;ra++) dummy(ra);
    PUT32(GPIOCBASE+0x18,0x02000100);
    for(ra=0;ra<200000;ra++) dummy(ra);
}
    //in order to change any pll settings must disable pll first
    ra=GET32(RCCBASE+0x00);
    ra&=~(1<<24);
    PUT32(RCCBASE+0x00,ra);
    while(1) if((GET32(RCCBASE+0x00)&(1<<25))==0) break;
    //no hse pre-division
    PUT32(RCCBASE+0x2C,0);
    //set other pll settings 6*8=48mhz, max speed for this part
    ra=GET32(RCCBASE+0x04);
    ra&=~(0x3F<<16);
    ra|=0x4<<18; //multiply by 6
    ra|=1<<16; //hse source
    PUT32(RCCBASE+0x04,ra);
    //turn pll back on with new settings and wait for it to be ready
    ra=GET32(RCCBASE+0x00);
    ra|=(1<<24);
    PUT32(RCCBASE+0x00,ra);
    while(1) if((GET32(RCCBASE+0x00)&(1<<25))!=0) break;
    //switch system to use pll clock
    ra=GET32(RCCBASE+0x04);
    ra&=~3;
    ra|=2;
    PUT32(RCCBASE+0x04,ra);
    while(1) if((GET32(RCCBASE+0x04)&3)==2) break;
    //now running at 48MHz.


    for(rx=0;;rx++)
    {
        PUT32(GPIOCBASE+0x18,0x01000200);
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOCBASE+0x18,0x02000100);
        for(ra=0;ra<200000;ra++) dummy(ra);
    }

    return(0);
}

