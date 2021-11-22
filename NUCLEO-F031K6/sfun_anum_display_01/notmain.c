

void ASMDELAY ( unsigned int );

//#define DEBUG_UART
#define PADDR 0xE0


void PUT16 ( unsigned int, unsigned int );
void PUT32 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOABASE 0x48000000
#define GPIOA_MODER (GPIOABASE+0x00)
#define GPIOBBASE 0x48000400

#define RCCBASE 0x40021000

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018
#define STK_MASK 0x00FFFFFF;

#define TIM2_BASE 0x40000000
#define TIM2_CR1 (TIM2_BASE+0x00)
#define TIM2_CR2 (TIM2_BASE+0x04)
#define TIM2_SR  (TIM2_BASE+0x10)
#define TIM2_CNT (TIM2_BASE+0x24)
#define TIM2_PSC (TIM2_BASE+0x28)
#define TIM2_ARR (TIM2_BASE+0x2C)
#define TIM2_MASK 0xFFFFFFFF

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

static void show_number ( unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int colon, unsigned int dot )
{
	unsigned char data[16];
	unsigned int ra;
	unsigned int rb;
	
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
 
int notmain ( void )
{
	unsigned int nums[4];
	
	
    i2c_pin_init();

	send_command(0x21,0x00); //enable oscillator
	send_command(0xE0,0x00); //full brightness (reset value)
	send_command(0x80,0x00); //blinking off (reset value)
	send_command(0x81,0x00); //blinking off, display on


	nums[0]=0;
	nums[1]=1;
	nums[2]=2;
	nums[3]=3;
	while(1)
	{
		show_number(nums[0],nums[1],nums[2],nums[3],nums[2]&1,(nums[2]&2)>>1);
		nums[3]++; 
		if(nums[3]>9) { nums[3]=0; nums[2]++; }
		if(nums[2]>9) { nums[2]=0; nums[1]++; }
		if(nums[1]>9) { nums[1]=0; nums[0]++; }
		if(nums[0]>9) { nums[0]=0; }
		
		
		ASMDELAY(0x10000);
	}

    return(0);
}


