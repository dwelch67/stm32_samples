
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF


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

#define LED00OUT  (0x1)
#define LED01OUT  (0x1)
#define LED02OUT  (0x1)
#define LED00MODER ((1<<(0<<1))|(1<<(1<<1)))
#define LED01MODER ((1<<(0<<1))|(1<<(2<<1)))
#define LED02MODER ((1<<(0<<1))|(1<<(3<<1)))

#define LED10OUT  (0x2)
#define LED11OUT  (0x2)
#define LED12OUT  (0x2)
#define LED10MODER ((1<<(1<<1))|(1<<(0<<1)))
#define LED11MODER ((1<<(1<<1))|(1<<(2<<1)))
#define LED12MODER ((1<<(1<<1))|(1<<(3<<1)))

#define LED20OUT  (0x4)
#define LED21OUT  (0x4)
#define LED22OUT  (0x4)
#define LED20MODER ((1<<(2<<1))|(1<<(0<<1)))
#define LED21MODER ((1<<(2<<1))|(1<<(1<<1)))
#define LED22MODER ((1<<(2<<1))|(1<<(3<<1)))

#define LED30OUT  (0x8)
#define LED31OUT  (0x8)
#define LED32OUT  (0x8)
#define LED30MODER ((1<<(3<<1))|(1<<(0<<1)))
#define LED31MODER ((1<<(3<<1))|(1<<(1<<1)))
#define LED32MODER ((1<<(3<<1))|(1<<(2<<1)))

//[y][x]
static const unsigned char ledout[4][4]=
{
    {LED00OUT,LED01OUT,LED02OUT,0},
    {LED10OUT,LED11OUT,LED12OUT,0},
    {LED20OUT,LED21OUT,LED22OUT,0},
    {LED30OUT,LED31OUT,LED32OUT,0},
};
static const unsigned char ledmoder[4][4]=
{
    {LED00MODER,LED01MODER,LED02MODER,0},
    {LED10MODER,LED11MODER,LED12MODER,0},
    {LED20MODER,LED21MODER,LED22MODER,0},
    {LED30MODER,LED31MODER,LED32MODER,0},
};

static void quicky ( unsigned int moder, unsigned char o, unsigned char m )
{
    unsigned int ra;
    PUT32(GPIOABASE+0x14,o);
    PUT32(GPIOABASE+0x00,moder|m);
    while(1)
    {
        ra=GET32(STK_CSR);
        if(ra&(1<<16)) break;
    }
    PUT32(GPIOABASE+0x00,moder);
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;
    unsigned int ry;
    unsigned int moder;


    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    //ra|=1<<(0<<1); //PA0
    //ra|=1<<(1<<1); //PA1
    //ra|=1<<(2<<1); //PA2
    //ra|=1<<(3<<1); //PA3
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<0); //PA0
    ra&=~(1<<1); //PA1
    ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<(0<<1); //PA0
    ra|=3<<(1<<1); //PA1
    ra|=3<<(2<<1); //PA2
    ra|=3<<(3<<1); //PA3
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    PUT32(GPIOABASE+0x0C,ra);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,10000-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);
    moder=GET32(GPIOABASE+0x00);

    while(1)
    {
        for(ry=0;ry<4;ry++)
        {
            for(rx=0;rx<100;rx++)
            {
                quicky(moder,ledout[ry][0],ledmoder[ry][0]);
                quicky(moder,ledout[ry][1],ledmoder[ry][1]);
                quicky(moder,ledout[ry][2],ledmoder[ry][2]);
            }
        }
        for(rx=0;rx<3;rx++)
        {
            for(ry=0;ry<75;ry++)
            {
                quicky(moder,ledout[0][rx],ledmoder[0][rx]);
                quicky(moder,ledout[1][rx],ledmoder[1][rx]);
                quicky(moder,ledout[2][rx],ledmoder[2][rx]);
                quicky(moder,ledout[3][rx],ledmoder[3][rx]);
            }
        }
    }
    return(0);
}
