
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
void ASMDELAY ( unsigned int );

#define GPIOABASE 0x48000000
#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF

#define TIM2_BASE 0x40000000
#define TIM2_CR1 (TIM2_BASE+0x00)
#define TIM2_CR2 (TIM2_BASE+0x04)
#define TIM2_SR  (TIM2_BASE+0x10)
#define TIM2_CNT (TIM2_BASE+0x24)
#define TIM2_PSC (TIM2_BASE+0x28)
#define TIM2_ARR (TIM2_BASE+0x2C)
#define TIM2_MASK 0xFFFFFFFF

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

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(DATA<<1));
    ra&=~(3<<(WR<<1));
    ra&=~(3<<(CS<<1));
    ra|=1<<(DATA<<1);
    ra|=1<<(WR<<1);
    ra|=1<<(CS<<1);
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<DATA);
    ra&=~(1<<WR);
    ra&=~(1<<CS);
    PUT32(GPIOABASE+0x04,ra);
    PUT32(GPIOABASE+0x18,(1<<DATA)|(1<<WR)|(1<<CS));
}
static void ht_data ( unsigned int x )
{
    if(x) PUT32(GPIOABASE+0x18,(1<<(DATA+ 0)));
    else  PUT32(GPIOABASE+0x18,(1<<(DATA+16)));
}
static void ht_wr ( unsigned int x )
{
    if(x) PUT32(GPIOABASE+0x18,(1<<(WR+ 0)));
    else  PUT32(GPIOABASE+0x18,(1<<(WR+16)));
}
static void ht_cs ( unsigned int x )
{
    if(x) PUT32(GPIOABASE+0x18,(1<<(CS+ 0)));
    else  PUT32(GPIOABASE+0x18,(1<<(CS+16)));
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


static unsigned int mt,mo;
static unsigned int st,so;

static void long_div ( unsigned int ra )
{
    unsigned int num;
    unsigned int res;
    unsigned int acc;
    unsigned int den;
    unsigned int rb;
    unsigned int mm;
    unsigned int ss;

    //8000000/50000 = 160 ticks per second
    //divide by 160
    num=ra;
    res=0;
    acc=0;
    den=160;
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
    num=res;
    res=0;
    acc=0;
    den=60;
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
    mm=res;
    ss=acc;

    num=mm;
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
    mt=res;
    mo=acc;

    num=ss;
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
    st=res;
    so=acc;
}


//static
unsigned int short_div ( unsigned int ra )
{
    unsigned int num;
    unsigned int res;
    unsigned int acc;
    unsigned int den;
    unsigned int rb;

    //8000000/50000 = 160 ticks per second
    //divide by 160
    num=ra;
    res=0;
    acc=0;
    den=160;
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
    //num=res;
    //res=0;
    //acc=0;
    //den=60;
    //for(rb=0x80000000;rb;rb>>=1)
    //{
        //acc<<=1;
        //if(rb&num) acc|=1;
        //if(acc>=den)
        //{
            //acc-=den;
            //res|=rb;
        //}
    //}
    return(res);
}


static unsigned char num[8];
static void show_number ( void )
{
    ht_write(0,num_bits[num[4]]);
    ht_write(2,num_bits[num[3]]);
    ht_write(4,num_bits[num[2]]);
    ht_write(6,num_bits[num[1]]);
    ht_write(8,num_bits[num[0]]);
}
int notmain ( void )
{
    unsigned int ra;

    ht_init();

    num[0]=10;
    num[1]=10;
    num[2]=10;
    num[3]=10;
    num[4]=10;
    num[5]=10;

    ra=GET32(RCCBASE+0x14);
    ra|=1<<17; //enable port a
    PUT32(RCCBASE+0x14,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(4<<1));
    ra&=~(3<<(5<<1));
    ra|=1<<(4<<1);
    ra|=1<<(5<<1);
    PUT32(GPIOABASE+0x00,ra);
    //OTYPER
    ra=GET32(GPIOABASE+0x04);
    ra&=~(1<<4);
    ra&=~(1<<5);
    PUT32(GPIOABASE+0x04,ra);
    PUT32(GPIOABASE+0x18,(1<<(4+16))|(1<<(5+16)));


    ra=GET32(RCCBASE+0x1C);
    ra|=1<<0; //enable timer 2
    PUT32(RCCBASE+0x1C,ra);

    PUT32(TIM2_CR1,0x00000000);
    PUT32(TIM2_ARR,(160*7*60)+160);
PUT32(TIM2_ARR,(160*2*60)+(10*160));
PUT32(TIM2_ARR,(160*1*60)+(5*160));
PUT32(TIM2_ARR,(15*160));
    PUT32(TIM2_CNT,0x00000000);
    PUT32(TIM2_PSC,50000-1);
    PUT32(TIM2_CR1,0x00000011);
    PUT32(TIM2_SR,0);
    while(1)
    {
        ra=GET32(TIM2_SR);
        if(ra&1) break;
        ra=GET32(TIM2_CNT);
        long_div(ra);
        num[1]=mo;
        num[2]=st;
        num[3]=so;
        show_number();
        if(mo==1)
        {
            if(so&1)
            {
                PUT32(GPIOABASE+0x18,(1<<(4+16))|(1<<(5+ 0)));
            }
            else
            {
                PUT32(GPIOABASE+0x18,(1<<(4+16))|(1<<(5+16)));
            }
        }
        if(mo==0)
        {
            if(so&1)
            {
                PUT32(GPIOABASE+0x18,(1<<(4+16))|(1<<(5+ 0)));
            }
            else
            {
                PUT32(GPIOABASE+0x18,(1<<(4+ 0))|(1<<(5+16)));
            }
        }
    }
    PUT32(TIM2_CR1,0x00000000);
    PUT32(TIM2_ARR,0xFFFFFFFF);
    PUT32(TIM2_CNT,0x00000160);
//PUT32(TIM2_CNT,160*9*60);
    PUT32(TIM2_PSC,50000-1);
    PUT32(TIM2_CR1,0x00000001);
    while(1)
    {
        ra=GET32(TIM2_CNT);
        if(ra<(160*10*60))
        {
            long_div(ra);
            num[1]=mo;
            num[2]=st;
            num[3]=so;
            show_number();
        }
        if(ra&0x20)
        {
            PUT32(GPIOABASE+0x18,(1<<(4+16))|(1<<(5+ 0)));
        }
        else
        {
            PUT32(GPIOABASE+0x18,(1<<(4+ 0))|(1<<(5+16)));
        }
    }

    return(0);
}
