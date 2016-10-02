
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

int notmain ( void )
{
    PUT32(0x20000800,0x12345678);
    return(0);
}
