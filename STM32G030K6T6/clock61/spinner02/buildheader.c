

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

void figure ( unsigned int hh, unsigned int ll, unsigned int second )
{
    unsigned int amoder;
    unsigned int absrr;

    unsigned int h,l;

    amoder=0x28000000;
    //amoder|=2<<18; //NO UART TX!!!
    //amoder|=2<<20; //spinner, no uart.

    absrr=0;



    switch(hh)
    {
        case 4: h=0; break;
        case 5: h=1; break;
        case 3: h=2; break;
        case 2: h=3; break;
        case 8: h=4; break;
        case 1: h=5; break;
        case 0: h=6; break;
        case 6: h=7; break;
        case 7: h=8; break;
    }
    switch(ll)
    {
        case 4: l=0; break;
        case 5: l=1; break;
        case 3: l=2; break;
        case 2: l=3; break;
        case 8: l=4; break;
        case 1: l=5; break;
        case 0: l=6; break;
        case 6: l=7; break;
        case 7: l=8; break;
    }

    //if(h<8)
    //{
        amoder|=(1<<(h<<1));
        absrr|=(1<<(h+ 0));
    //}
    //else
    //{
        //amoder|=(1<<(9<<1));
        //absrr|=(1<<(9+ 0));
    //}
    //if(l<8)
    //{
        amoder|=(1<<(l<<1));
        absrr|=(1<<(l+16));
    //}
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
