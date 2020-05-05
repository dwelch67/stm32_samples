
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOA_BASE      0x50000000
#define GPIOA_MODER     (GPIOA_BASE+0x00)
#define GPIOA_OTYPER    (GPIOA_BASE+0x04)
#define GPIOA_BSRR      (GPIOA_BASE+0x18)

#define RCC_BASE        0x40021000
#define RCC_IOPENR      (RCC_BASE+0x2C)

#define LEDG  0
#define LEDA  1
#define LEDC  2
#define LEDF  3
#define LEDX  4
#define VCC3  5
#define LEDP  6
#define VCC2  7
#define LEDE  8
#define LEDL 11
#define LEDD 12
#define VCC1 13
#define VCC0 14
#define LEDB 15

int notmain ( void )
{
    unsigned int ra;
    unsigned int rx;

    ra=GET32(RCC_IOPENR);
    ra|=1<<0; //GPIOA
    PUT32(RCC_IOPENR,ra);

    ra=GET32(GPIOA_MODER);
    ra&=~(3<<(LEDA<<1));
    ra&=~(3<<(LEDB<<1));
    ra&=~(3<<(LEDC<<1));
    ra&=~(3<<(LEDD<<1));
    ra&=~(3<<(LEDE<<1));
    ra&=~(3<<(LEDF<<1));
    ra&=~(3<<(LEDG<<1));
    ra&=~(3<<(LEDP<<1));
    ra&=~(3<<(LEDL<<1));
    ra&=~(3<<(LEDX<<1));
    ra&=~(3<<(VCC0<<1));
    ra&=~(3<<(VCC1<<1));
    ra&=~(3<<(VCC2<<1));
    ra&=~(3<<(VCC3<<1));
    ra|= (1<<(LEDA<<1));
    ra|= (1<<(LEDB<<1));
    ra|= (1<<(LEDC<<1));
    ra|= (1<<(LEDD<<1));
    ra|= (1<<(LEDE<<1));
    ra|= (1<<(LEDF<<1));
    ra|= (1<<(LEDG<<1));
    ra|= (1<<(LEDP<<1));
    ra|= (1<<(LEDL<<1));
    ra|= (1<<(LEDX<<1));
    ra|= (1<<(VCC0<<1));
    ra|= (1<<(VCC1<<1));
    ra|= (1<<(VCC2<<1));
    ra|= (1<<(VCC3<<1));
    PUT32(GPIOA_MODER,ra);

    PUT32(GPIOA_BSRR,
        (1<<(VCC0+0)) |
        (1<<(VCC1+0)) |
        (1<<(VCC2+0)) |
        (1<<(VCC3+0))
    );

    for(rx=0;;rx++)
    {
        PUT32(GPIOA_BSRR,
            (1<<(LEDA+ 0)) |
            (1<<(LEDB+ 0)) |
            (1<<(LEDC+ 0)) |
            (1<<(LEDD+ 0)) |
            (1<<(LEDE+ 0)) |
            (1<<(LEDF+ 0)) |
            (1<<(LEDG+ 0)) |
            (1<<(LEDP+ 0)) |
            (1<<(LEDL+16)) |
            (1<<(LEDX+16))
        );
        for(ra=0;ra<40000;ra++) dummy(ra);
        PUT32(GPIOA_BSRR,
            (1<<(LEDA+16)) |
            (1<<(LEDB+16)) |
            (1<<(LEDC+16)) |
            (1<<(LEDD+16)) |
            (1<<(LEDE+16)) |
            (1<<(LEDF+16)) |
            (1<<(LEDG+16)) |
            (1<<(LEDP+16)) |
            (1<<(LEDL+ 0)) |
            (1<<(LEDX+ 0))
        );
        for(ra=0;ra<40000;ra++) dummy(ra);
    }

    return(0);
}
