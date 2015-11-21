
//------------------------------------------------------------------------
//------------------------------------------------------------------------

void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void BRANCHTO ( unsigned int );
void dummy ( unsigned int );

#define USART1BASE  0x40013800

#define GPIOABASE   0x50000000
#define GPIOBBASE   0x50000400
#define RCCBASE     0x40021000

//------------------------------------------------------------------------
void clock_init ( void )
{
    unsigned int ra;

    ra=GET32(RCCBASE+0x00);
    ra|=1<<0;
    PUT32(RCCBASE+0x00,ra);
    while(1)
    {
        ra=GET32(RCCBASE+0x00);
        if(ra&(1<<2)) break;
    }
    PUT32(RCCBASE+0x0C,1);
}
//------------------------------------------------------------------------
void uart_init ( void )
{
    unsigned int ra;

    //need HSI16 up
    //ra=GET32(RCCBASE+0x00);
    //ra|=1<<0;
    //PUT32(RCCBASE+0x00,ra);
    //while(1)
    //{
        //ra=GET32(RCCBASE+0x00);
        //if(ra&(1<<2)) break;
    //}

    ra=GET32(RCCBASE+0x2C);
    ra|=1<<0; //enable port a
    PUT32(RCCBASE+0x2C,ra);

    ra=GET32(RCCBASE+0x34);
    ra|=1<<14; //enable USART1
    PUT32(RCCBASE+0x34,ra);

    ra=GET32(RCCBASE+0x4C);
    ra&=~(3<<0);
    ra|=2<<0; //HSI16 clock for uart
    PUT32(RCCBASE+0x4C,ra);


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
    //afr4 0100
    ra=GET32(GPIOABASE+0x24);
    ra&=~(0xF<<4); //PA9
    ra|=0x4<<4; //PA9 AF4 USART1_TX
    ra&=~(0xF<<8); //PA10
    ra|=0x4<<8; //PA10 AF4 USART1_RX
    PUT32(GPIOABASE+0x24,ra);

    ra=GET32(RCCBASE+0x24);
    ra|=1<<14; //reset USART1
    PUT32(RCCBASE+0x24,ra);
    ra&=~(1<<14);
    PUT32(RCCBASE+0x24,ra);

    PUT32(USART1BASE+0x0C,139);
    PUT32(USART1BASE+0x08,1<<12);
    PUT32(USART1BASE+0x00,(1<<3)|(1<<2)|1);
}
//------------------------------------------------------------------------
void uart_send ( unsigned int x )
{
    while(1) if(GET32(USART1BASE+0x1C)&(1<<7)) break;
    PUT32(USART1BASE+0x28,x);
}
//------------------------------------------------------------------------
unsigned int uart_recv ( void )
{
    while(1) if((GET32(USART1BASE+0x1C))&(1<<5)) break;
    return(GET32(USART1BASE+0x24));
}
//------------------------------------------------------------------------
void hexstrings ( unsigned int d )
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
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int state;
    unsigned int byte_count;
    unsigned int address;
    unsigned int record_type;
    unsigned int segment;
    unsigned int data;
    unsigned int sum;
    unsigned int ra;
    unsigned int min,max;

    clock_init();
    uart_init();
    hexstring(0x12345678);

    uart_send('I');
    uart_send('H');
    uart_send('E');
    uart_send('X');
    uart_send(0x0D);
    uart_send(0x0A);

    state=0;
    segment=0;
    sum=0;
    data=0;
    record_type=0;
    address=0;
    byte_count=0;
    min=0; min--;
    max=0;

    while(1)
    {
        ra=uart_recv();
        if(ra==':')
        {
            state=1;
            continue;
        }
        if(ra==0x0D)
        {
            state=0;
            continue;
        }
        if(ra==0x0A)
        {
            state=0;
            continue;
        }
        if((ra=='g')||(ra=='G'))
        {
            uart_send(0x0D);
            hexstring(min);
            hexstring(max);
            uart_send('-');
            uart_send('-');
            uart_send(0x0D);
            uart_send(0x0A);
            uart_send(0x0A);
            BRANCHTO(0x20000001);
            state=0;
            break;
        }
        switch(state)
        {
            case 0:
            {
                break;
            }
            case 1:
            case 2:
            {
                byte_count<<=4;
                if(ra>0x39) ra-=7;
                byte_count|=(ra&0xF);
                byte_count&=0xFF;
                state++;
                break;
            }
            case 3:
            case 4:
            case 5:
            case 6:
            {
                address<<=4;
                if(ra>0x39) ra-=7;
                address|=(ra&0xF);
                address&=0xFFFF;
                address|=segment;
                state++;
                break;
            }
            case 7:
            {
                record_type<<=4;
                if(ra>0x39) ra-=7;
                record_type|=(ra&0xF);
                record_type&=0xFF;
                state++;
                break;
            }
            case 8:
            {
                record_type<<=4;
                if(ra>0x39) ra-=7;
                record_type|=(ra&0xF);
                record_type&=0xFF;
                switch(record_type)
                {
                    case 0x00:
                    {
                        state=14;
                        break;
                    }
                    case 0x01:
                    {
                        hexstring(sum);
                        state=0;
                        break;
                    }
                    case 0x02:
                    {
                        state=9;
                        break;
                    }
                    case 0x04:
                    {
                        state=39;
                        break;
                    }
                    default:
                    {
                        state=0;
                        break;
                    }
                }
                break;
            }
            case 9:
            case 10:
            case 11:
            case 12:
            {
                segment<<=4;
                if(ra>0x39) ra-=7;
                segment|=(ra&0xF);
                segment&=0xFFFF;
                state++;
                break;
            }
            case 13:
            {
                segment<<=4;
                state=0;
                break;
            }

            case 39:
            case 40:
            case 41:
            case 42:
            {
                segment<<=4;
                if(ra>0x39) ra-=7;
                segment|=(ra&0xF);
                segment&=0xFFFF;
                state++;
                break;
            }
            case 43:
            {
                segment<<=16;
                state=0;
                break;
            }



            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
            {
                data<<=4;
                if(ra>0x39) ra-=7;
                data|=(ra&0xF);
                if(state==21)
                {
                    ra=(data>>24)|(data<<24);
                    ra|=(data>>8)&0x0000FF00;
                    ra|=(data<<8)&0x00FF0000;
                    data=ra;
                    PUT32(address,data);
                    if(address<min) min=address;
                    if(address>max) max=address;
                    sum+=address;
                    sum+=data;
                    address+=4;
                    state=14;
                }
                else
                {
                    state++;
                }
                break;
            }
        }
    }
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
