
#include <stdio.h>


#define LED00OUT  (0x1)
#define LED01OUT  (0x1)
#define LED02OUT  (0x1)
#define LED00MODER ((1<<(0<<1))|(1<<(1<<1)))
#define LED01MODER ((1<<(0<<1))|(1<<(2<<1)))
#define LED02MODER ((1<<(0<<1))|(1<<(3<<1)))

#define LED10OUT  (0x2)
#define LED11OUT  (0x2)
#define LED12OUT  (0x2)
#define LED10MODER ((1<<(1<<1))|(1<<(0<<1)))
#define LED11MODER ((1<<(1<<1))|(1<<(2<<1)))
#define LED12MODER ((1<<(1<<1))|(1<<(3<<1)))

#define LED20OUT  (0x4)
#define LED21OUT  (0x4)
#define LED22OUT  (0x4)
#define LED20MODER ((1<<(2<<1))|(1<<(0<<1)))
#define LED21MODER ((1<<(2<<1))|(1<<(1<<1)))
#define LED22MODER ((1<<(2<<1))|(1<<(3<<1)))

#define LED30OUT  (0x8)
#define LED31OUT  (0x8)
#define LED32OUT  (0x8)
#define LED30MODER ((1<<(3<<1))|(1<<(0<<1)))
#define LED31MODER ((1<<(3<<1))|(1<<(1<<1)))
#define LED32MODER ((1<<(3<<1))|(1<<(2<<1)))

//[y][x]
static const unsigned char ledout[4][4]=
{
    {LED00OUT,LED01OUT,LED02OUT,0},
    {LED10OUT,LED11OUT,LED12OUT,0},
    {LED20OUT,LED21OUT,LED22OUT,0},
    {LED30OUT,LED31OUT,LED32OUT,0},
};
static const unsigned char ledmoder[4][4]=
{
    {LED00MODER,LED01MODER,LED02MODER,0},
    {LED10MODER,LED11MODER,LED12MODER,0},
    {LED20MODER,LED21MODER,LED22MODER,0},
    {LED30MODER,LED31MODER,LED32MODER,0},
};

static const unsigned short figure_eight[16]=
{
//000.
//....
//....
0xE00,
//.000
//....
//....
0x700,
//..00
//...0
//....
0x310,
//...0
//..00
//....
0x130,
//....
//.000
//....
0x070,
//....
//000.
//....
0x0E0,
//....
//00..
//0...
0x0C8,
//....
//0...
//00..
0x08C,
//....
//....
//000.
0x00E,
//....
//....
//.000
0x007,
//....
//...0
//..00
0x013,
//....
//..00
//...0
0x031,
//....
//.000
//....
0x070,
//....
//000.
//....
0x0E0,
//0...
//00..
//....
0x8C0,
//00..
//0...
//....
0xC80
};

static const unsigned short round_about[10]=
{
//000.
//....
//....
0xE00,
//.000
//....
//....
0x700,
//..00
//...0
//....
0x310,
//...0
//...0
//...0
0x111,
//....
//...0
//..00
0x013,
//....
//....
//.000
0x007,
//....
//....
//000.
0x00E,
//....
//0...
//00..
0x08C,
//0...
//0...
//0...
0x888,
//00..
//0...
//....
0xC80,
};


unsigned char one[16*3][2];



int main ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
    unsigned int rx;
    unsigned int ry;
    unsigned int rz;



    rz=0;
    for(rb=0;rb<16;rb++)
    {
        rc=figure_eight[rb];
        //for(rz=0;rz<THREELEDS;rz++)
        {
            ry=3;
            rx=2;
            for(rd=0x800;rd;rd>>=1)
            {
                if(rd&rc)
                {
                    one[rz][0]=ledout[ry][rx];
                    one[rz][1]=ledmoder[ry][rx];
                    rz++;
                }
                if((ry--)==0)
                {
                    ry=3;
                    if((rx--)==0) rx=2;
                }
            }
        }
    }
    printf("\n");
    printf("static const unsigned int eight[32]=\n");
    printf("{\n");
    for(rz=0;rz<(16*3);rz+=3)
    {
        printf("0x");
        for(ra=0;ra<3;ra++) printf("%02X",one[rz+ra][0]);
        printf(",");
        printf("0x");
        for(ra=0;ra<3;ra++) printf("%02X",one[rz+ra][1]);
        printf(",\n");
    }
    printf("};\n");


    rz=0;
    for(rb=0;rb<10;rb++)
    {
        rc=round_about[rb];
        //for(rz=0;rz<THREELEDS;rz++)
        {
            ry=3;
            rx=2;
            for(rd=0x800;rd;rd>>=1)
            {
                if(rd&rc)
                {
                    one[rz][0]=ledout[ry][rx];
                    one[rz][1]=ledmoder[ry][rx];
                    rz++;
                }
                if((ry--)==0)
                {
                    ry=3;
                    if((rx--)==0) rx=2;
                }
            }
        }
    }
    printf("\n");
    printf("static const unsigned int circle[20]=\n");
    printf("{\n");
    for(rz=0;rz<(10*3);rz+=3)
    {
        printf("0x");
        for(ra=0;ra<3;ra++) printf("%02X",one[rz+ra][0]);
        printf(",");
        printf("0x");
        for(ra=0;ra<3;ra++) printf("%02X",one[rz+ra][1]);
        printf(",\n");
    }
    printf("};\n");

    printf("\n");

    return(0);
}

