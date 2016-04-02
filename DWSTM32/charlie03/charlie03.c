
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

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

#define THREELEDS 10

static const unsigned short figure_eight[16]=
{
//000.
//....
//....
0xE00,
//.000
//....
//....
0x700,
//..00
//...0
//....
0x310,
//...0
//..00
//....
0x130,
//....
//.000
//....
0x070,
//....
//000.
//....
0x0E0,
//....
//00..
//0...
0x0C8,
//....
//0...
//00..
0x08C,
//....
//....
//000.
0x00E,
//....
//....
//.000
0x007,
//....
//...0
//..00
0x013,
//....
//..00
//...0
0x031,
//....
//.000
//....
0x070,
//....
//000.
//....
0x0E0,
//0...
//00..
//....
0x8C0,
//00..
//0...
//....
0xC80
};

static const unsigned short round_about[10]=
{
//000.
//....
//....
0xE00,
//.000
//....
//....
0x700,
//..00
//...0
//....
0x310,
//...0
//...0
//...0
0x111,
//....
//...0
//..00
0x013,
//....
//....
//.000
0x007,
//....
//....
//000.
0x00E,
//....
//0...
//00..
0x08C,
//0...
//0...
//0...
0x888,
//00..
//0...
//....
0xC80,
};


int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
    unsigned int rx;
    unsigned int ry;
    unsigned int rz;
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
        for(ra=0;ra<10;ra++)
        {
            for(rx=0;rx<50;rx++)
            {
                quicky(moder,ledout[0][0],ledmoder[0][0]);
                quicky(moder,ledout[0][1],ledmoder[0][1]);
                quicky(moder,ledout[0][2],ledmoder[0][2]);
                quicky(moder,ledout[1][0],ledmoder[1][0]);
                quicky(moder,ledout[1][1],ledmoder[1][1]);
                quicky(moder,ledout[1][2],ledmoder[1][2]);
            }
            for(rx=0;rx<50;rx++)
            {
                quicky(moder,ledout[2][0],ledmoder[2][0]);
                quicky(moder,ledout[2][1],ledmoder[2][1]);
                quicky(moder,ledout[2][2],ledmoder[2][2]);
                quicky(moder,ledout[3][0],ledmoder[3][0]);
                quicky(moder,ledout[3][1],ledmoder[3][1]);
                quicky(moder,ledout[3][2],ledmoder[3][2]);
            }
        }
        for(ra=0;ra<10;ra++)
        {
            for(rx=0;rx<50;rx++)
            {
                quicky(moder,ledout[0][0],ledmoder[0][0]);
                quicky(moder,ledout[0][1],ledmoder[0][1]);
                quicky(moder,ledout[0][2],ledmoder[0][2]);
                quicky(moder,ledout[3][0],ledmoder[3][0]);
                quicky(moder,ledout[3][1],ledmoder[3][1]);
                quicky(moder,ledout[3][2],ledmoder[3][2]);
            }
            for(rx=0;rx<50;rx++)
            {
                quicky(moder,ledout[1][0],ledmoder[1][0]);
                quicky(moder,ledout[1][1],ledmoder[1][1]);
                quicky(moder,ledout[1][2],ledmoder[1][2]);
                quicky(moder,ledout[2][0],ledmoder[2][0]);
                quicky(moder,ledout[2][1],ledmoder[2][1]);
                quicky(moder,ledout[2][2],ledmoder[2][2]);
            }
        }
        for(ra=0;ra<10;ra++)
        {
            for(rb=0;rb<16;rb++)
            {
                rc=figure_eight[rb];
                for(rz=0;rz<THREELEDS;rz++)
                {
                    ry=3;
                    rx=2;
                    for(rd=0x800;rd;rd>>=1)
                    {
                        if(rd&rc) quicky(moder,ledout[ry][rx],ledmoder[ry][rx]);
                        if((ry--)==0)
                        {
                            ry=3;
                            if((rx--)==0) rx=2;
                        }
                    }
                }
            }
        }
        for(ra=0;ra<10;ra++)
        {
            for(rb=0;rb<10;rb++)
            {
                rc=round_about[rb];
                for(rz=0;rz<THREELEDS;rz++)
                {
                    ry=3;
                    rx=2;
                    for(rd=0x800;rd;rd>>=1)
                    {
                        if(rd&rc) quicky(moder,ledout[ry][rx],ledmoder[ry][rx]);
                        if((ry--)==0)
                        {
                            ry=3;
                            if((rx--)==0) rx=2;
                        }
                    }
                }
            }
        }
    }
    return(0);
}
