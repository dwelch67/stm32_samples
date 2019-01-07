
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    PUT32(GPIOABASE+0x00,0x28015555);

    for(rx=0;;rx++)
    {
        PUT32(GPIOABASE+0x18,0x000F00F0);
        for(ra=0;ra<200000;ra++) dummy(ra);
        PUT32(GPIOABASE+0x18,0x00F0000F);
        for(ra=0;ra<200000;ra++) dummy(ra);
    }
    return(0);
}
/*


dfu-util -a 0 -s 0x08000000 -D blinker01.bin

*/
