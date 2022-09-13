

.cpu cortex-m0
.thumb


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
.globl GET16
GET16:
    ldrh r0,[r0]
    bx lr

.thumb_func
.globl dummy
dummy:
    bx lr


.balign 0x100
/* r0 count */
/* r1 timer address */
.thumb_func
.globl TEST
TEST:
    push {r4,r5}
    ldr r4,[r1]

loop:
    sub r0,#1
    bne loop

    ldr r5,[r1]
    sub r0,r4,r5
    pop {r4,r5}
    bx lr

    nop
    nop
    nop
    nop

.balign 0x80

/* r0 address */
/* r1 count */
/* r2 timer address */
.thumb_func
.globl swtest
swtest:
    push {r4,r5}
    ldr r4,[r2]

swloop:
    str r3,[r0]
    str r3,[r0]
    str r3,[r0]
    str r3,[r0]

    str r3,[r0]
    str r3,[r0]
    str r3,[r0]
    str r3,[r0]

    str r3,[r0]
    str r3,[r0]
    str r3,[r0]
    str r3,[r0]

    str r3,[r0]
    str r3,[r0]
    str r3,[r0]
    str r3,[r0]
    sub r1,#1
    bne swloop

    ldr r5,[r2]
    sub r0,r4,r5
    pop {r4,r5}
    bx lr



.balign 0x80

/* r0 address */
/* r1 count */
/* r2 timer address */
.thumb_func
.globl lwtest
lwtest:
    push {r4,r5}
    ldr r4,[r2]

lwloop:
    ldr r3,[r0]
    ldr r3,[r0]
    ldr r3,[r0]
    ldr r3,[r0]

    ldr r3,[r0]
    ldr r3,[r0]
    ldr r3,[r0]
    ldr r3,[r0]

    ldr r3,[r0]
    ldr r3,[r0]
    ldr r3,[r0]
    ldr r3,[r0]

    ldr r3,[r0]
    ldr r3,[r0]
    ldr r3,[r0]
    ldr r3,[r0]
    sub r1,#1
    bne lwloop

    ldr r5,[r2]
    sub r0,r4,r5
    pop {r4,r5}
    bx lr


.globl HOP
HOP:
    bx r2
