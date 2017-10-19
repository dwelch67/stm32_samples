
.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
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
.globl DOWFI
DOWFI:
    wfi
    b DOWFI


.end

