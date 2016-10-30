
#include <stdio.h>

//60 = (2*3)*(2*5) = 4*15
//timer set to 30 seconds per tick so 120 ticks per minute
//so divide by 8*15



int main ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int den;
    unsigned int acc;
    unsigned int num;
    unsigned int res;

    for(ra=0;ra<10000;ra++)
    {
        //num=ra>>4; //divide by 16
        num=ra;
        res=0;
        acc=0;
        den=1440;
        for(rb=0x80000000;rb;rb>>=1)
        {
            acc<<=1;
            if(rb&num) acc|=1;
            if(acc>=den)
            {
                acc-=den;
                res|=rb;
            }
        }
        printf("%u %u %u ",ra,res,acc);
        num=acc;
        res=0;
        acc=0;
        den=120;
        for(rb=0x80000000;rb;rb>>=1)
        {
            acc<<=1;
            if(rb&num) acc|=1;
            if(acc>=den)
            {
                acc-=den;
                res|=rb;
            }
        }
        printf("%u %u %u\n",res,acc,acc>>1);


        
    }
    


    
}


