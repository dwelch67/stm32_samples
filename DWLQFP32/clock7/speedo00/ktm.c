

#include <stdio.h>

int main ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
    unsigned int a;
    unsigned int b;
    unsigned int c;
    double d;

    for(ra=0;ra<256;ra++)
    {
        d=ra;
        d=d*0.6213711922;
        d+=0.5;
        rd=(unsigned int)d;
        rc=rd;
        a=rc/100;
        rc-=a*100;
        b=rc/10;
        rc-=b*10;
        c=rc;
        rc=(a<<8)|(b<<4)|c;
        //printf("%u %u %u%u%u %x\n",ra,rd,a,b,c,rc);
        printf("0x%03X, //%u\n",rc,ra);
    }
    return(0);
}


