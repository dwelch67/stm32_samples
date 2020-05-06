

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
.word systick_handler   // 15 SysTick
@.word hang              // 16 irq  0
@.word hang              // 17 irq  1
@.word hang              // 18 irq  2
@.word hang              // 19 irq  3
@.word hang              // 20 irq  4
@.word hang              // 21 irq  5
@.word hang              // 22 irq  6
@.word hang              // 23 irq  7
@.word hang              // 24 irq  8
@.word hang              // 25 irq  9
@.word hang              // 26 irq 10
@.word hang              // 27 irq 11
@.word hang              // 28 irq 12
@.word hang              // 29 irq 13
@.word hang              // 30 irq 14
@.word hang              // 31 irq 15
@.word hang              // 32 irq 16
@.word hang              // 33 irq 17
@.word hang              // 34 irq 18
@.word hang              // 35 irq 19
@.word hang              // 36 irq 20
@.word hang              // 37 irq 21
@.word hang              // 38 irq 22
@.word hang              // 39 irq 23
@.word hang              // 40 irq 24
@.word hang              // 41 irq 25
@.word hang              // 42 irq 26
@.word hang              // 43 irq 27
@.word hang              // 44 irq 28
@.word hang              // 45 irq 29
@.word hang              // 46 irq 30
@.word hang              // 47 irq 31

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

.thumb_func
.globl ienable
ienable:
    cpsie i
    bx lr


.end

