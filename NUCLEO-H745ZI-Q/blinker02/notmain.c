
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );
#define RCCBASE 0x58024400
#define RCC_APB4ENR (RCCBASE+0F4)
#define RCC_C1_APB4ENR (RCCBASE+154)
#define RCC_C2_APB4ENR (RCCBASE+1B4)
#define SYSCFG_BASE 0x58000400
#define RCC_AHB4ENR (RCCBASE+0x0E0)
#define CPUID 0xE000ED00

#define GPIOB_BASE      0x58020400
#define GPIOB_MODER     (GPIOB_BASE+0x00)
#define GPIOB_OTYPER    (GPIOB_BASE+0x04)
#define GPIOB_BSRR      (GPIOB_BASE+0x18)

#define STK_CSR 0xE000E010
#define STK_RVR 0xE000E014
#define STK_CVR 0xE000E018

//PB14

void do_delay ( unsigned int sec )
{
    unsigned int ra,rd;

    for(rd=0;rd<sec;)
    {
        ra=GET32(STK_CSR);
        if(ra&(1<<16))
        {
            rd++;
        }
    }
}

int notmain ( void )
{
    unsigned int ra;

    ra=GET32(CPUID)&0xFFF0;
    //comment one of these
    //if(ra==0xC270) return(0); //cortex-m7
    if(ra==0xC240) return(0); //cortex-m4

    ra=GET32(RCC_AHB4ENR);
    ra|=1<<1;
    PUT32(RCC_AHB4ENR,ra);

    ra=GET32(GPIOB_MODER);
    ra&=(~(3<<(14<<1)));
    ra|=  (1<<(14<<1)) ;
    PUT32(GPIOB_MODER,ra);

    ra=GET32(GPIOB_OTYPER);
    ra&=(~(1<<14));
    PUT32(GPIOB_OTYPER,ra);

    //PUT32(STK_CSR,0x00000004);
    //PUT32(STK_RVR,0xFFFFFFFF);
    //PUT32(STK_CSR,0x00000005);

    //while(1)
    //{
        //PUT32(GPIOB_BSRR,1 << (14+ 0));
        //while(1) if(GET32(STK_CVR)&0x800000) break;
        //PUT32(GPIOB_BSRR,1 << (14+16));
        //while(1) if((GET32(STK_CVR)&0x800000)==0) break;
    //}

    PUT32(STK_CSR,0x00000004);
    PUT32(STK_RVR,16000000-1);
    PUT32(STK_CSR,0x00000005);

    while(1)
    {
        PUT32(GPIOB_BSRR,1 << (14+ 0));
        do_delay(40);
        PUT32(GPIOB_BSRR,1 << (14+16));
        do_delay(1);
    }

    return(0);
}
