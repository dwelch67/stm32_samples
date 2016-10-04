
//PA9  TX
//PA10 RX

void PUT32 ( unsigned int, unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET8 ( unsigned int );
void dummy ( unsigned int );

#define STK_CTRL (0xE000E010+0x00)
#define STK_LOAD (0xE000E010+0x04)
#define STK_VAL  (0xE000E010+0x08)

#define VTOR 0xE000ED08

#define USART1_BASE 0x40013800
#define USART1_SR   (USART1_BASE+0x00)
#define USART1_DR   (USART1_BASE+0x04)
#define USART1_BRR  (USART1_BASE+0x08)
#define USART1_CR1  (USART1_BASE+0x0C)
#define USART1_CR2  (USART1_BASE+0x10)
#define USART1_CR3  (USART1_BASE+0x14)
#define USART1_GTPR (USART1_BASE+0x18)

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
#define RCC_APB1ENR (RCC_BASE+0x1C)
#define RCC_CR      (RCC_BASE+0x00)
#define RCC_CFGR    (RCC_BASE+0x04)

#define FLASH_ACR   0x40022000

#define USB_BASE 0x40005C00
#define USB_CNTR    (USB_BASE+0x40)
#define USB_ISTR    (USB_BASE+0x44)
#define USB_FNR     (USB_BASE+0x48)
#define USB_DADDR   (USB_BASE+0x4C)
#define USB_BTABLE  (USB_BASE+0x50)
#define USB_EP0R    (USB_BASE+0x00)
#define USB_EP1R    (USB_BASE+0x04)
#define USB_EP2R    (USB_BASE+0x08)
#define USB_EP3R    (USB_BASE+0x0C)
//#define USB_    (USB_BASE+0x)
//#define USB_    (USB_BASE+0x)

//MAKE USB_BTABLE 0 for these to work!!
#define BTABLE 0

#define USB_SRAM        0x40006000
//#define USB_ADDR0_TX    (USB_SRAM+BTABLE+(0*16)+0)
//#define USB_COUNT0_TX   (USB_SRAM+BTABLE+(0*16)+4)
//#define USB_ADDR0_RX    (USB_SRAM+BTABLE+(0*16)+8)
//#define USB_COUNT0_RX   (USB_SRAM+BTABLE+(0*16)+12)
//this drove me crazy, makes no sense
#define USB_ADDR0_TX    (USB_SRAM+BTABLE+(0*8)+0)
#define USB_COUNT0_TX   (USB_SRAM+BTABLE+(0*8)+2)
#define USB_ADDR0_RX    (USB_SRAM+BTABLE+(0*8)+4)
#define USB_COUNT0_RX   (USB_SRAM+BTABLE+(0*8)+8)

#define PMA_BASE (0x40006000)
#define EP0_TX_ADDR (0x0020)
#define EP0_RX_ADDR (0x0060)
#define EP1_TX_ADDR (0x00a0)
#define EP2_RX_ADDR (0x00e0)
#define EP3_TX_ADDR (0x0120)



static void clock_init ( void )
{
    unsigned int ra;
    unsigned int cfgr;

    ra=GET32(RCC_CR);
    ra=ra|1<<16; //HSEON
    PUT32(RCC_CR,ra);

    while(1) if(GET32(RCC_CR)&(1<<17)) break; //HSERDY

    cfgr=0x00000000;
    cfgr|= (0x7<<18); //PLL input clock x 9
    cfgr|= (1<<16);   //PLLSRC PREDIV1 selected as PLL input clock
    PUT32(RCC_CFGR,cfgr);

    ra=GET32(RCC_CR);
    ra=ra|1<<24;    //PLLON
    PUT32(RCC_CR,ra);

    while(1) if(GET32(RCC_CR)&(1<<25)) break; //PLLRDY

    //cfgr|= (0x7<<24); //MCO SYSCLK/2 must not exceed 50MHz
    cfgr|= (0x0<<22); //USB 48MHz
    cfgr|= (0x4<<8);  //APB1 36MHz
    PUT32(RCC_CFGR,cfgr);

    PUT32(FLASH_ACR,0x2); //72MHz 2 wait states

    cfgr|= (0x2<<0); //PLL
    PUT32(RCC_CFGR,cfgr);

    while(1) if((GET32(RCC_CFGR)&0xF)==0xA) break; //Wait for it
}
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
    //72000000/(16*115200) = 39.0625
    //0.0625 * 16 = 1
    //39 1/16 0x27
    PUT32(USART1_BRR,0x0271); //72Mhz
    //PUT32(USART1_BRR,0x0045); //8Mhz
    PUT32(USART1_CR1,0x200C);
}
//static unsigned int uart_getc ( void )
//{
    //while(1)
    //{
        //if(GET32(USART1_SR)&0x20) break;
    //}
    //return(GET32(USART1_DR)&0xFF);
//}
static void uart_putc ( unsigned int c )
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
        uart_putc(rc);
        if(rb==0) break;
    }
    uart_putc(0x20);
}
static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_putc(0x0D);
    uart_putc(0x0A);
}
unsigned int uart_check ( unsigned int *data )
{
    if(GET32(USART1_SR)&0x20)
    {
        *data=GET32(USART1_DR)&0xFF;
        return(1);
    }
    return(0);
}

static unsigned int dhead;
static unsigned int dtail;
static unsigned int dlog[512];
void log_data ( unsigned int x )
{
    dlog[dtail]=x;
    dtail=(dtail+1)&0x1FF;
}
void show_log_data ( void )
{
    while(dhead!=dtail)
    {
        hexstring(dlog[dhead]);
        dhead=(dhead+1)&0x1FF;
    }
}


int notmain ( void )
{
    unsigned int ra;

static unsigned int state;
static unsigned int myadd;

    PUT32(VTOR,0x00000000); //just in case
    clock_init();
    uart_init();
    //hexstring(0x12345678);
    dhead=0;
    dtail=0;
    log_data(0x12345678);

    ra=GET32(RCC_APB1ENR);
    ra|=1<<23; //UNDOCUMENTED USB ENABLE!
    PUT32(RCC_APB1ENR,ra);

    PUT16(USB_CNTR,0x0001); //Clear PWDN (power up)
    for(ra=0;ra<3000;ra++) dummy(ra); //should be well over tSTARTUP
    PUT16(USB_CNTR,0x0000); //Clear FRES (release reset)
    PUT16(USB_ISTR,0x0000); //Clear RESET and any other interrupts

    ra=GET32(GPIOA_CRH);
    ra&=~(0xF<<12);
    ra|= (0xB<<12); //1011
    ra&=~(0xF<<16);
    ra|= (0xB<<12); //1011
    PUT32(GPIOA_CRH,ra);

    //ATTACHED?
    state=0; //ATTACHED
    myadd=0;

    while(1)
    {
        unsigned int istr;

        unsigned int x;
        if(uart_check(&x))
        {
            if(x==0x20) show_log_data();
        }

        istr=GET16(USB_ISTR);
        if(istr&0x0400)  //RESET
        {
            log_data(istr|0x11000000);
            PUT16(USB_ISTR,~0x0400);
            PUT16(USB_BTABLE,BTABLE);
            PUT16(USB_COUNT0_RX,64);
            PUT16(USB_ADDR0_RX,0x40);
            PUT16(USB_COUNT0_TX,0);
            PUT16(USB_ADDR0_TX,0x80|myadd);
            ra=GET16(USB_EP0R);
            ra&=~(1<<15); //CTR_RX
            ra^= (3<<12); //STAT_RX  RX_VALID
            ra&=~(3<<9);  //EP_TYPE
            ra|= (1<<9);  //EP_TYPE CONTROL
            ra&=~(1<<8);  //EP_KIND
            ra&=~(1<<7);  //CTR_TX
            ra^= (2<<4);  //TX_NAK
            PUT16(USB_EP0R,ra);
            PUT16(USB_DADDR,0x0080);
            state=1; //DEFAULT
        }
        if(istr&0x8000)  //CTR
        {
            log_data(istr|0x22000000);
            log_data(istr&0xF);
            if((istr&0xF)==0)
            {
                //endpoint 0
                if(istr&0x10) //DIR
                {
                    //CTR_RX, OUT HOST->USB
                    ra=GET16(USB_EP0R);
                    if(ra&0x0800) //SETUP
                    {
                        ra=GET16(USB_EP0R);
                        ra&=~(1<<15); //CTR_RX
                        PUT16(USB_EP0R,ra);
                        {
                            unsigned int rb;
                            unsigned int r0,r1;//,r2,r3;
                            rb=USB_SRAM+(0x40<<1);
                            r0=GET16(rb); rb+=4;
                            r1=GET16(rb); rb+=4;
                            //r2=GET16(rb); rb+=4;
                            //r3=GET16(rb); rb+=4;
                            log_data(0x123123);
                            log_data(r0);
                            log_data(r1);
                            //log_data(r2);
                            //log_data(r3);
                            switch(r0)
                            {
                                case 0x0680: //GET_DESCRIPTOR
                                {
                                    //wValue Descriptor Type & Index
                                    //wIndex Zero or Language ID
                                    //wLength Descriptor Length
                                    switch(r1&0xFF00)
                                    {
                                        case 0x0100: //Device Descriptor
                                        {
                                            rb=USB_SRAM+(0x80<<1);
                                            PUT16(rb,0x0112); rb+=4; //descriptor, size
                                            PUT16(rb,0x0200); rb+=4; //bcdUSB?
                                            PUT16(rb,0x0102); rb+=4; //bDeviceSubClass, bDeviceClass
                                            PUT16(rb,0x4003); rb+=4; //bMaxPacketSize, bDeviceProtocol
                                            PUT16(rb,0xDEAD); rb+=4; //VID
                                            PUT16(rb,0xBEEF); rb+=4; //PID
                                            PUT16(rb,0x0200); rb+=4; //bcdDevice rel 2.00
                                            PUT16(rb,0x0201); rb+=4; //string index product, string index manufacturer
                                            PUT16(rb,0x0103); rb+=4; //bNumConfigurations, string index serial number
                                            PUT16(USB_COUNT0_TX,0x12);
                                            PUT16(USB_ADDR0_TX,0x80);
                                            ra=GET16(USB_EP0R);
                                            ra^= (3<<4);  //STAT_TX  TX_VALID
                                            PUT16(USB_EP0R,ra);
                                            break;
                                        }
                                        //case 0x0200: //Config Descriptor
                                        //{
                                            //break;
                                        //}
                                        //case 0x0300: //String Descriptor
                                        //{
                                            //break;
                                        //}
                                        //case 0x0400: //Interface
                                        //{
                                            //break;
                                        //}
                                        //case 0x0500: //Endpoint
                                        //{
                                            //break;
                                        //}
                                    }
                                    break;
                                }
                                default:
                                {
                                //hexstrings(r0); //bmRequest
                                //hexstrings(r1); //wValue
                                //hexstrings(r2); //wIndex
                                //hexstring (r3); //wLength
                                break;
                                }
                            }


                        }
                    }
                    else //not setup
                    if(ra&0x8000)
                    {
                        ra=GET16(USB_EP0R);
                        ra&=~(1<<15); //CTR_RX
                        ra^= (3<<12); //STAT_RX  RX_VALID
                        PUT16(USB_EP0R,ra);
                    }
                }
                else
                {
                    //CTR_TX, IN USB->HOST
                    ra=GET16(USB_EP0R);
                    ra&=~(1<<7); //CTR_TX
                    PUT16(USB_EP0R,ra);
                    if(state==1)
                    {
                        PUT16(USB_DADDR,0x0080|myadd);
                        state=2; //ADDRESS
                    }
                    ra=GET16(USB_EP0R);
                    ra^= (3<<12); //STAT_RX  RX_VALID
                    PUT16(USB_EP0R,ra);
                }
            }
        }
    }

    //while(1)
    //{
        //ra=uart_getc();
        ////hexstring(ra);
    //}
    return(0);
}
/*

halt
flash write_image erase wip/wip.thumb.elf



*/
