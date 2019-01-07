

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

unsigned int stuff[60][2];
unsigned int hl[60][2];

void figure ( unsigned int h, unsigned int l, unsigned int second )
{
    unsigned int amoder;
    unsigned int absrr;

    amoder=0x28280000;
    //amoder|=2<<18; //NO UART TX!!!
    //amoder|=2<<20; //spinner, no uart.

    absrr=0;

//    if(h<8)
    {
        amoder|=(1<<(h<<1));
        absrr|=(1<<(h+ 0));
    }
    //else
    //{
        //amoder|=(1<<(9<<1));
        //absrr|=(1<<(9+ 0));
    //}
    //if(l<8)
    {
        amoder|=(1<<(l<<1));
        absrr|=(1<<(l+16));
    }
    //else
    //{
        //amoder|=(1<<(9<<1));
        //absrr|=(1<<(9+16));
    //}

    stuff[second][0]=amoder;
    stuff[second][1]=absrr;
    hl[second][0]=h;
    hl[second][1]=l;
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

            rd+=15;
            if(rd>59) rd-=60;


            figure(rb,rc,rd);
            ra++;
        }
    }

    printf("const unsigned int twostuff[60][2]=\n");
    printf("{\n");

    for(ra=0;ra<60;ra++)
    {
        printf("{");
        for(rb=0;rb<2;rb++)
        {
            printf("0x%08X,",stuff[ra][rb]);
        }
        printf("}, // %02u %u %u\n",ra,hl[ra][0],hl[ra][1]);
    }
    printf("};\n");


}
