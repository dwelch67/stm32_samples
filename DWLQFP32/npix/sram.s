

.cpu cortex-m0
.thumb


.thumb_func
.global _start
_start:
    b reset
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
.globl GET16
GET16:
    ldrh r0,[r0]
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


.align
gpiobase:   .word
gpioon:     .word
gpiooff:    .word


.thumb_func
.globl gpio_setup
gpio_setup:


.thumb_func
.globl xshort_on
xshort_on:
    str r2,[r0]
    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop

    bx lr

.thumb_func
.globl xlong_off
xlong_off:
    str r1,[r0]
    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop

    nop
    nop
    nop
    nop

    bx lr

.end

