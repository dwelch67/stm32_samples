
.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
.word 0x20001000
.word reset

.thumb_func
reset:

    ldr r0,=#0x40023830
    ldr r1,=0x00100002
    str r1,[r0]

    ldr r0,=#0x40023830
    ldr r1,=0x00100002
    str r1,[r0]

    ldr r0,=0x40020400
    ldr r1,=0x10004281
    str r1,[r0]

    add r0,#0x18
    ldr r1,=0x00004081
    ldr r2,=0x40810000
    ldr r3,=0x00300000

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

