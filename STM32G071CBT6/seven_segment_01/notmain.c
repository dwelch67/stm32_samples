
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

void ienable ( void );
void DOWFI ( void );

#define GPIOBBASE 0x50000400
#define GPIOCBASE 0x50000800
#define RCCBASE 0x40021000

#define STK_CSR  0xE000E010
#define STK_RVR  0xE000E014
#define STK_CVR  0xE000E018
#define STK_MASK 0x00FFFFFF

static const unsigned int seven[16][4]=
{
{0x080307FC,0x04030BFC,0x02030DFC,0x01030EFC,},
{0x089F0760,0x049F0B60,0x029F0D60,0x019F0E60,},
{0x082507DA,0x04250BDA,0x02250DDA,0x01250EDA,},
{0x080D07F2,0x040D0BF2,0x020D0DF2,0x010D0EF2,},
{0x08990766,0x04990B66,0x02990D66,0x01990E66,},
{0x084907B6,0x04490BB6,0x02490DB6,0x01490EB6,},
{0x084107BE,0x04410BBE,0x02410DBE,0x01410EBE,},
{0x081F07E0,0x041F0BE0,0x021F0DE0,0x011F0EE0,},
{0x080107FE,0x04010BFE,0x02010DFE,0x01010EFE,},
{0x081907E6,0x04190BE6,0x02190DE6,0x01190EE6,},
{0x08FF0700,0x04FF0B00,0x02FF0D00,0x01FF0E00,},
{0x00FF0F00,0x00FF0F00,0x00FF0F00,0x00FF0F00,},
{0x00FF0F00,0x00FF0F00,0x00FF0F00,0x00FF0F00,},
{0x00FF0F00,0x00FF0F00,0x00FF0F00,0x00FF0F00,},
{0x00FF0F00,0x00FF0F00,0x00FF0F00,0x00FF0F00,},
{0x00FF0F00,0x00FF0F00,0x00FF0F00,0x00FF0F00,},
};

//-------------------------------------------------------------------
//  **** WARNING THE CODE BELOW IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------
volatile unsigned int counter;
volatile unsigned int num[4];
void systick_handler ( void )
{
    GET32(STK_CSR);
    switch(counter&3)
    {
        case 0: PUT32(GPIOBBASE+0x18,seven[num[0]][0]); break;
        case 1: PUT32(GPIOBBASE+0x18,seven[num[1]][1]); break;
        case 2: PUT32(GPIOBBASE+0x18,seven[num[2]][2]); break;
        case 3: PUT32(GPIOBBASE+0x18,seven[num[3]][3]); break;
    }
    counter++;
}
//-------------------------------------------------------------------
//  **** WARNING THE CODE ABOVE IS PART OF AN INTERRUPT HANDLER ****
//-------------------------------------------------------------------


int notmain ( void )
{
    unsigned int ra;
    //unsigned int rx;

    ra=GET32(RCCBASE+0x34);
    ra|=1<<1; //enable port b
    PUT32(RCCBASE+0x34,ra);

    //moder
    ra=GET32(GPIOBBASE+0x00);
    ra&=~(3<<( 0<<1)); //PC0
    ra&=~(3<<( 1<<1)); //PC1
    ra&=~(3<<( 2<<1)); //PC2
    ra&=~(3<<( 3<<1)); //PC3
    ra&=~(3<<( 4<<1)); //PC4
    ra&=~(3<<( 5<<1)); //PC5
    ra&=~(3<<( 6<<1)); //PC6
    ra&=~(3<<( 7<<1)); //PC7
    ra&=~(3<<( 8<<1)); //PC8
    ra&=~(3<<( 9<<1)); //PC9
    ra&=~(3<<(10<<1)); //PC10
    ra&=~(3<<(11<<1)); //PC11
    ra|= (1<<( 0<<1)); //PC0
    ra|= (1<<( 1<<1)); //PC1
    ra|= (1<<( 2<<1)); //PC2
    ra|= (1<<( 3<<1)); //PC3
    ra|= (1<<( 4<<1)); //PC4
    ra|= (1<<( 5<<1)); //PC5
    ra|= (1<<( 6<<1)); //PC6
    ra|= (1<<( 7<<1)); //PC7
    ra|= (1<<( 8<<1)); //PC8
    ra|= (1<<( 9<<1)); //PC9
    ra|= (1<<(10<<1)); //PC10
    ra|= (1<<(11<<1)); //PC11
    PUT32(GPIOBBASE+0x00,ra);


    PUT32(GPIOBBASE+0x18,0x00FF0F00);


    counter=0;
    num[0]=1;
    num[1]=2;
    num[2]=3;
    num[3]=4;


    PUT32(STK_CSR,4);
    PUT32(STK_RVR,0x10000-1);
    PUT32(STK_CVR,0x10000-1);
    PUT32(STK_CSR,7);
    ienable();

    //DOWFI();
    while(1)
    {
        num[3]++;
        if(num[3]>9)
        {
            num[3]=0;
            num[2]++;
            if(num[2]>9)
            {
                num[2]=0;
                num[1]++;
                if(num[1]>9)
                {
                    num[1]=0;
                    num[0]++;
                    if(num[0]>9)
                    {
                        num[0]=0;
                    }
                }
            }
        }
        for(ra=0;ra<20000;ra++) dummy(ra);
    }



    return(0);
}
