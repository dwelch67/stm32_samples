
#include <stdio.h>


int main ( void )
{
    unsigned int rm;
    unsigned int rs;
    unsigned int ra;
    unsigned int rb;

    for(rm=0;rm<60;rm++)
    {
        for(rs=0;rs<60;rs++)
        {
            ra=rm<<1;
            if(rs>29) ra++;
            rb=ra/5;
            //printf("%02u:%02u %u\n",rm,rs,rb);
        }
    }

    for(rm=0;rm<120;rm++)
    {
        rb=rm/5;
        printf("%2u,//%3u\n",rb,rm);
    }


    return(0);
}

