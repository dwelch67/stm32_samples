
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define GPIOCBASE 0x48000800
#define RCCBASE   0x40021000
#define FLASHBASE 0x40022000
#define FLASH_ACR (FLASHBASE+0x00)

static void led_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port A
    PUT32(RCCBASE+0x14,ra);

    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<10); //PA5
    ra|=1<<10; //PA5
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<5); //PA5
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<10; //PA5
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<10); //PA5
    PUT32(GPIOABASE+0x0C,ra);
}

static void led_on ( void )
{
    PUT32(GPIOABASE+0x18,((1<<5)<<0));
}

static void led_off ( void )
{
    PUT32(GPIOABASE+0x18,((1<<5)<<16));
}

static void button_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<19; //enable port C
    PUT32(RCCBASE+0x14,ra);

    //moder 2b00
    ra=GET32(GPIOCBASE+0x00);
    ra&=~(3<<26); //PC13
    PUT32(GPIOCBASE+0x00,ra);

    //pupdr 2b00
    ra=GET32(GPIOCBASE+0x0C);
    ra&=~(3<<26); //PC13
    PUT32(GPIOCBASE+0x0C,ra);
}

static unsigned int read_button ( void )
{
    return(GET32(GPIOCBASE+0x10)&(1<<13));
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    led_init();
    button_init();

    //this is a safety measure, if the code below that messes with
    //the clock source, bricks the microcontroller then press and
    //hold the user button, and press reset, on that reset this code
    //will see the button pressed and go into this infinite loop
    //instead of into the code below, allowing us to reprogram the
    //microcontrollers flash with hopefully a not buggy program.
    if(read_button()==0)
    {
        //make the led glow
        while(1)
        {
            led_on();
            led_off();
        }
    }
    //as delivered this code does not brick the microcontroller
    //the above was for me to develop this code and for you to change
    //this code as desired.


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
        led_on();
        for(ra=0;ra<200000;ra++) dummy(ra);
        led_off();
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
    //We cannot overclock the flash, so slow down the flash first
    //then speed up the clock
    //PUT32(FLASH_ACR,0x01); //>24Mhz no prefetch
    PUT32(FLASH_ACR,0x11); // >24MHZ with prefetch.
    //switch system to use pll clock
    ra=GET32(RCCBASE+0x04);
    ra&=~3;
    ra|=2;
    PUT32(RCCBASE+0x04,ra);
    while(1) if((GET32(RCCBASE+0x04)&3)==2) break;
    //now running at 48MHz.

    //blink a few times using this clock
    for(rx=0;rx<4;rx++)
    {
        led_on();
        for(ra=0;ra<200000;ra++) dummy(ra);
        led_off();
        for(ra=0;ra<200000;ra++) dummy(ra);
    }

    //press the reset button to see this happen again

    return(0);
}

