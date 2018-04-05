

#include <stdio.h>


    ////moder
    //ra=GET32(GPIOABASE+0x00);
    //ra&=~(3<<(4<<1));
    //ra|= (1<<(4<<1));
    //PUT32(GPIOABASE+0x00,ra);
    ////OTYPER
    //ra=GET32(GPIOABASE+0x04);
    //ra&=~(1<<4); //PA4
    //PUT32(GPIOABASE+0x04,ra);

unsigned int stuff[60][4];
unsigned int hl[60][2];

void figure ( unsigned int h, unsigned int l, unsigned int second )
{
    unsigned int amoder;
    unsigned int bmoder;
    unsigned int absrr;
    unsigned int bbsrr;
    
    //printf("//%u %u\n",h,l);

    amoder=0;
    bmoder=0;
    absrr=0;
    bbsrr=0;

    if(h<8)
    {
        amoder|=(1<<(h<<1));
        absrr|=(1<<(h+ 0));
    }
    else
    {
        bmoder|=(1<<(1<<1));
        bbsrr|=(1<<(1+ 0));
    }
    if(l<8)
    {
        amoder|=(1<<(l<<1));
        absrr|=(1<<(l+16));
    }
    else
    {
        bmoder|=(1<<(1<<1));
        bbsrr|=(1<<(1+16));
    }

    stuff[second][0]=amoder;
    stuff[second][1]=bmoder;
    stuff[second][2]=absrr;
    stuff[second][3]=bbsrr;
    hl[second][0]=h;
    hl[second][1]=l;

    //printf("{0x%08X,0x%08X,0x%08X,0x%08X}, //%u %u %02u\n",amoder,bmoder,absrr,bbsrr,h,l,second);
   
}


int main ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;

    ra=0;
    for(rb=0;rb<9;rb++)
    for(rc=0;rc<9;rc++)
    {
        if(ra<60)
        {
            if(rb==rc) continue;
            //printf("%2u: ",ra);
            rd=(60-ra)%60;
            figure(rb,rc,rd);
            ra++;
        }
    }

    for(ra=0;ra<60;ra++)
    {
        printf("{");
        for(rb=0;rb<4;rb++)
        {
            printf("0x%08X,",stuff[ra][rb]);
        }
        printf("}, // %02u %u %u\n",ra,hl[ra][0],hl[ra][1]);
    }

    
}    
