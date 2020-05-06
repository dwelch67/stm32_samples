
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
void ienable ( void );

#define GPIOABASE   0x50000000
#define GPIOCBASE   0x50000800
#define RCCBASE     0x40021000

#define UART2_BASE 0x40004400

#define STK_CSR     0xE000E010
#define STK_RVR     0xE000E014
#define STK_CVR     0xE000E018
#define STK_MASK    0x00FFFFFF

const unsigned int seven_up[16]=
{
    0x00880077,
    0x00BE0041,
    0x001900E6,
    0x001C00E3,
    0x002E00D1,
    0x004C00B3,
    0x004800B7,
    0x009E0061,
    0x000800F7,
    0x000C00F3,

    0x00FF0000,
    0x00FF0000,
    0x00FF0000,
    0x00FF0000,
    0x00FF0000,
    0x00FF0000,
};
const unsigned int seven_dn[16]=
{
    0x00770088,
    0x004100BE,
    0x00E60019,
    0x00E3001C,
    0x00D1002E,
    0x00B3004C,
    0x00B70048,
    0x0061009E,
    0x00F70008,
    0x00F3000C,

    0x000000FF,
    0x000000FF,
    0x000000FF,
    0x000000FF,
    0x000000FF,
    0x000000FF,
};

//-------------------------------------------------------------------
//  **** WARNING THE CODE BELOW IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------
volatile unsigned int counter;
volatile unsigned int updown[2];
void systick_handler ( void )
{
    GET32(STK_CSR);
    PUT32(GPIOABASE+0x18,updown[counter&1]);
    counter++;
}
//-------------------------------------------------------------------
//  **** WARNING THE CODE ABOVE IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------

static int uart_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x2C);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x2C,ra);

    ra=GET32(RCCBASE+0x38);
    ra|=1<<17; //enable USART2
    PUT32(RCCBASE+0x38,ra);

    //PA9  UART1_TX
    //PA10 UART1_RX

    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<( 9<<1));  //PA9
    ra&=~(3<<(10<<1));  //PA10
    ra|=  2<<( 9<<1);   //PA9
    ra|=  2<<(10<<1);   //PA10
    PUT32(GPIOABASE+0x00,ra);

    //AFRL
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<4); //PA9
    ra&=~(0xF<<8); //PA10
    ra|= (0x4<<4); //PA9
    ra|= (0x4<<8); //PA10
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x28);
    ra|=1<<17; //reset USART2
    PUT32(RCCBASE+0x28,ra);
    ra&=~(1<<17);
    PUT32(RCCBASE+0x28,ra);

    //MSI clock
    //2097000/9600

    PUT32(UART2_BASE+0x0C,218);
    PUT32(UART2_BASE+0x08,1<<12);
    PUT32(UART2_BASE+0x00,(1<<3)|(1<<2)|1);

    return(0);
}

//static void uart_send ( unsigned int x )
//{
    //while(1) if(GET32(UART2_BASE+0x1C)&(1<<7)) break;
    //PUT32(UART2_BASE+0x28,x);
//}

static unsigned int uart_recv ( void )
{
    while(1) if((GET32(UART2_BASE+0x1C))&(1<<5)) break;
    return(GET32(UART2_BASE+0x24));
}



int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int digit;
    unsigned int digits[4];
    unsigned int newnum[6];

    uart_init();

    ra=GET32(RCCBASE+0x2C);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x2C,ra);
    //moder
    ra=GET32(GPIOABASE+0x00);
    ra&=~(3<<(0<<1)); //PA0
    ra&=~(3<<(1<<1)); //PA1
    ra&=~(3<<(2<<1)); //PA2
    ra&=~(3<<(3<<1)); //PA3
    ra&=~(3<<(4<<1)); //PA4
    ra&=~(3<<(5<<1)); //PA5
    ra&=~(3<<(6<<1)); //PA6
    ra&=~(3<<(7<<1)); //PA7
    ra|=  1<<(0<<1) ; //PA0
    ra|=  1<<(1<<1) ; //PA1
    ra|=  1<<(2<<1) ; //PA2
    ra|=  1<<(3<<1) ; //PA3
    ra|=  1<<(4<<1) ; //PA4
    ra|=  1<<(5<<1) ; //PA5
    ra|=  1<<(6<<1) ; //PA6
    ra|=  1<<(7<<1) ; //PA7
    PUT32(GPIOABASE+0x00,ra);

    ra=GET32(RCCBASE+0x2C);
    ra|=1<<2; //enable port c
    PUT32(RCCBASE+0x2C,ra);
    //moder
    ra=GET32(GPIOCBASE+0x00);
    ra&=~(3<<(14<<1)); //PC13
    ra&=~(3<<(15<<1)); //PC14
    PUT32(GPIOCBASE+0x00,ra);
    digit=GET32(GPIOCBASE+0x10);
    digit>>=14;
    digit&=3;
    ra=GET32(RCCBASE+0x2C);
    ra&=(~(1<<2)); //disable port c
    PUT32(RCCBASE+0x2C,ra);

    counter=0;
    updown[0]=seven_up[digit];
    updown[1]=seven_dn[digit];
    PUT32(STK_CSR,4);
    PUT32(STK_RVR,20000-1);
    PUT32(STK_CVR,20000-1);
    PUT32(STK_CSR,7);
    ienable();

    digits[0]=0x31;
    digits[1]=0x32;
    digits[2]=0x33;
    digits[3]=0x34;

    while(1)
    {
        ra=uart_recv();
        if(ra!='X') continue;
        newnum[0]=uart_recv();
        newnum[1]=uart_recv();
        newnum[2]=uart_recv();
        newnum[3]=uart_recv();
        newnum[4]=uart_recv();
        newnum[6]=uart_recv();
        ra=uart_recv();
        if(ra!='Y') continue;
        digits[0]=(newnum[0]&0xF);
        digits[1]=(newnum[1]&0xF);
        digits[2]=(newnum[2]&0xF);
        digits[3]=(newnum[3]&0xF);


        while((counter&0x20)==0) continue;
        while((counter&0x20)!=0) continue;
        for(ra=0;ra<4;ra++)
        {
            while((counter&0x20)==0) continue;
            updown[0]=seven_up[digits[ra]];
            updown[1]=seven_dn[digits[ra]];
            while((counter&0x20)!=0) continue;
            updown[0]=seven_up[0x0A];
            updown[1]=seven_dn[0x0A];
        }
        updown[0]=seven_up[0x0A];
        updown[1]=seven_dn[0x0A];

        for(ra=0;ra<4;)
        {
            rb=uart_recv();
            if(rb=='X') ra++;
        }
    }
    return(0);
}
