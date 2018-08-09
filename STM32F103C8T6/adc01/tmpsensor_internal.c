
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );
void sleep(void);

#define RCC_BASE     0x40021000
#define ADC_BASE     0x40012400
#define GPIOA_BASE   0x40010800
#define GPIOB_BASE   0x40010C00
#define GPIOC_BASE   0x40011000

#define ADC_CR1      (ADC_BASE + 0x04)
#define ADC_CR2      (ADC_BASE + 0x08)
#define ADC_SMPR1    (ADC_BASE + 0x0C)
#define ADC_SQR1     (ADC_BASE + 0x2C)
#define ADC_SQR3     (ADC_BASE + 0x34)
#define ADC_SR       (ADC_BASE + 0x00)
#define ADC_DR       (ADC_BASE + 0x4C)

#define STK_CTRL     (0xE000E010+0x00)
#define STK_LOAD     (0xE000E010+0x04)
#define STK_VAL      (0xE000E010+0x08)

#define USART1_BASE  0x40013800
#define USART1_SR    (USART1_BASE+0x00)
#define USART1_DR    (USART1_BASE+0x04)
#define USART1_BRR   (USART1_BASE+0x08)
#define USART1_CR1   (USART1_BASE+0x0C)
#define USART1_CR2   (USART1_BASE+0x10)
#define USART1_CR3   (USART1_BASE+0x14)
#define USART1_GTPR  (USART1_BASE+0x18)

#define GPIOA_CRL    (GPIOA_BASE+0x00)
#define GPIOA_CRH    (GPIOA_BASE+0x04)
#define GPIOA_IDR    (GPIOA_BASE+0x08)
#define GPIOA_ODR    (GPIOA_BASE+0x0C)
#define GPIOA_BSRR   (GPIOA_BASE+0x10)
#define GPIOA_BRR    (GPIOA_BASE+0x14)
#define GPIOA_LCKR   (GPIOA_BASE+0x18)

#define GPIOB_CRL    (GPIOB_BASE+0x00)
#define GPIOB_CRH    (GPIOB_BASE+0x04)
#define GPIOB_IDR    (GPIOB_BASE+0x08)
#define GPIOB_ODR    (GPIOB_BASE+0x0C)
#define GPIOB_BSRR   (GPIOB_BASE+0x10)
#define GPIOB_BRR    (GPIOB_BASE+0x14)
#define GPIOB_LCKR   (GPIOB_BASE+0x18)

#define GPIOC_CRL    (GPIOC_BASE+0x00)
#define GPIOC_CRH    (GPIOC_BASE+0x04)
#define GPIOC_IDR    (GPIOC_BASE+0x08)
#define GPIOC_ODR    (GPIOC_BASE+0x0C)
#define GPIOC_BSRR   (GPIOC_BASE+0x10)
#define GPIOC_BRR    (GPIOC_BASE+0x14)
#define GPIOC_LCKR   (GPIOC_BASE+0x18)

#define RCC_APB2ENR  (RCC_BASE+0x18)
#define RCC_APB2RSTR (RCC_BASE+0x0C)
#define RCC_CFGR     (RCC_BASE+0x04)


/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

void print_message(char *msg, unsigned int length)
{

    while (*msg)
    {
        PUT32(USART1_DR, *msg);
        sleep();
        msg++;
    }
    PUT32(USART1_DR,'\r');
    sleep();
    PUT32(USART1_DR,'\n');
    sleep();

}

// integer division
int mydiv(int value, int divisor)
{
    int quotient=0;
    int remainder=0;
    int bitcount;
    int value_sign=1;
    int divisor_sign;
    
    if (value<0)
    {
        value = -value;
        value_sign=-1;
    } else {
        divisor_sign = 1;
    }
    
    if (divisor<0)
    {
        divisor=-divisor;
        divisor_sign=-1;
    } else {
        divisor_sign = 1;
    }
    
    if (divisor == 0) return 0xffffffff;

    for (bitcount = 31; bitcount>=0; bitcount--)
    {
        remainder <<= 1;    

        if (value & (1<<bitcount)) remainder|=1;
    
        if (remainder >= divisor)
        {
                remainder -= divisor;
            quotient |= (1<<bitcount);
        }
    }
    return (quotient * value_sign * divisor_sign);
}

// integer modulo
int mymod(int value, int divisor)
{
    int quotient=0;
    int remainder=0;
    int bitcount;
    int value_sign=1;
    
    if (value<0)
    {
        value = -value;
        value_sign=-1;
    }
    
    if (divisor<0)
    {
        divisor=-divisor;
    
    }
    
    if (divisor == 0) return 0xffffffff;

    for (bitcount = 31; bitcount>=0; bitcount--)
    {
        remainder <<= 1;    

        if (value & (1<<bitcount)) remainder|=1;
    
        if (remainder >= divisor)
        {
                remainder -= divisor;
            quotient |= (1<<bitcount);
        }
    }
    // ignore divisor sign on mod function (according to gcc)
    return (remainder * value_sign);
}

void sendi(unsigned int i)
{
    //printi (out, *varg++, 10, 1, width, pad, 'a');
    char *s;
    char b = 10;
    char sg = 1;
    char letbase = 'a';
    
    char print_buf[PRINT_BUF_LEN];
    
    char t;
    char u = i;

    // if (i == 0) {
    //     print_buf[0] = '0';
    //     print_buf[1] = '\0';
    //     return prints (out, print_buf, width, pad);
    // }

    if (sg && b == 10 && i < 0) {
        u = -i;
    }

    s = print_buf + PRINT_BUF_LEN-1;
    *s = '\0';

    while (u) {
        // t = u % b ;
        t = mymod(u,b);
        if( t >= 10 )
            t += letbase - '0' - 10;
        *--s = t + '0';
        // u /= b
        u = mydiv(u,b);
    }

    print_message(s, 0);
}

// turn on LED
// turn ADC on using bit 0 of ADC_CR2
// make ADC cont using bit 1 of ADC_CR2
// make ADC cal using bit 3
// flash LED while bit 3 == 1
// turn off LED
// 

void print_message(char *msg, unsigned int length);

void init_led_uart_adc1();
void set_adc_clock_rate(void);
void deinit_adc1(void);
void init_adc1(void);
void wakeup_temp_sensor(void);
void wakeup_adc1(void);
void calibrate_adc(void);
void configure_adc(void);
void do_adc(void);

void led_on(void);
void led_off(void);
void led_blink(void);


void init_led_uart_adc1(void)
{

    unsigned int ra;

    ra=GET32(RCC_APB2ENR);
    ra|=1<<2;   //GPIOA
    ra|=1<<3;   //GPIOB
    ra|=1<<4;   //GPIOC
    ra|=1<<14;  //USART1
    ra|=1<<9;   //ADC1
    PUT32(RCC_APB2ENR,ra);

    //LED output PC13
    ra=GET32(GPIOC_CRH);
    ra&=~(3<<20);   //PC13
    ra|=1<<20;      //PC13
    ra&=~(3<<22);   //PC13
    ra|=0<<22;      //PC13
    PUT32(GPIOC_CRH,ra);

    //pa9 TX  alternate function output push-pull
    //pa10 RX configure as input floating
    ra=GET32(GPIOA_CRH);
    ra&=~(0xFF0);
    ra|=0x490;
    PUT32(GPIOA_CRH,ra);

    //PB1 Analog Input
    ra=GET32(GPIOB_CRH);
    ra&=~(1<<6);
    PUT32(GPIOB_CRH,ra);

    PUT32(USART1_CR1,0x2000);
    PUT32(USART1_CR2,0x0000);
    PUT32(USART1_CR3,0x0000);

    // 8000000/16 = 500000
    // 500000/115200 = 4.34
    // 4 and 5/16 = 4.3125
    // 4.3125 * 16 * 115200 = 7948800
    PUT32(USART1_BRR,0x0045);
    PUT32(USART1_CR1,0x200C);

    PUT32(STK_CTRL,4);
    PUT32(STK_LOAD,0x00FFFFFF);
    PUT32(STK_CTRL,5);

}

void set_adc_clock_rate(void)
{
    unsigned int ra;
    ra=GET32(RCC_CFGR);
    
    /* Clear ADCPRE[15:14] bits */
    ra &= ~(3<<14);
    
    //ADCCLK = PCLK22/6 = 72/6=12MHz
    ra |= 1<<15;
    PUT32(RCC_CFGR, ra);
}

void deinit_adc1(void)
{
    unsigned int ra;
    ra=GET32(RCC_APB2RSTR);

    // set bit 9 ADC1RST
    ra |= 1<<9;
    PUT32(RCC_APB2RSTR, ra);

    // now clear it
    ra &= ~(1<<9);
    PUT32(RCC_APB2RSTR, ra);  
    

    // ra=GET32(ADC_CR2);
 //    ra|=0; //disable ADON
 //    PUT32(ADC_CR2, ra);
}

void init_adc1(void)
{
    unsigned int ra;
    ra=GET32(ADC_CR1);

    // Clear DUALMOD[19-16] and SCAN[8] bits
    ra &= 0xFFF0FEFF;
    // set DUAL mode to independent (0000)
    // and SCAN mode to disabled (single conversion)
    ra |= 0x00000000;// only useful for demonstration purposes
    PUT32(ADC_CR1, ra);

    ra=GET32(ADC_CR2);

    /* Clear CONT, ALIGN and EXTTRIG bits */
    ra &= 0xFFF1F7FD;
    /* Set ALIGN bit to Right 12-bit data alignment
       Set EXTTRIG bits to No external triggering
       Set CONT bit to disabled (single conversion) */
    // ra |= 0x000E0000; // this is
    // ra |= 0 << 1;

    ra |= (0x00000000 | 0x000E0000 | (0 << 1)); 
    PUT32(ADC_CR2, ra);

    /* Get the ADC1 SQR1 value */
    ra = GET32(ADC_SQR1);
    /* Clear L[23-20] bits (number of conversions in the group)*/
    ra &= 0xFF0FFFFF;
    /* Configure regular channel sequence length 
       number of conversion in the group = 1*/
    unsigned int tmp = 0;
    unsigned nbrChannelsInGroup = 1;
    tmp |= (nbrChannelsInGroup - 1); // just for demo purposes
    
    ra |= (tmp << 20);
    PUT32(ADC_SQR1, ra);    

}

void wakeup_temp_sensor(void)
{
    unsigned int ra;
    ra = GET32(ADC_CR2);

    // enable bit 23 (TSVREFE)
    ra |= 1 << 23;
    PUT32(ADC_CR2, ra);
}

void wakeup_adc1(void)
{
    unsigned int ra;
    ra = GET32(ADC_CR2);

    // enable bit 0 (ADON)
    ra |= 0x00000001;
    PUT32(ADC_CR2, ra);
}

void calibrate_adc(void)
{

    // reset adc1 calibration
    unsigned int ra;
    ra = GET32(ADC_CR2);

    // enable bit 3 (RSTCAL)
    ra |= 1 << 3;
    PUT32(ADC_CR2, ra);
    
    // test calibration complete
    while ((GET32(ADC_CR2) & (1<<3)) != 0)
    {
        led_blink();
    }

    ra=GET32(ADC_CR2);

    // begin calibrate
    ra|=1<<2;
    PUT32(ADC_CR2, ra);

    // test calibration complete
    while ((GET32(ADC_CR2) & (1<<2)) != 0)
    {
        led_blink();
    }
}

void configure_adc(void)
{

    // /* Get the old register value */
    // tmpreg1 = ADCx->SMPR1;
    // /* Calculate the mask to clear */
    // tmpreg2 = (u32)SMPR1_SMP_Set << (3 * (ADC_Channel - 10));
    // /* Clear the old discontinuous mode channel count */
    // tmpreg1 &= ~tmpreg2;
    // /* Calculate the mask to set */
    // tmpreg2 = (u32)ADC_SampleTime << (3 * (ADC_Channel - 10));
    // /* Set the discontinuous mode channel count */
    // tmpreg1 |= tmpreg2;
    // /* Store the new register value */
    // ADCx->SMPR1 = tmpreg1;

    unsigned int ra, tmp;
    ra = GET32(ADC_SMPR1);

    // calculate the mask to clear the sample selection time
    // SMP16[2:0] bits 20-18; channel 16 is the internal temp sensor
    tmp = 0x07 << (3 * (16 - 10));
    //clear those bits first
    ra &= ~tmp;
    // set the bits to 000 (1.5 cycles)
    tmp = 0x00 << (3 * (16 - 10)); // again not entirely necessary here but just for completeness
    ra |= tmp;
    PUT32(ADC_SMPR1, ra);

    // here rank is the 1st conversion in the group sequence
    // so we need bits SQ1[0-4]
    unsigned int Rank = 1;
    ra=GET32(ADC_SQR3);

    // clear those bits
    // this just a way to calcualte their position in the
    // register (in this case the first 4 bits)
    tmp = 0x1F << (5 * (Rank - 1));
    ra &= ~tmp;

    // set these bits to the channel number
    tmp = 0x10 << (5 * (Rank - 1));
    ra |= tmp;

    PUT32(ADC_SQR3, ra);
}

void do_adc(void)
{
    unsigned int ra;
    ra=GET32(ADC_CR2);

    // set the SWSTART: start conversion of regular group
    // set the JSWSTART: start conversion of injected group
    // set the EXTTRIG: External trigger conversion mode for regular channels

    ra |= 0x05 << 20;
    PUT32(ADC_CR2, ra);

    // wait for EOC flag
    while ((GET32(ADC_SR) & 0x02) == 0)
    {
        print_message("waiting for end of conversion flag", 16);
    }

    print_message("value = ", 9);
    sendi(GET32(ADC_DR));

    // clear the EOC flag
    ra = GET32(ADC_CR2);

    ra &= ~(0x02);
    PUT32(ADC_CR2, ra);
    sleep();
}

void led_on(void)
{
    PUT32(GPIOC_BASE+0x10,1<<(13+16));  
}

void led_off(void)
{
    PUT32(GPIOC_BASE+0x10,1<<(13+0));
}

void led_blink(void)
{
    led_on();
    sleep();
    led_off();
    sleep();
}

void sleep(void)
{
    unsigned int ra;
    for(ra=0;ra<100000;ra++) dummy(ra);
}

int notmain(void)
{   
    
    init_led_uart_adc1();
    print_message("starting", 9);
    
    set_adc_clock_rate();
    print_message("clock set", 10);
    
    deinit_adc1();
    print_message("deinit complete", 16);

    init_adc1();
    print_message("init complete", 14);
    
    wakeup_temp_sensor();
    //print_message("sensor woke", 12);
    
    wakeup_adc1();
    //print_message("adc1 woke", 10);
    
    calibrate_adc();
    //print_message("calibration complete", 21);

    configure_adc();

    while(1){print_message("sensing", 8);
    do_adc();}
    return(0);
}
