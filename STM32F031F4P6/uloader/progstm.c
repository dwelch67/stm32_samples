
//-----------------------------------------------------------------------------
// Copyright (C) David Welch, 2000
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ser.h"

#include "blinker.bin.h"

unsigned int seq;
unsigned int ra,rb,rc,rd;
unsigned int addr;

unsigned char sdata[512];
unsigned char udata[512];
unsigned char rdata[5000];

//-----------------------------------------------------------------------------
void xor_data ( unsigned char *sdata, unsigned int len )
{
    unsigned int ra,rc;

    rc=0;
    for(ra=0;ra<len;ra++) rc^=sdata[ra];
    sdata[ra]=rc&0xFF;
}
//-----------------------------------------------------------------------------
int detect_chip ( void )
{
    unsigned int rb;

    printf("detect_chip()\n");
    sdata[0]=0x7F;
    ser_senddata(sdata,1);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]==0x79)
            {
                ser_dump(rb);
                break;
            }
            printf("detect_chip error %u 0x%02X\n",rb,rdata[0]);
            return(1);
        }
    }
    printf("chip found\n");
    return(0);
}
//-----------------------------------------------------------------------------
int get ( void )
{
    unsigned int ra,rb,rc;

    printf("get()\n");
    sdata[0]=0x00;
    sdata[1]=sdata[0]^0xFF;
    ser_senddata(sdata,2);

    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("go error\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
            if(rb>1)
            {
                if(rb==(rdata[1]+4))
                {
                    ser_dump(rb);
                    break;
                }
            }
        }
    }
    printf("%02X %02X\n",rdata[0],rdata[1]);
    printf("%02X bootloader version\n",rdata[2]);
    for(ra=0;ra<rdata[1];ra++)
    {
        switch(rdata[ra+3])
        {
            default: printf("%02X unknown\n",rdata[ra+3]); break;
            case 0x00: printf("%02X get\n",rdata[ra+3]); break;
            case 0x01: printf("%02X get version and read protection status\n",rdata[ra+3]); break;
            case 0x02: printf("%02X get ID\n",rdata[ra+3]); break;
            case 0x11: printf("%02X Read Memory\n",rdata[ra+3]); break;
            case 0x21: printf("%02X Go\n",rdata[ra+3]); break;
            case 0x31: printf("%02X Write Memory\n",rdata[ra+3]); break;
            case 0x43: printf("%02X Erase\n",rdata[ra+3]); break;
            case 0x44: printf("%02X Extended Erase\n",rdata[ra+3]); break;
            case 0x63: printf("%02X Write Protect\n",rdata[ra+3]); break;
            case 0x73: printf("%02X Write Unprotect\n",rdata[ra+3]); break;
            case 0x82: printf("%02X Readout Protect\n",rdata[ra+3]); break;
            case 0x92: printf("%02X Readout Unprotect\n",rdata[ra+3]); break;
        }
    }
    if(rdata[rdata[1]+3]!=0x79)
    {
        printf("go ack error\n");
        return(1);
    }
    return(0);
}
//-----------------------------------------------------------------------------
int getverpstat ( void )
{
    unsigned int ra,rb,rc;

    printf("getverpstat()\n");
    sdata[0]=0x01;
    sdata[1]=sdata[0]^0xFF;
    ser_senddata(sdata,2);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("getverpstat error\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
            if(rb==5)
            {
                ser_dump(rb);
                break;
            }
        }
    }
    if((rdata[0]!=0x79)||(rdata[4]!=0x79))
    {
        printf("ack error\n");
        return(1);
    }
    printf("0x%02X version\n",rdata[1]);
    printf("0x%02X read prot disables\n",rdata[2]);
    printf("0x%02X read prot enables\n",rdata[3]);
    return(0);
}
//-----------------------------------------------------------------------------
int getid ( void )
{
    unsigned int ra,rb,rc;

    printf("getid()\n");
    sdata[0]=0x02;
    sdata[1]=sdata[0]^0xFF;
    ser_senddata(sdata,2);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rb==5)
            {
                ser_dump(rb);
                break;
            }
        }
    }
    for(ra=0;ra<5;ra++) printf("%02X ",rdata[ra]); printf("\n");

    return(0);
}
//-----------------------------------------------------------------------------
int read_mem_32 ( unsigned int add, unsigned int *data )
{
    unsigned int ra,rb,rc;

    *data=0;

    printf("read_mem_32(0x%08X)\n",add);
    sdata[0]=0x11;
    sdata[1]=sdata[0]^0xFF;
    ser_senddata(sdata,2);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("read_mem_32 error 1\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
            ser_dump(1); //rb should be a 1!
            break;
        }
    }
    sdata[0]=(add>>24)&0xFF;
    sdata[1]=(add>>16)&0xFF;
    sdata[2]=(add>> 8)&0xFF;
    sdata[3]=(add>> 0)&0xFF;

    sdata[4]=sdata[0]^sdata[1]^sdata[2]^sdata[3];

    //xor_data(sdata,4);
    ser_senddata(sdata,5);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("read_mem_32 error 2\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
            ser_dump(1); //rb should be a 1!
            break;
        }
    }
    sdata[0]=4-1;
    sdata[1]=sdata[0]^0xFF;
    ser_senddata(sdata,2);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb==5)
        {
            ser_dump(rb);
            break;
        }
    }
    if(rdata[0]!=0x79)
    {
        printf("read_mem_32 error 3\n");
        for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
        return(1);
    }
    //for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
    *data =rdata[4]; *data<<=8;
    *data|=rdata[3]; *data<<=8;
    *data|=rdata[2]; *data<<=8;
    *data|=rdata[1];
    printf("=0x%08X\n",*data);
    return(0);
}
//-----------------------------------------------------------------------------
int write_mem_32 ( unsigned int add, unsigned int data )
{
    unsigned int ra,rb,rc;

    printf("write_mem_32(0x%08X,0x%08X)\n",add,data);
    sdata[0]=0x31;
    sdata[1]=sdata[0]^0xFF;
    ser_senddata(sdata,2);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("write_mem_32 error 1\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
            ser_dump(rb); //rb should be a 1!
            break;
        }
    }
    sdata[0]=(add>>24)&0xFF;
    sdata[1]=(add>>16)&0xFF;
    sdata[2]=(add>> 8)&0xFF;
    sdata[3]=(add>> 0)&0xFF;
    xor_data(sdata,4);
    ser_senddata(sdata,5);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("write_mem_32 error 2\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
            ser_dump(rb); //rb should be a 1!
            break;
        }
    }
    sdata[0]=4-1;
    sdata[1]=(data>> 0)&0xFF;
    sdata[2]=(data>> 8)&0xFF;
    sdata[3]=(data>>16)&0xFF;
    sdata[4]=(data>>24)&0xFF;
    xor_data(sdata,5);
    ser_senddata(sdata,6);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("write_mem_32 error 3\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
            ser_dump(rb); //rb should be a 1!
            break;
        }
    }

    return(0);
}
//-----------------------------------------------------------------------------
int erase_page ( unsigned int page )
{
    unsigned int ra,rb,rc;

    printf("erase_page(%u)\n",page);
    sdata[0]=0x43;
    sdata[1]=sdata[0]^0xFF;
    ser_senddata(sdata,2);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("erase_page error 1\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
            ser_dump(rb); //rb should be a 1!
            break;
        }
    }
    sdata[0]=1; //one page
    sdata[1]=page&0xFF; //page number
    xor_data(sdata,2);
    ser_senddata(sdata,3);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("erase_page error 2\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
            ser_dump(rb); //rb should be a 1!
            break;
        }
    }
    printf("erased\n");
    return(0);
}
//-----------------------------------------------------------------------------
int erase_flash ( void )
{
    unsigned int ra,rb,rc;

    printf("erase_flash()\n");
    sdata[0]=0x43;
    sdata[1]=sdata[0]^0xFF;
    ser_senddata(sdata,2);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("erase_flash error 1\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
            ser_dump(rb); //rb should be a 1!
            break;
        }
    }
    sdata[0]=0xFF; //one page
    sdata[1]=sdata[0]^0xFF;
    ser_senddata(sdata,2);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("erase_flash error 2\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
            ser_dump(rb); //rb should be a 1!
            break;
        }
    }
    printf("erased\n");
    return(0);
}
//-----------------------------------------------------------------------------
int extended_erase_flash ( void )
{
    unsigned int ra,rb,rc;

    printf("erase_flash()\n");
    sdata[0]=0x44;
    sdata[1]=sdata[0]^0xFF;
    ser_senddata(sdata,2);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("extended_erase_flash error 1\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
            ser_dump(rb); //rb should be a 1!
            break;
        }
    }
    sdata[0]=0xFF;
    sdata[1]=0xFF;
    //xor_data(sdata,2);
    sdata[2]=0x00;
    ser_senddata(sdata,3);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("extended_erase_flash error 2\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
            ser_dump(rb); //rb should be a 1!
            break;
        }
    }
    printf("erased\n");
    return(0);
}
//-----------------------------------------------------------------------------
int go ( unsigned int add )
{
    unsigned int ra,rb,rc;

    printf("go(0x%08X)\n",add);
    sdata[0]=0x21;
    sdata[1]=sdata[0]^0xFF;
    ser_senddata(sdata,2);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("go error 1\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]); printf("\n");
            ser_dump(rb); //rb should be a 1!
            break;
        }
    }
    sdata[0]=(add>>24)&0xFF;
    sdata[1]=(add>>16)&0xFF;
    sdata[2]=(add>> 8)&0xFF;
    sdata[3]=(add>> 0)&0xFF;
    xor_data(sdata,4);
    ser_senddata(sdata,5);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("go error 1\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]); printf("\n");
            ser_dump(rb); //rb should be a 1!
            break;
        }
    }
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb)
        {
            if(rdata[0]!=0x79)
            {
                printf("go error 1\n");
                for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
                return(1);
            }
for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]); printf("\n");
            ser_dump(rb); //rb should be a 1!
            break;
        }
    }
    printf("went\n");
    return(0);
}
//-----------------------------------------------------------------------------
int read_unprotect ( void )
{
    unsigned int ra,rb;

    printf("read_unprotect()\n");
    sdata[0]=0x92;
    sdata[1]=sdata[0]^0xFF;
    ser_senddata(sdata,2);
    while(1)
    {
        rb=ser_copystring(rdata);
        if(rb==2)
        {
            ser_dump(rb);
            break;
        }
    }
    if((rdata[0]!=0x79)||(rdata[1]!=0x79))
    {
        printf("read_unprotect error\n");
        for(ra=0;ra<rb;ra++) printf("0x%02X\n",rdata[ra]);
        return(1);
    }
    return(0);
}
//-----------------------------------------------------------------------------
int get_stm_info ( void )
{
    unsigned int ra,rb,rc;

    //if(detect_chip()) return(1);
    if(get()) return(1);
    if(getverpstat()) return(1);
    if(getid()) return(1);

    return(0);
}
//-----------------------------------------------------------------------------
int do_stm_stuff ( void )
{
    unsigned int ra,rb,rc;


    //if(detect_chip()) return(1);
    //if(get()) return(1);
    //if(getverpstat()) return(1);
    //if(getid()) return(1);


    //if(erase_flash()) return(1);
    if(extended_erase_flash()) return(1);

    ////if(read_unprotect()) return(1);
    ////sleep(1);
    ////if(detect_chip()) return(1);

    //if(read_mem_32(0x08000000,&rc)) return(1);
    //if(read_mem_32(0x08000004,&rc)) return(1);
    //if(read_mem_32(0x08000008,&rc)) return(1);

    //for(ra=0;ra<bindatalen;ra++)
    //{
        //if(write_mem_32(0x08000000+(ra<<2),bindata[ra])) return(1);
    //}
    //for(ra=0;ra<10;ra++)
    //{
        //if(read_mem_32(0x08000000+(ra<<2),&rc)) return(1);
    //}

    ////sdata[0]=0x92;
    ////sdata[1]=sdata[0]^0xFF;
    //////ser_senddata(sdata,2);
    ////while(1)
    ////{
        ////rb=ser_copystring(rdata);
        ////for(ra=0;ra<rb;ra++)
        ////{
            ////printf("0x%02X ",rdata[ra]);
            ////if((rdata[ra]>=0x20)&&(rdata[ra]<127)) printf("[%c]",rdata[ra]);
            ////printf("\n");
        ////}
        ////ser_dump(rb);
    ////}




    return(0);
}
//-----------------------------------------------------------------------------
int main ( void )
{
    unsigned int ra,rb,rc,rd;

    if(ser_open())
    {
        printf("ser_open() failed\n");
        return(1);
    }
    printf("port opened\n");
    if(detect_chip())
    {
        ser_close();
        return(1);
    }


    get_stm_info();
    do_stm_stuff();


    ser_close();
    return(0);
}
//-----------------------------------------------------------------------------
// Copyright (C) David Welch, 2000, 2003, 2008, 2009
//-----------------------------------------------------------------------------

