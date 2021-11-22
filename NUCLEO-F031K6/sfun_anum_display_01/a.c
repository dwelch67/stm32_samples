

#include <stdio.h>

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

static void send_data ( unsigned char *data, unsigned int len )
{
	unsigned int ra;
	
	for(ra=0;ra<len;ra++)
	{
		printf("%2u 0x%02X\n",ra,data[ra]);
	}
}


static void show_number ( unsigned int a, unsigned int b, unsigned int c, unsigned int d )
{
	unsigned char data[16];
	unsigned int ra;
	unsigned int rb;
	
	for(ra=0;ra<14;ra++) data[ra]=0;
	
	for(rb=13,ra=0x40;ra;ra>>=1,rb-=2)
	{
		if(numbits[a]&ra) data[rb]|=(1<<0);
		//if(numbits[b]&ra) data[rb]|=(ra<<1);
		//if(numbits[c]&ra) data[rb]|=(ra<<2);
		//if(numbits[d]&ra) data[rb]|=(ra<<3);
	}
	//data[1]|=data[13]<<4;
	send_data(data,14);
}
 
 
int main ( void )
{
	show_number(8,8,8,8);
	
	
	return(0);
} 
 
