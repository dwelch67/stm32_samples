
//------------------------------------------------------------------------
//------------------------------------------------------------------------

void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define RCCBASE 0x40021000
#define RCC_CR          (RCCBASE+0x00)
#define RCC_CFGR        (RCCBASE+0x04)
#define RCC_APB2RSTR    (RCCBASE+0x0C)
#define RCC_AHBENR      (RCCBASE+0x14)
#define RCC_APB2ENR     (RCCBASE+0x18)

#define GPIOABASE 0x48000000
#define GPIOA_MODER     (GPIOABASE+0x00)
#define GPIOA_OTYPER    (GPIOABASE+0x04)
#define GPIOA_OSPEEDR   (GPIOABASE+0x08)
#define GPIOA_PUPDR     (GPIOABASE+0x0C)
#define GPIOA_IDR       (GPIOABASE+0x10)
#define GPIOA_ODR       (GPIOABASE+0x14)
#define GPIOA_BSRR      (GPIOABASE+0x18)
#define GPIOA_LCKR      (GPIOABASE+0x1C)
#define GPIOA_AFRL      (GPIOABASE+0x20)
#define GPIOA_AFRH      (GPIOABASE+0x24)
#define GPIOA_BRR       (GPIOABASE+0x28)

#define USART1BASE 0x40013800
#define USART1_CR1      (USART1BASE+0x00)
#define USART1_CR3      (USART1BASE+0x08)
#define USART1_BRR      (USART1BASE+0x0C)
#define USART1_ISR      (USART1BASE+0x1C)
#define USART1_RDR      (USART1BASE+0x24)
#define USART1_TDR      (USART1BASE+0x28)

//------------------------------------------------------------------------
static int clock_init ( void )
{
    unsigned int ra;

    //switch to external clock.
    ra=GET32(RCC_CR);
    ra|=1<<16;
    PUT32(RCC_CR,ra);
    while(1) if(GET32(RCC_CR)&(1<<17)) break;
    ra=GET32(RCC_CFGR);
    ra&=~3;
    ra|=1;
    PUT32(RCC_CFGR,ra);
    while(1) if((GET32(RCC_CFGR)&3)==1) break;
    return(0);
}
//------------------------------------------------------------------------
static int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //enable port A
    PUT32(RCC_AHBENR,ra);

    ra=GET32(RCC_APB2ENR);
    ra|=1<<14; //enable USART1
    PUT32(RCC_APB2ENR,ra);

    //moder 10
    ra=GET32(GPIOA_MODER);
    ra&=~(3<<18); //PA9
    ra&=~(3<<20); //PA10
    ra|=2<<18; //PA9
    ra|=2<<20; //PA10
    PUT32(GPIOA_MODER,ra);
    //OTYPER 0
    ra=GET32(GPIOA_OTYPER);
    ra&=~(1<<9); //PA9
    ra&=~(1<<10); //PA10
    PUT32(GPIOA_OTYPER,ra);
    //ospeedr 11
    ra=GET32(GPIOA_OSPEEDR);
    ra|=3<<18; //PA9
    ra|=3<<20; //PA10
    PUT32(GPIOA_OSPEEDR,ra);
    //pupdr 00
    ra=GET32(GPIOA_PUPDR);
    ra&=~(3<<18); //PA9
    ra&=~(3<<20); //PA10
    PUT32(GPIOA_PUPDR,ra);
    //afr 0001
    ra=GET32(GPIOA_AFRH);
    ra&=~(0xF<<4); //PA9
    ra&=~(0xF<<8); //PA10
    ra|=0x1<<4; //PA9
    ra|=0x1<<8; //PA10
    PUT32(GPIOA_AFRH,ra);




    ra=GET32(RCC_APB2RSTR);
    ra|=1<<14; //reset USART1
    PUT32(RCC_APB2RSTR,ra);
    ra&=~(1<<14);
    PUT32(RCC_APB2RSTR,ra);

    //PUT32(USART1_BRR,69); //115200
    PUT32(USART1_BRR,833); //9600
    PUT32(USART1_CR3,1<<12);
    PUT32(USART1_CR1,(1<<3)|(1<<2)|1);

    return(0);
}
//------------------------------------------------------------------------
static void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1_ISR)&(1<<7)) break;
    PUT32(USART1_TDR,x);
}
//------------------------------------------------------------------------
//static unsigned int uart_recv ( void )
static unsigned int getbyte ( void )
{
    while(1) if((GET32(USART1_ISR))&(1<<5)) break;
    return(GET32(USART1_RDR));
}
//------------------------------------------------------------------------
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
//------------------------------------------------------------------------
static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}


//unsigned char getbyte ( void )
//{
    //if(nmeaoff>=nmealen) exit(1);
    //return(nmeadata[nmeaoff++]);
//}

//#define getbyte uart_recv


unsigned char buffer[128];
unsigned char section[32];
unsigned int buffoff;
unsigned int soff;
unsigned int state;
unsigned int sum;
unsigned int csum;
unsigned int nowtime;
unsigned int nowdate;
unsigned int nowvalid;
unsigned int newtime;
unsigned int newdate;
unsigned int newvalid;
unsigned int dstadjust;

static int process_time ( void )
{
    //printf("[%s]\n",section);
    newtime=0;
    if(soff>5)
    {
        unsigned int ra,rb;
        //newtime|=(section[0]&0xF)<<20;
        //newtime|=(section[1]&0xF)<<16;
        //newtime|=(section[2]&0xF)<<12;
        //newtime|=(section[3]&0xF)<< 8;
        //newtime|=(section[4]&0xF)<< 4;
        //newtime|=(section[5]&0xF)<< 0;
        for(ra=0,rb=20;ra<6;ra++,rb-=4)
        {
            newtime|=(section[ra]&0xF)<<rb;
        }
    }
    return(0);
}
static int process_date ( void )
{

    //printf("[%s]\n",section);
    newdate=0;
    if(soff>5)
    {
        unsigned int ra,rb;
        //newdate|=(section[0]&0xF)<<20;
        //newdate|=(section[1]&0xF)<<16;
        //newdate|=(section[2]&0xF)<<12;
        //newdate|=(section[3]&0xF)<< 8;
        //newdate|=(section[4]&0xF)<< 4;
        //newdate|=(section[5]&0xF)<< 0;
        for(ra=0,rb=20;ra<6;ra++,rb-=4)
        {
            newdate|=(section[ra]&0xF)<<rb;
        }
    }
    return(0);
}

//       012345                                            012345
//$GPRMC,192218.000,A,4030.9658,N,07955.1459,W,1.98,279.68,281214,,,A*71
//      1          2 3         4 5          6 7    8      9      ABC

static int process_buffer ( void )
{
    unsigned int ra;
    unsigned int comma;
    unsigned int h,m,s,mm,dd,yy;
    //printf("[%s]\n",(char *)buffer);

    //for(ra=0;buffer[ra];ra++) uart_send(buffer[ra]);
    //uart_send(0x0D);
    //uart_send(0x0A);

    comma=0;
    soff=0;
    for(ra=0;buffer[ra];ra++)
    {
        if(buffer[ra]==',')
        {
            comma++;
            section[soff]=0;
            switch(comma)
            {
                case 2:
                {
                    process_time();
                    break;
                }
                case 3:
                {
                    if(section[0]=='A') newvalid=1;
                    else newvalid=0;
                    break;
                }
                case 10:
                {
                    process_date();
                    break;
                }
            }
            soff=0;
        }
        else
        {
            section[soff++]=buffer[ra];
        }
    }
    //printf("%u %06X %06X\n",newvalid,newtime,newdate);
//hexstring(newtime);
//newvalid=0;
    if(newvalid)
    {
        s=(newtime>>4)&0xF;
        s=(s<<3)+(s<<1);
        //s=s*10;
        s+=(newtime>>0)&0xF;
        m=(newtime>>12)&0xF;
        m=(m<<3)+(m<<1);
        //m=m*10;
        m+=(newtime>>8)&0xF;
        h=(newtime>>20)&0xF;
        h=(h<<3)+(h<<1);
        //h=h*10;
        h+=(newtime>>16)&0xF;
        yy=(newdate>>4)&0xF;
        yy=(yy<<3)+(yy<<1);
        //yy=yy*10;
        yy+=(newdate>>0)&0xF;
        mm=(newdate>>12)&0xF;
        mm=(mm<<3)+(mm<<1);
        //mm=mm*10;
        mm+=(newdate>>8)&0xF;
        dd=(newdate>>20)&0xF;
        dd=(dd<<3)+(dd<<1);
        //dd=dd*10;
        dd+=(newdate>>16)&0xF;

        //printf("%02u:%02u:%02u  ",h,m,s);
        //printf("20%02u-%02u-%02u  ",yy,mm,dd);

        if(h<dstadjust)
        {
            h+=24;
            //take one day off
            if(dd>1)
            {
                dd--;
            }
            else
            {
                if(mm==1)
                {
                    mm=12;
                    //dont care about y3k and it is already past 2001...
                    yy--;
                }
                else mm--;
                switch(mm)
                {
                    case 1: dd=31; //jan
                    case 2: //feb
                    {
                        if(yy&3) dd=28;
                        else     dd=29;
                        //this thing wont be running in year > 2099
                        break;
                    }
                    case 3: dd=31; //mar
                    case 4: dd=30; //apr
                    case 5: dd=31; //may
                    case 6: dd=30; //jun
                    case 7: dd=31; //jul
                    case 8: dd=31; //aug
                    case 9: dd=30; //sep
                    case 10: dd=31; //oct
                    case 11: dd=30; //nov
                    case 12: dd=31; //dec
                }
            }
        }
        h-=dstadjust;
        //printf("%02u:%02u:%02u  ",h,m,s);
        //printf("20%02u-%02u-%02u\n",yy,mm,dd);

        nowtime=newtime&0xFFFF;
        ra=h;
        if(ra>=20) { nowtime|=2<<20; ra-=20; }
        if(ra>=10) { nowtime|=1<<20; ra-=10; }
        nowtime|=ra<<16;

        nowdate=0;
        ra=yy;
        if(ra>=30) { nowdate|=3<<20; ra-=30; }
        if(ra>=20) { nowdate|=2<<20; ra-=20; }
        if(ra>=10) { nowdate|=1<<20; ra-=10; }
        nowdate|=ra<<16;
        ra=mm;
        if(ra>=10) { nowdate|=1<<12; ra-=10; }
        nowdate|=ra<<8;
        ra=dd;
        if(ra>=30) { nowdate|=3<<4; ra-=30; }
        if(ra>=20) { nowdate|=2<<4; ra-=20; }
        if(ra>=10) { nowdate|=1<<4; ra-=10; }
        nowdate|=ra<<0;

        hexstrings(nowtime);
        hexstring(nowdate);
    }
    return(0);
}
//------------------------------------------------------------------------
int notmain ( void )
{
    //unsigned int ra;
    unsigned char newbyte;

    clock_init();
    uart_init();

//buffoff=0;
//soff=0;
//state=0;
//sum=0;
//csum=0;
//nowtime=0;
//nowdate=0;
//nowvalid=0;
//newtime=0;
//newdate=0;
//newvalid=0;
//dstadjust=0;


    //nmeaoff=0;
    state=0;
    sum=0;
    csum=0;
    newvalid=0;
    dstadjust=5;
//$GPRMC,192218.000,A,4030.9658,N,07955.1459,W,1.98,279.68,281214,,,A*71

    while(1)
    {
        newbyte=getbyte();
//uart_send(newbyte);
//uart_send(0x30+state);
//hexstring(state);
//printf("%c",newbyte);
        switch(state)
        {
            case 0: //search for $
            {
                if(newbyte=='$')
                {
                    sum=0;
                    buffoff=0;
//uart_send(newbyte);
//printf("\n");
                    state++;
                }
                break;
            }
            case 1:
            {
                if(newbyte=='G')
                {
                    buffer[buffoff++]=newbyte;
                    sum^=newbyte;;
                    state++;
                }
                else
                {
                    state=0;
                }
                break;
            }
            case 2:
            {
                if(newbyte=='P')
                {
                    buffer[buffoff++]=newbyte;
                    sum^=newbyte;;
                    state++;
                }
                else
                {
                    state=0;
                }
                break;
            }
            case 3:
            {
                if(newbyte=='R')
                {
                    buffer[buffoff++]=newbyte;
                    sum^=newbyte;;
                    state++;
                }
                else
                {
                    state=0;
                }
                break;
            }
            case 4:
            {
                if(newbyte=='M')
                {
                    buffer[buffoff++]=newbyte;
                    sum^=newbyte;;
                    state++;
                }
                else
                {
                    state=0;
                }
                break;
            }
            case 5:
            {
                if(newbyte=='C')
                {
                    buffer[buffoff++]=newbyte;
                    sum^=newbyte;;
                    state++;
                }
                else
                {
                    state=0;
                }
                break;
            }
            case 6:
            {
                if(newbyte=='*')
                {
                    csum=0;
                    buffer[buffoff]=0;
                    state++;
                }
                else
                {
                    buffer[buffoff++]=newbyte;
                    sum^=newbyte;;
                }
                break;
            }
            case 7:
            {
                if(newbyte>0x39) csum=newbyte-0x37;
                else             csum=newbyte-0x30;
                csum<<=4;
                state++;
                break;
            }
            case 8:
            {
                if(newbyte>0x39) csum+=newbyte-0x37;
                else             csum+=newbyte-0x30;
                if(sum==csum) process_buffer();
                //hexstrings(sum);
                //uart_send(0x55);
                //hexstring(csum);
                state=0;
                break;
            }
        }
    }
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
