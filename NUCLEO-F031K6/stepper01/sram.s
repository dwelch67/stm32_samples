
;@-----------------------
.cpu cortex-m0
.thumb
;@-----------------------
.thumb_func
.global _start
_start:
    ldr r0,stacktop
    mov sp,r0
    bl notmain
    b hang
.thumb_func
hang:   b .
;@-----------------------
.align
stacktop: .word 0x20001000
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
.globl dummy
dummy:
    bx lr
;@-----------------------
.end
;@-----------------------
