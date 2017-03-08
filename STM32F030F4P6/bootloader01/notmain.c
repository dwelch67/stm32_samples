
//------------------------------------------------------------------------
//------------------------------------------------------------------------

//4 Kbytes 0x1000
//16 Kbytes 0x4000
#define SRAMSTART 0x20000000
//leave room for the stack used by this program.
#define SRAMEND   (0x20000FFF-64)

//PA9  USART1_TX
//PA10 USART1_RX

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
void PUT8 ( unsigned int, unsigned int );
void BRANCHTO ( unsigned int );

#define GPIOABASE   0x48000000
#define RCCBASE     0x40021000
#define USART1BASE  0x40013800

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

    //PA9  UART1_TX
    //PA10 UART1_RX

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

    PUT32(USART1BASE+0x0C,69);
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
    while(1) if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    return(GET32(USART1BASE+0x24));
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
//------------------------------------------------------------------------
static unsigned int ctonib ( unsigned int c )
{
    if(c>0x39) c-=7;
    return(c&0xF);
}
//-------------------------------------------------------------------------
static int mem_write_byte ( unsigned int a, unsigned int d )
{
    if(a<SRAMSTART)
    {
        hexstring(a);
        return(1);
    }
    if(a>SRAMEND)
    {
        hexstring(a);
        return(1);
    }
    //hexstrings(a);
    //hexstrings(d);
    PUT8(a,d);
    return(0);
}
//-------------------------------------------------------------------------
void hello ( void )
{
    hexstring(0x12345678);
    //hexstring(1);
    uart_send(0x0D);
    uart_send(0x0A);
    uart_send('S');
    uart_send('R');
    uart_send('E');
    uart_send('C');
    uart_send(0x0D);
    uart_send(0x0A);
}
//-------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int state;
    unsigned int ra;
    unsigned int type;
    unsigned int count;
    unsigned int sum;
    unsigned int entry;
    unsigned int addr;
    unsigned int data;

    uart_init();
    hello();
    PUT32(SRAMSTART,0xFFFFFFFF);

    state=0;
    count=0;
    sum=0;
    addr=0;
    type=0;
    data=0;
    entry=SRAMSTART;
    while(1)
    {
        ra=uart_recv();
//hexstring(ra);
//hexstring(state);
        switch(state)
        {
            case 0:
            {
                if(ra=='S')
                {
                    sum=0;
                    state++;
                }
                else
                if(ra=='g')
                {
                    hexstring(entry);
                    BRANCHTO(entry|1);
                }
                else
                if(ra==0x1B)
                {
                    hello();
                }
                else
                if(ra==0x0A)
                {
                    hello();
                }
                //else uart_send(ra);
                break;
            }
            case 1:
            {
                switch(ra)
                {
                    case '0':
                    {
                        state=0;
                        break;
                    }
                    case '3':
                    {
                        type=3;
                        state++;
                        break;
                    }
                    case '7':
                    {
                        type=7;
                        state++;
                        break;
                    }
                    default:
                    {
                        hexstring(ra);
                        hexstring(0xBADBAD00);
                        return(1);
                    }
                }
                break;
            }

            case 2:
            {
                count=ctonib(ra);
                state++;
                break;
            }
            case 3:
            {
                count<<=4;
                count|=ctonib(ra);
                if(count<5)
                {
                    hexstring(0xBADBAD01);
                    return(1);
                }
                sum+=count&0xFF;
                addr=0;
                state++;
                break;
            }
            case  4:
            case  6:
            case  8:
            case 10:
            {
                addr<<=4;
                addr|=ctonib(ra);
                state++;
                break;
            }
            case  5:
            case  7:
            case  9:
            {
                count--;
                addr<<=4;
                addr|=ctonib(ra);
                sum+=addr&0xFF;
                state++;
                break;
            }
            case 11:
            {
                count--;
                addr<<=4;
                addr|=ctonib(ra);
                sum+=addr&0xFF;
                state++;
                break;
            }
            case 12:
            {
                data=ctonib(ra);
                state++;
                break;
            }
            case 13:
            {
                data<<=4;
                data|=ctonib(ra);
                sum+=data&0xFF;
                count--;
                if(count==0)
                {
                    if(type==7)
                    {
                        entry=addr;
                        hexstring(entry);
                    }
                    sum&=0xFF;
                    if(sum!=0xFF)
                    {
                        hexstring(0xBADBAD02);
                        return(1);
                    }
                    state=0;
                }
                else
                {
                    mem_write_byte(addr,data);
                    addr++;
                    state=12;
                }
                break;
            }
        }
    }
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
