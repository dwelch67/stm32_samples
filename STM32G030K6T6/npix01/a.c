

#include <stdio.h>

unsigned short data[32];
unsigned int off;
unsigned int bit;

void add_bit ( unsigned int x )
{
    data[off]<<=1;
    data[off]|=(x&1);
    bit++;
    if(bit==16)
    {
        off++;
        bit=0;
    }
}
void add_one_color ( unsigned int c )
{
    unsigned int ra;
    for(ra=0x80;ra;ra>>=1)
    {
        if(c&ra)
        {
            add_bit(1);
            add_bit(1);
            add_bit(0);
        }
        else
        {
            add_bit(0);
            add_bit(0);
            add_bit(1);
        }
    }
}
void add_colors ( unsigned int g, unsigned int r, unsigned int b )
{
    add_one_color(g);
    add_one_color(r);
    add_one_color(b);
}
int main ( void )
{
    unsigned int ra;
    off=0;
    bit=0;

    add_colors(0x00,0x00,0x20);
//    add_colors(0x00,0x00,0x00);
  //  add_colors(0x00,0x00,0x20);
    while(bit!=0) add_bit(0);

    printf("off %u\n",off);
    printf("const unsigned short data[%u]=\n",off+1);
    printf("{\n");
    for(ra=0;ra<off;ra++) printf("0x%04X,\n",data[ra]);
    printf("0,\n");
    printf("};\n");
    return(0);

}


