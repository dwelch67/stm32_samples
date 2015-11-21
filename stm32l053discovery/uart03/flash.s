
;@-----------------------
.cpu cortex-m0
.thumb
;@-----------------------

.thumb_func
.global _start
_start:
stacktop: .word 0x20001000
.word reset
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang
.word hang

.thumb_func
reset:
    bl notmain
    b hang
.thumb_func
hang:   b .
;@-----------------------
.align
;@-----------------------
.thumb_func
.globl PUT16
PUT16:
    strh r1,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl GET32
GET32:
    ldr r0,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl GET16
GET16:
    ldrh r0,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl dummy
dummy:
    bx lr
;@-----------------------
.thumb_func
.globl BRANCHTOR2
BRANCHTOR2:
    bx r2
;@-----------------------
.align
.thumb_func
.globl ASMDELAY
ASMDELAY:
    ldr r2,[r1]
ASMDELAY0:
    nop
    nop
    nop
    sub r0,#1
    bne ASMDELAY0
    ldr r3,[r1]
    sub r3,r2
    mov r0,r3
    bx lr
;@-----------------------
.end
;@-----------------------
