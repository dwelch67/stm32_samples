
void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define GPIOB_BASE 0x48000400

#define GPIOA_BASE 0x48000000
#define GPIOA_MODER     (GPIOA_BASE+0x00)
#define GPIOA_AFRL      (GPIOA_BASE+0x20)
#define GPIOA_AFRH      (GPIOA_BASE+0x24)

#define TIM16_BASE 0x40014400
#define TIM16_CR1       (TIM16_BASE+0x00)
#define TIM16_DIER      (TIM16_BASE+0x0C)
#define TIM16_SR        (TIM16_BASE+0x10)
//#define TIM16_EGR       (TIM16_BASE+0x14)
#define TIM16_CCMR1     (TIM16_BASE+0x18)
#define TIM16_CCER      (TIM16_BASE+0x20)
#define TIM16_CNT       (TIM16_BASE+0x24)
#define TIM16_PSC       (TIM16_BASE+0x28)
#define TIM16_ARR       (TIM16_BASE+0x2C)
//#define TIM16_CCR1      (TIM16_BASE+0x34)
#define TIM16_BDTR      (TIM16_BASE+0x44)

#define TIM17_BASE 0x40014800
#define TIM17_CR1       (TIM17_BASE+0x00)
//#define TIM17_SR        (TIM17_BASE+0x10)
#define TIM17_EGR       (TIM17_BASE+0x14)
#define TIM17_CCMR1     (TIM17_BASE+0x18)
#define TIM17_CCER      (TIM17_BASE+0x20)
#define TIM17_CNT       (TIM17_BASE+0x24)
#define TIM17_PSC       (TIM17_BASE+0x28)
#define TIM17_ARR       (TIM17_BASE+0x2C)
#define TIM17_CCR1      (TIM17_BASE+0x34)
#define TIM17_BDTR      (TIM17_BASE+0x44)


#define RCC_BASE        0x40021000
#define RCC_AHBENR      (RCC_BASE+0x14)
#define RCC_APB2ENR     (RCC_BASE+0x18)


static void do_on ( void )
{
    PUT16(TIM16_CCMR1,(5<<4));
}
static void do_off ( void )
{
    PUT16(TIM16_CCMR1,(4<<4));
}

#define CAR 210
#define ENV (CAR*21)




int notmain ( void )
{
    unsigned int ra;
    unsigned int rz;
    unsigned int rx;
    unsigned int data;
    unsigned int bit;
    unsigned int rb;

    ra=GET32(RCC_APB2ENR);
    ra|=1<<17; //TIM16
    ra|=1<<18; //TIM17
    PUT32(RCC_APB2ENR,ra);

    ra=GET32(RCC_AHBENR);
    ra|=1<<17; //IOPA
    PUT32(RCC_AHBENR,ra);

if(1)
{
    ra=GET32(GPIOA_MODER);
    ra&=(~(3<<(7<<1))); //PA7
    ra|=  (2<<(7<<1));  //PA7 alternate function mode
    PUT32(GPIOA_MODER,ra);

    ra=GET32(GPIOA_AFRL);
    ra&=(~(0xF<<((7-0)<<2))); //PA7
    ra|=  (0x5<<((7-0)<<2));  //PA7 AF5 TIM17_CH1
    PUT32(GPIOA_AFRL,ra);
}

    ra=GET32(GPIOA_MODER);
    ra&=(~(3<<(13<<1))); //PA13
    ra|=  (2<<(13<<1));  //PA13 alternate function mode
    PUT32(GPIOA_MODER,ra);

    ra=GET32(GPIOA_AFRH);
    ra&=(~(0xF<<((13-8)<<2))); //PA13
    ra|=  (0x1<<((13-8)<<2));  //PA13 AF1 IR_OUT
    PUT32(GPIOA_AFRH,ra);

    //carrier
    PUT16(TIM17_PSC,0);
    PUT16(TIM17_ARR,CAR/2);
    PUT16(TIM17_CCR1,CAR/2);
    PUT16(TIM17_CCMR1,(3<<4)|(1<<3));
    PUT16(TIM17_CCER,(1<<0));
    PUT16(TIM17_BDTR,(1<<15));
    PUT16(TIM17_CR1,(1<<0));

    //data
    PUT16(TIM16_PSC,ENV-1);
    PUT16(TIM16_ARR,10);
    //PUT16(TIM16_CCMR1,(5<<4));
    do_off();
    PUT16(TIM16_CCER,(1<<0));
    PUT16(TIM16_BDTR,(1<<15));
    //PUT16(TIM16_DIER,(1<<0));
    PUT16(TIM16_CR1,(1<<0));

rx=0;


    ////rx=GET16(TIM16_CNT);
    //rx=0;
    //do_off();
    //for(ra=0;ra<100;)
    //{
        //rz=GET16(TIM16_CNT);
        //if(rz!=rx)
        //{
            //ra++;
            //rx=rz;
        //}
    //}

    //do_on();
    //return(1);


    //while(1)
    //{
        //do_on();
        //for(ra=0;ra<40;)
        //{
            //rz=GET16(TIM16_CNT);
            //if(rz!=rx)
            //{
                //ra++;
                //rx=rz;
            //}
        //}
        //do_off();
        //for(ra=0;ra<10;)
        //{
            //rz=GET16(TIM16_CNT);
            //if(rz!=rx)
            //{
                //ra++;
                //rx=rz;
            //}
        //}
    //}



    do_off();
    for(ra=0;ra<40;)
    {
        rz=GET16(TIM16_CNT);
        if(rz!=rx)
        {
            ra++;
            rx=rz;
        }
    }
    do_on();
    for(ra=0;ra<16;)
    {
        rz=GET16(TIM16_CNT);
        if(rz!=rx)
        {
            ra++;
            rx=rz;
        }
    }
    do_off();
    for(ra=0;ra<8;)
    {
        rz=GET16(TIM16_CNT);
        if(rz!=rx)
        {
            ra++;
            rx=rz;
        }
    }
    data=0xA10C0807;
    for(bit=0x80000000;bit;bit>>=1)
    {
        do_on();
        while(1)
        {
            rz=GET16(TIM16_CNT);
            if(rz!=rx)
            {
                rx=rz;
                break;
            }
        }
        do_off();
        if(bit&data)
        {
            for(ra=0;ra<3;)
            {
                rz=GET16(TIM16_CNT);
                if(rz!=rx)
                {
                    ra++;
                    rx=rz;
                }
            }
        }
        else
        {
            while(1)
            {
                rz=GET16(TIM16_CNT);
                if(rz!=rx)
                {
                    rx=rz;
                    break;
                }
            }
        }
    }
    do_on();
    while(1)
    {
        rz=GET16(TIM16_CNT);
        if(rz!=rx)
        {
            rx=rz;
            break;
        }
    }
    do_off();
    for(ra=0;ra<80;)
    {
        rz=GET16(TIM16_CNT);
        if(rz!=rx)
        {
            ra++;
            rx=rz;
        }
    }
    do_on();
    for(ra=0;ra<16;)
    {
        rz=GET16(TIM16_CNT);
        if(rz!=rx)
        {
            ra++;
            rx=rz;
        }
    }
    do_off();
    for(ra=0;ra<4;)
    {
        rz=GET16(TIM16_CNT);
        if(rz!=rx)
        {
            ra++;
            rx=rz;
        }
    }
    do_on();
    while(1)
    {
        rz=GET16(TIM16_CNT);
        if(rz!=rx)
        {
            rx=rz;
            break;
        }
    }
    do_off();



    return(0);
}
