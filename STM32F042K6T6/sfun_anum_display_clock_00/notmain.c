

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define GPIOA_MODER (GPIOABASE+0x00)

#define RCCBASE     0x40021000

#define USART1BASE  0x40013800


static unsigned int aa,bb,cc,dd;

static unsigned char num[8];




#define PADDR 0xE0


#define SCL 0
#define SDA 1

static void i2c_delay ( void )
{
    ////unsigned int ra;
    ////for(ra=0;ra<2;ra++) dummy(ra);
    ////if(uart2_check()) uart2_recv();
	//dummy(0);
}

static void scl_high ( void )
{
    PUT32(GPIOABASE+0x18,(1<<(SCL+ 0)));
}

static void sda_high ( void )
{
    PUT32(GPIOABASE+0x18,(1<<(SDA+ 0)));
}

static void scl_low ( void )
{
    PUT32(GPIOABASE+0x18,(1<<(SCL+16)));
}

static void sda_low ( void )
{
    PUT32(GPIOABASE+0x18,(1<<(SDA+16)));
}

//static unsigned int sda_read ( void )
//{
    //unsigned int ra;

    //ra=GET32(GPIOABASE+0x10);
    //ra&=(1<<SDA);
    //return(ra);
//}

//static void sda_input ( void )
//{
    //unsigned int ra;
    ////moder
    //ra=GET32(GPIOA_MODER);
    //ra&=~(3<<(SDA<<1)); //PC1
    ////ra|= (1<<(SDA<<1)); //PC1
    //PUT32(GPIOA_MODER,ra);
//}

//static void sda_output ( void )
//{
    //unsigned int ra;
    ////moder
    //ra=GET32(GPIOA_MODER);
    //ra&=~(3<<(SDA<<1)); //PC1
    //ra|= (1<<(SDA<<1)); //PC1
    //PUT32(GPIOA_MODER,ra);
//}

static void i2c_pin_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(SDA<<1));
    ra&=~(3<<(SCL<<1));
    ra|=1<<(SDA<<1);
    ra|=1<<(SCL<<1);
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<SDA);
    ra&=~(1<<SCL);
    PUT32(GPIOABASE+0x04,ra);
    PUT32(GPIOABASE+0x18,(1<<SDA)|(1<<SCL));

    scl_high();
    sda_high();
}

static void i2c_start ( void )
{
    i2c_delay();
    i2c_delay();
    sda_low();
    i2c_delay();
    i2c_delay();
    scl_low();
    i2c_delay();
}

static void i2c_stop ( void )
{
    i2c_delay();
    scl_high();
    i2c_delay();
    sda_high();
    i2c_delay();
    i2c_delay();
}

static unsigned int i2c_write_byte ( unsigned int b )
{
    unsigned int ra;
    for(ra=0x80;ra;ra>>=1)
    {
        i2c_delay();
        if(ra&b) sda_high();
        else     sda_low();
        i2c_delay();
        scl_high();
        i2c_delay();
        scl_low();
        i2c_delay();
        sda_low();
        i2c_delay();
    }
    i2c_delay();
    //sda_input();
    //i2c_delay();
    scl_high();
    i2c_delay();
    //ra=sda_read();
    //i2c_delay();
    scl_low();
    i2c_delay();
    //sda_output();
    //i2c_delay();
    return(ra);
}




//static unsigned int i2c_read_byte ( unsigned int *b )
//{
    //unsigned int ra;
    //unsigned int rb;
    //i2c_delay();
    //sda_input();
    //rb=0;
    //for(ra=0;ra<9;ra++)
    //{
        //i2c_delay();
        //scl_high();
        //i2c_delay();
        //rb<<=1;
        //if(sda_read()) rb|=1;
        //i2c_delay();
        //scl_low();
        //i2c_delay();
    //}
    //sda_output();
    //i2c_delay();
    //ra=rb&1;
    //*b=rb>>1;
    //return(ra);
//}

static void send_command ( unsigned int cmd, unsigned int data )
{
    i2c_start();
    if(i2c_write_byte(PADDR))
    {
        i2c_stop();
        //hexstring(0xBADBAD00);
        return;
    }
    i2c_write_byte(cmd);
    i2c_write_byte(data);
    i2c_stop();
}

static void send_data ( unsigned char *data, unsigned int len )
{
	unsigned int ra;

    i2c_start();
    if(i2c_write_byte(PADDR))
    {
        i2c_stop();
        //hexstring(0xBADBAD00);
        return;
    }
	for(ra=0;ra<len;ra++)
	{
		i2c_write_byte(data[ra]);
	}
    i2c_stop();
}

//static int i2c_detect ( unsigned int addr )
//{
    //i2c_start();
    //if(i2c_write_byte(addr))
    //{
        //i2c_stop();
        //return(0);
    //}
	//return(1);
    //i2c_stop();
//}

//  aaaa
// f    b
// f    b
//  gghh
// e    c
// e    c
//  dddd

// 0  a  00000000
// 1  0  hhhhaaaa
// 2  1  .......C colon
// 3  2  ....bbbb
// 4  3  .......D decimal
// 5  4  ....cccc
// 6  5  ........
// 7  6  ....dddd
// 8  7  ........
// 9  8  ....eeee
// 0  9  ........
// 1 10  ....ffff
// 2 11  ........
// 3 12  ....gggg

const unsigned char numbits[16]=
{
	0x3F, //0:  ..fe dcba
	0x06, //1:  .... .cb.
	0x5B, //2:  .g.e d.ba
	0x4F, //3:  .g.. dcba
	0x66, //4:  .gf. .cb.
	0x6D, //5:  .gf. dc.a
	0x7D, //6:  .gfe dc.a
	0x07, //7:  .... .cba
	0x7F, //8:  .gfe dcba
	0x6F, //9:  .gf. dcba
	0x00, //A:
	0x00, //B:
	0x00, //C:
	0x00, //D:
	0x00, //E:
	0x00, //F:
};
static unsigned int colon;
static unsigned int dot;

static void show_number ( void )
{
	unsigned char data[16];
	unsigned int ra;
	unsigned int rb;


	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;

	colon=(colon+1)&1;

	a=num[0];
	b=num[1];
	c=num[2];
	d=num[3];

	for(ra=0;ra<14;ra++) data[ra]=0;

	for(rb=13,ra=0x40;ra;ra>>=1,rb-=2)
	{
		if(numbits[a]&ra) data[rb]|=1;
		if(numbits[b]&ra) data[rb]|=2;
		if(numbits[c]&ra) data[rb]|=4;
		if(numbits[d]&ra) data[rb]|=8;
	}
	data[1]|=data[13]<<4;
	data[2]=colon;
	data[4]=dot;
	send_data(data,14);
}



void ten_div ( unsigned int ra )
{
    unsigned int num;
    unsigned int res;
    unsigned int acc;
    unsigned int den;
    unsigned int rb;


    num=ra;
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
    aa=res;

    num=acc;
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
    bb=res;

    num=acc;
    res=0;
    acc=0;
    den=10;
    for(rb=0x40;rb;rb>>=1)
    {
        acc<<=1;
        if(rb&num) acc|=1;
        if(acc>=den)
        {
            acc-=den;
            res|=rb;
        }
    }
    cc=res;
    dd=acc;
}




static int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);

    ra=GET32(RCCBASE+0x18);
    ra|=1<<14; //enable USART1
    PUT32(RCCBASE+0x18,ra);

    //PA9  UART1_TX
    //PA10 UART1_RX

    //moder 10
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<18); //PA9
    ra|=  2<<18; //PA9
    ra&=~(3<<20); //PA10
    ra|=  2<<20; //PA10
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

    //PUT32(USART1BASE+0x0C,1667); //4800
    PUT32(USART1BASE+0x0C,833); //9600
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|(1<<2)|1);

    return(0);
}

static unsigned int uart_recv ( void )
{
    while(1) if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    return(GET32(USART1BASE+0x24));
}

//$GPRMC,000143.00,A,4030.97866,N,07955.13947,W,0.419,,020416,,,A*6E
static unsigned int timezone;
static int do_nmea ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int state;
    unsigned int off;
    unsigned int sum;
    unsigned int savesum;
    unsigned int xsum;
    unsigned int validity;
    //unsigned int num[4];
    unsigned char xstring[32];


    state=0;
    off=0;
//$GPRMC,054033.00,V,
    sum=0;
    savesum=0;
    xsum=0;
    validity=0;
    while(1)
    {
        ra=uart_recv();
        //uart_send(ra);
        //uart_send(0x30+state);
        PUT32(USART1BASE+0x28,ra);

        if(ra!='*') sum^=ra;

        switch(state)
        {
            case 0:
            {
                if(ra=='$') state++;
                else state=0;
                sum=0;
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
                        //xstring[0] //tens of hours
                        //xstring[1] //ones of hours
                        //xstring[2] //tens of minutes
                        //xstring[3] //ones of minutes
                        //xstring[4] //tens of seconds
                        //xstring[5] //ones of seconds

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
                        //hour  =((xstring[0]&0xF)*10)+(xstring[1]&0xF);
                        //minute=((xstring[2]&0xF)*10)+(xstring[3]&0xF);
                        //second=((xstring[4]&0xF)*10)+(xstring[5]&0xF);
                        //hexstrings(hour);
                        //hexstrings(minute);
                        //hexstring(second);
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
                if(ra=='A') validity=1;
                else validity=0;
                state++;
            }
            case 9:
            {
                if(ra=='*')
                {
                    savesum=sum;
                    state++;
                }
                break;
            }
            case 10:
            {
                if(ra>0x39) ra-=0x37;
                xsum=(ra&0xF)<<4;
                state++;
                break;
            }
            case 11:
            {
                if(ra>0x39) ra-=0x37;
                xsum|=(ra&0xF);
                if(savesum==xsum)
                {
                    num[0]=xstring[0]&0xF;
                    num[1]=xstring[1]&0xF;
                    num[2]=xstring[2]&0xF;
                    num[3]=xstring[3]&0xF;
                    num[4]=xstring[4]&0xF;
                    num[5]=xstring[5]&0xF;

                    //just have the colon show validity state
                    if(validity)
                    {
						dot=0;
                        //num[1]|=0x80;
                    }
					else
					{
						dot=1;
					}
                    show_number();

                }
                for(rb=0;rb<6;rb++) xstring[rb]=0;
                state=0;
                break;
            }
        }
    }
    return(0);
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    timezone=4;
    rb=0x44444444;
    ra=GET32(0x20000D00);
    if(ra==0x44444444)
    {
        timezone=5;
        rb=0x55555555;
    }
    else
    if(ra==0x55555555)
    {
        timezone=4;
        rb=0x44444444;
    }
    PUT32(0x20000D00,rb);







    num[0]=0;
    num[1]=0;
    num[2]=0;
    num[3]=0;
    num[4]=0;
    num[5]=0;



    i2c_pin_init();

	send_command(0x21,0x00); //enable oscillator
	send_command(0xE0,0x00); //full brightness (reset value)
	send_command(0x80,0x00); //blinking off (reset value)
	send_command(0x81,0x00); //blinking off, display on

    num[0]=1;
    num[1]=2;
    num[2]=3;
    num[3]=4;
    num[4]=5;
    num[5]=6;
	colon=0;
	dot=0;

    show_number();


    uart_init();
    do_nmea();

    return(0);
}

/*

dfu-util -a 0 -s 0x08000000 -D notmain.bin

*/


