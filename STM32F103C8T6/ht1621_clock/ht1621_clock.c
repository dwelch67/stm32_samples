

//PA9  TX
//PA10 RX


void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
void ASMDELAY ( unsigned int );


#define USART1_BASE 0x40013800
#define USART1_SR   (USART1_BASE+0x00)
#define USART1_DR   (USART1_BASE+0x04)
#define USART1_BRR  (USART1_BASE+0x08)
#define USART1_CR1  (USART1_BASE+0x0C)
#define USART1_CR2  (USART1_BASE+0x10)
#define USART1_CR3  (USART1_BASE+0x14)
#define USART1_GTPR (USART1_BASE+0x18)

#define GPIOABASE 0x40010800
#define GPIOA_BASE 0x40010800
#define GPIOA_CRL  (GPIOA_BASE+0x00)
#define GPIOA_CRH  (GPIOA_BASE+0x04)
#define GPIOA_IDR  (GPIOA_BASE+0x08)
#define GPIOA_ODR  (GPIOA_BASE+0x0C)
#define GPIOA_BSRR (GPIOA_BASE+0x10)
#define GPIOA_BRR  (GPIOA_BASE+0x14)
#define GPIOA_LCKR (GPIOA_BASE+0x18)

#define RCC_BASE 0x40021000
#define RCC_APB2ENR (RCC_BASE+0x18)

#define GPIOCBASE 0x40011000
#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

static void uart_init ( void )
{
    //assuming 8MHz clock, 115200 8N1
    unsigned int ra;

    ra=GET32(RCC_APB2ENR);
    ra|=1<<2;   //GPIOA
    ra|=1<<14;  //USART1
    PUT32(RCC_APB2ENR,ra);

    //pa9 TX  alternate function output push-pull
    //pa10 RX configure as input floating
    ra=GET32(GPIOA_CRH);
    ra&=~(0xFF0);
    ra|=0x490;
    PUT32(GPIOA_CRH,ra);

    PUT32(USART1_CR1,0x2000);
    PUT32(USART1_CR2,0x0000);
    PUT32(USART1_CR3,0x0000);
    //8000000/16 = 500000
    //500000/115200 = 4.34
    //4 and 5/16 = 4.3125
    //4.3125 * 16 * 115200 = 7948800
    //4800
    //104.1667
    //104 2/16 7996800  3200
    //104 3/16 8001600  1600
    PUT32(USART1_BRR,0x0684);
    PUT32(USART1_CR1,0x200C);
}
static unsigned int uart_recv ( void )
{
    while(1)
    {
        if(GET32(USART1_SR)&0x20) break;
    }
    return(GET32(USART1_DR)&0xFF);
}
static void uart_send ( unsigned int c )
{
    while(1)
    {
        if(GET32(USART1_SR)&0x80) break;
    }
    PUT32(USART1_DR,c);
}
static void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}
static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}













static void ht_delay ( void )
{
    //ASMDELAY(10);
}

#define DATA    0
#define WR      1
#define CS      2

//  777
// 3   6
// 3   6
//  222
// 1   5
// 1   5
// 4000
static const unsigned char num_bits[16]=
{
0xEB,//0 765x3x10
0x60,//1 x65xxxxx
0xC7,//2 76xxx210
0xE5,//3 765xx2x0
0x6C,//4 x65x32xx
0xAD,//5 7x5x32x0
0xAF,//6 7x5x3210
0xE0,//7 765xxxxx
0xEF,//8 765x3210
0xED,//9 765x32x0
0x00,
0x00,
0x00,
0x00,
0x00,
0x00
};

static void ht_pin_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x18);
    ra|=1<<2; //enable port a
    PUT32(RCCBASE+0x18,ra);
    //config
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<((DATA<<2)+0));   //PA0
    ra|= (3<<((DATA<<2)+0));   //PA0
    ra&=~(3<<((DATA<<2)+2));   //PA0
    ra|= (0<<((DATA<<2)+2));   //PA0
    ra&=~(3<<((WR<<2)+0));   //PA1
    ra|= (3<<((WR<<2)+0));   //PA1
    ra&=~(3<<((WR<<2)+2));   //PA1
    ra|= (0<<((WR<<2)+2));   //PA1
    ra&=~(3<<((CS<<2)+0));   //PA2
    ra|= (3<<((CS<<2)+0));   //PA2
    ra&=~(3<<((CS<<2)+2));   //PA2
    ra|= (0<<((CS<<2)+2));   //PA2
    PUT32(GPIOABASE+0x00,ra);
    PUT32(GPIOABASE+0x10,(1<<DATA)|(1<<WR)|(1<<CS));
}
static void ht_data ( unsigned int x )
{
    if(x) PUT32(GPIOABASE+0x10,(1<<(DATA+ 0)));
    else  PUT32(GPIOABASE+0x10,(1<<(DATA+16)));
}
static void ht_wr ( unsigned int x )
{
    if(x) PUT32(GPIOABASE+0x10,(1<<(WR+ 0)));
    else  PUT32(GPIOABASE+0x10,(1<<(WR+16)));
}
static void ht_cs ( unsigned int x )
{
    if(x) PUT32(GPIOABASE+0x10,(1<<(CS+ 0)));
    else  PUT32(GPIOABASE+0x10,(1<<(CS+16)));
}
static void ht_strobe_wr ( void )
{
    ht_delay();
    ht_wr(0);
    ht_delay();
    ht_wr(1);
    ht_delay();
}
static void ht_command ( unsigned int x )
{
    unsigned int ra;
    x&=0xFF;
    x|=0x400;
    x<<=1;

    ht_delay();
    ht_cs(0);
    for(ra=0x800;ra;ra>>=1)
    {
        ht_data(ra&x);
        ht_strobe_wr();
    }
    ht_cs(1);
    ht_delay();
}
static void ht_write ( unsigned int a, unsigned int d )
{
    unsigned int ra;
    unsigned int x;
    x=0x5; //3
    x<<=6;
    x|=a&0x3F; //6
    x<<=8;
    x|=d&0xFF; //8
    x<<=1; //1
    ht_delay();
    ht_cs(0);
    //3+6+8+1 = 18
    for(ra=1<<(18-1);ra;ra>>=1)
    {
        ht_data(ra&x);
        ht_strobe_wr();
    }
    ht_cs(1);
    ht_delay();
}
static void ht_init ( void )
{
    unsigned int ra;
    unsigned int add;
    ht_pin_init();
    ht_command(0x00); //SYS DIS
    ASMDELAY(100000);
    ht_command(0x01); //SYS EN
    ht_command(0x18); //RC 256K default setting
    ht_command(0x29); //4 commons 1/3 bias
    ht_command(0x03); //LCD ON
    add=0;
    for(ra=0;ra<16;ra++)
    {
        ht_write(add,0x00);
        add+=2;
    }
}




static unsigned char xstring[32];
static unsigned int tim[4];
static unsigned int lasttim[4];
static void show_clock ( void )
{
    ht_write(0,num_bits[tim[3]]);
    ht_write(2,num_bits[tim[2]]);
    ht_write(4,num_bits[tim[1]]);
    if(tim[0]==0)
    {
        ht_write(6,0x00);
    }
    else
    {
        ht_write(6,num_bits[tim[0]]);
    }
    uart_send(0x30+tim[0]);
    uart_send(0x30+tim[1]);
    uart_send(0x30+tim[2]);
    uart_send(0x30+tim[3]);
    uart_send(0x0D);
    uart_send(0x0A);
}


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
//**/ uart_putc(ra);
//uart_putc(0x30+state);
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
                        ra=4; //time zone adjustment summer
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
    hexstring(0x12345678);

    ht_init();


    tim[0]=8;
    tim[1]=8;
    tim[2]=8;
    tim[3]=8;

    lasttim[0]=0;
    lasttim[1]=0;
    lasttim[2]=0;
    lasttim[3]=0;

    show_clock();

    do_nmea();

    return(0);
}
/*


halt
flash write_image erase ht1621_clock/ht1621_clock.thumb.elf



*/
