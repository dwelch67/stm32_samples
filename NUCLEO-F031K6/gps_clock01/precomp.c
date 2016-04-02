
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define LED00OUT  (0x1)
#define LED01OUT  (0x1)
#define LED02OUT  (0x1)
#define LED00MODER ((1<<(0<<1))|(1<<(1<<1)))
#define LED01MODER ((1<<(0<<1))|(1<<(3<<1)))
#define LED02MODER ((1<<(0<<1))|(1<<(4<<1)))

#define LED10OUT  (0x2)
#define LED11OUT  (0x2)
#define LED12OUT  (0x2)
#define LED10MODER ((1<<(1<<1))|(1<<(0<<1)))
#define LED11MODER ((1<<(1<<1))|(1<<(3<<1)))
#define LED12MODER ((1<<(1<<1))|(1<<(4<<1)))

#define LED20OUT  (0x8)
#define LED21OUT  (0x8)
#define LED22OUT  (0x8)
#define LED20MODER ((1<<(3<<1))|(1<<(0<<1)))
#define LED21MODER ((1<<(3<<1))|(1<<(1<<1)))
#define LED22MODER ((1<<(3<<1))|(1<<(4<<1)))

#define LED30OUT  (0x10)
#define LED31OUT  (0x10)
#define LED32OUT  (0x10)
#define LED30MODER ((1<<(4<<1))|(1<<(0<<1)))
#define LED31MODER ((1<<(4<<1))|(1<<(1<<1)))
#define LED32MODER ((1<<(4<<1))|(1<<(3<<1)))

//[y][x]
static const unsigned int ledout[4][4]=
{
    {LED00OUT,LED01OUT,LED02OUT,0},
    {LED10OUT,LED11OUT,LED12OUT,0},
    {LED20OUT,LED21OUT,LED22OUT,0},
    {LED30OUT,LED31OUT,LED32OUT,0},
};
static const unsigned int ledmoder[4][4]=
{
    {LED00MODER,LED01MODER,LED02MODER,0},
    {LED10MODER,LED11MODER,LED12MODER,0},
    {LED20MODER,LED21MODER,LED22MODER,0},
    {LED30MODER,LED31MODER,LED32MODER,0},
};

unsigned int one[256][2];

int main ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
    unsigned int rx;
    unsigned int ry;
    unsigned int rz;
    unsigned int r;

    for(r=0;r<3;r++)
    {
        rz=0;
        memset(one,0,sizeof(one));
        for(rb=0;rb<16;rb++)
        {
            rc=rb;
            //fprintf(stderr,"0x%03X\n",rc);
            //for(rz=0;rz<THREELEDS;rz++)
            {
                ry=3;
                rx=2;
                rx=2-r;
                for(rd=0x8;rd;rd>>=1)
                {
                    if(rd&rc)
                    {
                        one[rz][0]=ledout[ry][rx];
                        one[rz][1]=ledmoder[ry][rx];
                    }
                    if((ry--)==0)
                    {
                        ry=3;
                        if((rx--)==0) rx=2;
                    }
                    rz++;
                }
            }
        }
        fprintf(stderr,"rz %u\n",rz);
        printf("\n");
        printf("static const unsigned int digits%u[16][4][2]=\n",r);
        printf("{\n");
        for(rz=0;rz<64;rz++)
        {
            if((rz&0x3)==0x0) printf("{\n");
            printf("{0x%02X,0x%03X},\n",one[rz][0],one[rz][1]);
            if((rz&0x3)==0x3) printf("},\n");
        }
        printf("};\n");
    }
    printf("\n");

    return(0);
}

