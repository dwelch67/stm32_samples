
void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000
#define USART1BASE 0x40013800


#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

static unsigned char xstring[32];
static unsigned int tim[4];
static unsigned int lasttim[4];
#define ZMASK 0xFF
static unsigned char zbuff[ZMASK+1];
static unsigned int zhead;
static unsigned int ztail;




#define LED00OUT  (0x1)
#define LED01OUT  (0x1)
#define LED02OUT  (0x1)
#define LED00MODER ((1<<(0<<1))|(1<<(1<<1)))
#define LED01MODER ((1<<(0<<1))|(1<<(3<<1)))
#define LED02MODER ((1<<(0<<1))|(1<<(4<<1)))

#define LED10OUT  (0x2)
#define LED11OUT  (0x2)
#define LED12OUT  (0x2)
#define LED10MODER ((1<<(1<<1))|(1<<(0<<1)))
#define LED11MODER ((1<<(1<<1))|(1<<(3<<1)))
#define LED12MODER ((1<<(1<<1))|(1<<(4<<1)))

#define LED20OUT  (0x8)
#define LED21OUT  (0x8)
#define LED22OUT  (0x8)
#define LED20MODER ((1<<(3<<1))|(1<<(0<<1)))
#define LED21MODER ((1<<(3<<1))|(1<<(1<<1)))
#define LED22MODER ((1<<(3<<1))|(1<<(4<<1)))

#define LED30OUT  (0x10)
#define LED31OUT  (0x10)
#define LED32OUT  (0x10)
#define LED30MODER ((1<<(4<<1))|(1<<(0<<1)))
#define LED31MODER ((1<<(4<<1))|(1<<(1<<1)))
#define LED32MODER ((1<<(4<<1))|(1<<(3<<1)))


//[y][x]
static const unsigned int ledout[4][4]=
{
    {LED00OUT,LED01OUT,LED02OUT,0},
    {LED10OUT,LED11OUT,LED12OUT,0},
    {LED20OUT,LED21OUT,LED22OUT,0},
    {LED30OUT,LED31OUT,LED32OUT,0},
};
static const unsigned int ledmoder[4][4]=
{
    {LED00MODER,LED01MODER,LED02MODER,0},
    {LED10MODER,LED11MODER,LED12MODER,0},
    {LED20MODER,LED21MODER,LED22MODER,0},
    {LED30MODER,LED31MODER,LED32MODER,0},
};

static void uart_check ( void )
{
    if((GET32(USART1BASE+0x1C))&(1<<5))
    {
        zhead=(zhead+1)&ZMASK;
        zbuff[zhead]=GET32(USART1BASE+0x24)&0xFF;
    }
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//static void quicky ( unsigned int moder, unsigned int o, unsigned int m )
//{
    //unsigned int ra;
    //PUT32(GPIOABASE+0x14,o);
    //PUT32(GPIOABASE+0x00,moder|m);
    //while(1)
    //{
        //uart_check();
        //ra=GET32(STK_CSR);
        //if(ra&(1<<16)) break;
    //}
    //PUT32(GPIOABASE+0x00,moder);
    //if(1) //0 makes it brighter
    //{
        //while(1)
        //{
            //uart_check();
            //ra=GET32(STK_CSR);
            //if(ra&(1<<16)) break;
        //}
    //}
//}


static int delay ( unsigned int n )
{
    unsigned int ra;

    while(n--)
    {
        while(1)
        {
            uart_check();
            ra=GET32(STK_CSR);
            if(ra&(1<<16)) break;
        }
    }
    return(0);
}

//------------------------------------------------------------------------
static void show_time ( void )
{
    unsigned int rb;
    unsigned int rc;
    unsigned int rx;
    unsigned int ry;
    unsigned int moder;

    moder=GET32(GPIOABASE+0x00);

    for(rx=0;rx<3;rx++)
    {
        switch(rx)
        {
            case 0:
            {
                rb=lasttim[1];
                if(lasttim[0]) rb+=10;
                break;
            }
            case 1:
            {
                rb=lasttim[2];
                break;
            }
            case 2:
            {
                rb=lasttim[3];
                break;
            }
        }
        for(rc=8,ry=0;ry<4;ry++,rc>>=1)
        {
            PUT32(GPIOABASE+0x14,ledout[ry][rx]);
            PUT32(GPIOABASE+0x00,moder|ledmoder[ry][rx]);
            delay(1);
            if(rc&rb) delay(10);
            PUT32(GPIOABASE+0x00,moder);
            delay(50);
        }
    }


    //rb=lasttim[1];
    //if(lasttim[0]) rb+=10;
    //for(rx=0;rx<4;rx++)
    //{
        //rc=digits0[rb][rx][0];
        //rd=digits0[rb][rx][1];
        //{
            //quicky(moder,rc,rd);
        //}
    //}
    //rb=lasttim[2];
    //for(rx=0;rx<4;rx++)
    //{
        //rc=digits1[rb][rx][0];
        //rd=digits1[rb][rx][1];
        //{
            //quicky(moder,rc,rd);
        //}
    //}
    //rb=lasttim[3];
    ////for(rz=0;rz<10;rz++)
    //for(rx=0;rx<4;rx++)
    //{
        //rc=digits2[rb][rx][0];
        //rd=digits2[rb][rx][1];
        //{
            //quicky(moder,rc,rd);
        //}
    //}
}
//------------------------------------------------------------------------
static int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<14; //enable USART1
    PUT32(RCCBASE+0x18,ra);

    //moder 10
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<18); //PA9
    ra|=2<<18; //PA9
    ra&=~(3<<20); //PA10
    ra|=2<<20; //PA10
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER 0
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<9); //PA9
    ra&=~(1<<10); //PA10
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr 11
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<18; //PA9
    ra|=3<<20; //PA10
    PUT32(GPIOABASE+0x08,ra);
    //pupdr 00
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<18); //PA9
    ra&=~(3<<20); //PA10
    PUT32(GPIOABASE+0x0C,ra);
    //afr 0001
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<4); //PA9
    ra|=0x1<<4; //PA9
    ra&=~(0xF<<8); //PA10
    ra|=0x1<<8; //PA10
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x0C);
    ra|=1<<14; //reset USART1
    PUT32(RCCBASE+0x0C,ra);
    ra&=~(1<<14);
    PUT32(RCCBASE+0x0C,ra);

    PUT32(USART1BASE+0x0C,833);
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|(1<<2)|1);

    return(0);
}
//------------------------------------------------------------------------
static void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
    PUT32(USART1BASE+0x28,x);
}
//------------------------------------------------------------------------
static unsigned int uart_recv ( void )
{
    while(1)
    {
        if(zhead!=ztail)
        {
            ztail=(ztail+1)&ZMASK;
            return(zbuff[ztail]);
        }
        show_time();
    }
    //while(1)
    //{
        //if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    //}
    //return(GET32(USART1BASE+0x24));
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------

static int charlie_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    //ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    ra&=~(3<<(4<<1)); //PA4
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<0); //PA0
    ra&=~(1<<1); //PA1
    //ra&=~(1<<2); //PA2
    ra&=~(1<<3); //PA3
    ra&=~(1<<4); //PA4
    PUT32(GPIOABASE+0x04,ra);
    //ospeedr
    ra=GET32(GPIOABASE+0x08);
    ra|=3<<(0<<1); //PA0
    ra|=3<<(1<<1); //PA1
    //ra|=3<<(2<<1); //PA2
    ra|=3<<(3<<1); //PA3
    ra|=3<<(4<<1); //PA4
    PUT32(GPIOABASE+0x08,ra);
    //pupdr
    ra=GET32(GPIOABASE+0x0C);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    //ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    ra&=~(3<<(4<<1)); //PA4
    PUT32(GPIOABASE+0x0C,ra);

    PUT32(STK_CSR,4);
    PUT32(STK_RVR,100-1);
    PUT32(STK_CVR,0x00000000);
    PUT32(STK_CSR,5);
    return(0);
}

//------------------------------------------------------------------------
static void show_clock ( void )
{
    unsigned int ra;

    ra=0;
    if(tim[0]!=lasttim[0]) ra++;
    if(tim[1]!=lasttim[1]) ra++;
    if(tim[2]!=lasttim[2]) ra++;
    if(tim[3]!=lasttim[3]) ra++;
    if(ra==0) return;

    lasttim[0]=tim[0];
    lasttim[1]=tim[1];
    lasttim[2]=tim[2];
    lasttim[3]=tim[3];

    uart_send(0x30+lasttim[0]);
    uart_send(0x30+lasttim[1]);
    uart_send(0x30+lasttim[2]);
    uart_send(0x30+lasttim[3]);
    uart_send(0x0D);
    uart_send(0x0A);

}
//$GPRMC,000143.00,A,4030.97866,N,07955.13947,W,0.419,,020416,,,A*6E

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
static unsigned int timezone;
//------------------------------------------------------------------------
static int do_nmea ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int state;
    unsigned int off;
    //unsigned char toggle_seconds;

    //toggle_seconds=0;
    state=0;
    off=0;
//$GPRMC,054033.00,V,
    while(1)
    {
        ra=uart_recv();
        //uart_send(ra);
        //uart_send(0x30+state);
        switch(state)
        {
            case 0:
            {
                if(ra=='$') state++;
                else state=0;
                break;
            }
            case 1:
            {
                if(ra=='G') state++;
                else state=0;
                break;
            }
            case 2:
            {
                if(ra=='P') state++;
                else state=0;
                break;
            }
            case 3:
            {
                if(ra=='R') state++;
                else state=0;
                break;
            }
            case 4:
            {
                if(ra=='M') state++;
                else state=0;
                break;
            }
            case 5:
            {
                if(ra=='C') state++;
                else state=0;
                break;
            }
            case 6:
            {
                off=0;
                if(ra==',') state++;
                else state=0;
                break;
            }
            case 7:
            {
                if(ra==',')
                {
                    if(off>7)
                    {
                        rb=xstring[0]&0xF;
                        rc=xstring[1]&0xF;
                        //1010
                        rb=/*rb*10*/(rb<<3)+(rb<<1); //times 10
                        rb+=rc;
                        if(rb>12) rb-=12;
                        //ra=5; //time zone adjustment winter
                        //ra=4; //time zone adjustment summer
                        ra=timezone;
                        if(rb<=ra) rb+=12;
                        rb-=ra;
                        if(rb>9)
                        {
                            xstring[0]='1';
                            rb-=10;
                        }
                        else
                        {
                            xstring[0]='0';
                        }
                        rb&=0xF;
                        xstring[1]=0x30+rb;
                        rb=0;
                        //zstring[rb++]=0x77;
                        //toggle_seconds^=0x10;
                        //zstring[rb++]=toggle_seconds;
                        //zstring[rb++]=xstring[0];
                        //zstring[rb++]=xstring[1];
                        //zstring[rb++]=xstring[2];
                        //zstring[rb++]=xstring[3];
                        //xstring[rb++]=0x0D;
                        //zstring[rb++]=0;
                        tim[0]=xstring[0]&0xF;
                        tim[1]=xstring[1]&0xF;
                        tim[2]=xstring[2]&0xF;
                        tim[3]=xstring[3]&0xF;
                    }
                    else
                    {
                        //zstring[0]=0x33;
                        //zstring[1]=0x33;
                        //zstring[2]=0x33;
                        //zstring[3]=0x33;
                        //xstring[4]=0x0D;
                        //zstring[5]=0;
                        tim[0]=0;
                        tim[1]=0;
                        tim[2]=0;
                        tim[3]=0;
                    }
                    off=0;
                    state++;
                }
                else
                {
                    if(off<16)
                    {
                        xstring[off++]=ra;
                    }
                }
                break;
            }
            case 8:
            {
                //if(zstring[off]==0)
                //{
                    //state=0;
                //}
                //else
                //{
                    //uart_send(zstring[off++]);
                //}
                //show_time();
                show_clock();
                state=0;
                break;
            }
        }
    }
    return(0);
}


int notmain ( void )
{
    uart_init();
    charlie_init();

    timezone=4;
    if(GET32(0x20000D00)==0x12341234) timezone=5;
    PUT32(0x20000D00,0x12341234);

    lasttim[0]=1;
    lasttim[1]=1;
    lasttim[2]=2;
    lasttim[3]=3;

    zhead=0;
    ztail=0;

    do_nmea();

    return(0);
}
