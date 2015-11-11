
//-----------------------------------------------------------------------------
// Copyright (C) David Welch, 2000
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ser.h"

unsigned int seq;
unsigned int ra,rb,rc,rd;
unsigned int addr;

unsigned char sdata[512];
unsigned char udata[512];
unsigned char rdata[5000];

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
    if(extended_erase_flash()) return(1);
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

