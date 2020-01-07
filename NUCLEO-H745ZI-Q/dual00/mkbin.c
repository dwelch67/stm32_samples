
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char data[0x02000000];



int main ( void )
{
    unsigned int rb;
    FILE *fp;

    memset(data,0xFF,sizeof(data));

    fp=fopen("notmain7.bin","rb");
    if(fp==NULL)
    {
        printf("Error opening file notmain7.bin\n");
        return(1);
    }
    rb=fread(&data[0x00000000],1,0x01000000,fp);
    fclose(fp);
    printf("%u bytes read\n",rb);
    fp=fopen("notmain4.bin","rb");
    if(fp==NULL)
    {
        printf("Error opening file notmain7.bin\n");
        return(1);
    }
    rb=fread(&data[0x01000000],1,0x01000000,fp);
    fclose(fp);
    printf("%u bytes read\n",rb);


    fp=fopen("notmain.bin","wb");
    if(fp==NULL) return(1);
    fwrite(data,1,sizeof(data),fp);
    fclose(fp);

    printf("Done\n");
    return(0);
}
