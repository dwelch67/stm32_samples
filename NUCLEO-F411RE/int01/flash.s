
.thumb

.thumb_func
.global _start
_start:
stacktop: .word 0x20001000
.word reset
.word hang @ NMI
.word hang @ HardFault
.word hang @ MemManage
.word hang @ BusFault
.word hang @ UsageFault
.word hang @ 7
.word hang @ 8
.word hang @ 9
.word hang @ 10
.word hang @ SVCall
.word hang @ DebugMonitor
.word hang @ Reserved
.word hang @ PendSV
.word hang @ SysTick
.word hang @ External interrupt 0
.word hang @ External interrupt 1
.word hang @ External interrupt 2
.word hang @ External interrupt 3
.word hang @ External interrupt 4
.word hang @ External interrupt 5
.word hang @ External interrupt 6
.word hang @ External interrupt 7
.word hang @ External interrupt 8
.word hang @ External interrupt 9
.word hang @ External interrupt 10
.word hang @ External interrupt 11
.word hang @ External interrupt 12
.word hang @ External interrupt 13
.word hang @ External interrupt 14
.word hang @ External interrupt 15
.word hang @ External interrupt 16
.word hang @ External interrupt 17
.word hang @ External interrupt 18
.word hang @ External interrupt 19
.word hang @ External interrupt 20
.word hang @ External interrupt 21
.word hang @ External interrupt 22
.word hang @ External interrupt 23
.word hang @ External interrupt 24
.word hang @ External interrupt 25
.word hang @ External interrupt 26
.word hang @ External interrupt 27
.word hang @ External interrupt 28
.word hang @ External interrupt 29
.word hang @ External interrupt 30
.word hang @ External interrupt 31
.word hang @ External interrupt 32
.word hang @ External interrupt 33
.word hang @ External interrupt 34
.word hang @ External interrupt 35
.word hang @ External interrupt 36
.word hang @ External interrupt 37
.word hang @ External interrupt 38
.word hang @ External interrupt 39
.word hang @ External interrupt 40
.word hang @ External interrupt 41
.word hang @ External interrupt 42
.word hang @ External interrupt 43
.word hang @ External interrupt 44
.word hang @ External interrupt 45
.word hang @ External interrupt 46
.word hang @ External interrupt 47
.word hang @ External interrupt 48
.word hang @ External interrupt 49
.word tim5_handler @ External interrupt 50
.word hang @ External interrupt 51
.word hang @ External interrupt 52
.word hang @ External interrupt 53
.word hang @ External interrupt 54
.word hang @ External interrupt 55
.word hang @ External interrupt 56
.word hang @ External interrupt 57
.word hang @ External interrupt 58
.word hang @ External interrupt 59

.thumb_func
reset:
    bl notmain
    b hang
.thumb_func
hang:   b .

.thumb_func
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.thumb_func
.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.thumb_func
.globl DOWFI
DOWFI:
    wfi
    bx lr


