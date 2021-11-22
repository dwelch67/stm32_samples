



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


//static void i2c_delay ( void )
//{
    ////unsigned int ra;
    ////for(ra=0;ra<2;ra++) dummy(ra);
    ////if(uart2_check()) uart2_recv();
//}

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
}

static void i2c_start ( void )
{
    //i2c_delay();
    //i2c_delay();
    sda_low();
    //i2c_delay();
    //i2c_delay();
    scl_low();
    //i2c_delay();
}

static void i2c_stop ( void )
{
    //i2c_delay();
    scl_high();
    //i2c_delay();
    sda_high();
    //i2c_delay();
    //i2c_delay();
}

static unsigned int i2c_write_byte ( unsigned int b )
{
    unsigned int ra;
    for(ra=0x80;ra;ra>>=1)
    {
        //i2c_delay();
        if(ra&b) sda_high();
        else     sda_low();
        //i2c_delay();
        scl_high();
        //i2c_delay();
        scl_low();
        //i2c_delay();
        sda_low();
        //i2c_delay();
    }
    //i2c_delay();
    //sda_input();
    //i2c_delay();
    scl_high();
    //i2c_delay();
    //ra=sda_read();
    //i2c_delay();
    scl_low();
    //i2c_delay();
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



//  aaaa    bbbb     cccc     dddd    
// I    j  J    k i K    l   L    m   
// I    j  J    k   K    l   L    m   
//  EEee    FFff     GGgg     HHhh    
// A    o  B    p i C    q   D    r   
// A    o  B    p   C    q   D    r   
//  ssss    tttt     uuuu n   vvvv    
 
 
// a  00000000
// 0  hgfedcba 
// 1  .......i 
// 2  ....mlkj upper bits are top left angle
// 3  .......n 
// 4  ....rqpo upper bits middle verticle
// 5  ........
// 6  ....vuts upper bits slash
// 7  ........
// 8  ....DCBA upper bits slash
// 9  ........
//10  ....LKJI
//11  ........
//12  ....HGFE upper bits slash

//  aaaa  
// f    b 
// f    b 
//  gghh  
// e    c 
// e    c 
//  dddd  
 
// a  00000000
// 0  hhhhaaaa 
// 1  .......C colon 
// 2  ....bbbb 
// 3  .......D decimal
// 4  ....cccc 
// 5  ........
// 6  ....dddd 
// 7  ........
// 8  ....eeee 
// 9  ........
//10  ....ffff
//11  ........
//12  ....gggg 
 
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;


	unsigned char data[32];

    //i2c outputs
    i2c_pin_init();

    scl_high();
    sda_high();

	send_command(0x21,0x00); //enable oscillator
	send_command(0xE0,0x00); //full brightness (reset value)
	send_command(0x80,0x00); //blinking off (reset value)
	send_command(0x81,0x00); //blinking off, display on
	for(rb=0;rb<10;rb++) data[rb++]=0;
	//send_data(data,rb); 
	
	
	rb=0;
	data[rb++]=0x00; //add
	data[rb++]=0xDD; //0
	data[rb++]=0x01; //1
	data[rb++]=0x0D; //2
	data[rb++]=0x00; //3
	data[rb++]=0x0D; //4
	data[rb++]=0x00; //5
	data[rb++]=0x0D; //6
	data[rb++]=0x00; //7
	data[rb++]=0x0D; //8
	data[rb++]=0x00; //9
	data[rb++]=0x0D; //10
	data[rb++]=0x00; //11
	data[rb++]=0x0D; //12
	send_data(data,rb);
	













    return(0);
}


