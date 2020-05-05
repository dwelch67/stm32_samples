
.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
.word 0x20001000
.word reset
.word reset
.word reset
.word reset
.word reset
.word reset
.word reset
.word reset
.word reset
.word reset
.word reset
.word reset
.word reset
.word reset
.word reset
.word reset
.word reset
.word reset
.word reset

.thumb_func
reset:

    ldr r0,=0x40021014
    ldr r1,=0x00020014
    str r1,[r0]

    ldr r0,=0x48000000
    ldr r1,=0x28005555
    str r1,[r0]

    add r0,#0x18
    ldr r1,=0x000000FF
    ldr r2,=0x00FF0000
    ldr r3,=0x00100000

top:
    str r1,[r0]
    mov r4,r3
d1:
    sub r4,#1
    bne d1
    str r2,[r0]
    mov r4,r3
d2:
    sub r4,#1
    bne d2
    b top
.align

