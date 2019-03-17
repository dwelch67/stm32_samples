
#include <stdio.h>

char raw[10][8]=
{
{"ABCDEFg"}, //0
{"aBCdefg"}, //1
{"ABcDEfG"}, //2
{"ABCDefG"}, //3
{"aBCdeFG"}, //4
{"AbCDeFG"}, //5
{"AbCDEFG"}, //6
{"ABCdefg"}, //7
{"ABCDEFG"}, //8
{"ABCDeFG"}, //9
};

int main ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int seven[10];

    for(ra=0;ra<10;ra++)
    {
        rc=1<<(3+16);
        for(rb=0;raw[ra][rb];rb++)
        {
            switch(raw[ra][rb])
            {
                case 'a': rc|=1<<(16+5); break;
                case 'b': rc|=1<<(16+6); break;
                case 'c': rc|=1<<(16+0); break;
                case 'd': rc|=1<<(16+1); break;
                case 'e': rc|=1<<(16+2); break;
                case 'f': rc|=1<<(16+4); break;
                case 'g': rc|=1<<(16+7); break;
                case 'A': rc|=1<<( 0+5); break;
                case 'B': rc|=1<<( 0+6); break;
                case 'C': rc|=1<<( 0+0); break;
                case 'D': rc|=1<<( 0+1); break;
                case 'E': rc|=1<<( 0+2); break;
                case 'F': rc|=1<<( 0+4); break;
                case 'G': rc|=1<<( 0+7); break;
            }
        }
        seven[ra]=rc;
    }


    printf("const unsigned int seven_up[10]=\n");
    printf("{\n");
    for(ra=0;ra<10;ra++)
    {
        printf("0x%08X,\n",seven[ra]);
    }
    printf("};\n");

    printf("const unsigned int seven_dn[10]=\n");
    printf("{\n");
    for(ra=0;ra<10;ra++)
    {
        printf("0x%08X,\n",seven[ra]^0x00FF00FF);
    }
    printf("};\n");

    
    return(0);
}


