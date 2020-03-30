
.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
stacktop: .word 0x20000800
.word reset             //  1 reset
.word hang              //  2 NMI
.word hang              //  3 hard fault
.word hang              //  4 Reserved
.word hang              //  5 Reserved
.word hang              //  6 Reserved
.word hang              //  7 Reserved
.word hang              //  8 Reserved
.word hang              //  9 Reserved
.word hang              // 10 Reserved
.word hang              // 11 SVCall
.word hang              // 12 Reserved
.word hang              // 13 Reserved
.word hang              // 14 PendSV
.word hang              // 15 SysTick

.thumb_func
reset:
    bl notmain
    b hang
.thumb_func
hang:   b .

.align

.thumb_func
.globl PUT16
PUT16:
    strh r1,[r0]
    bx lr

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
.globl dummy
dummy:
    bx lr

.end

