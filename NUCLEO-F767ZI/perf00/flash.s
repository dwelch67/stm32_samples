

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


.thumb_func
.globl DSB
DSB:
    dsb
    bx lr

.thumb_func
.globl ISB
ISB:
    isb
    bx lr

.thumb_func
.globl DISB
DISB:
    dsb
    isb
    bx lr

nop
nop
nop
nop
nop
nop



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

nop
nop
nop
nop
nop
nop
nop
nop

.thumb_func
.globl lbtest
lbtest:
    push {r4,r5}
    ldr r4,[r2]
    
lbloop:
    ldrb r3,[r0]
    ldrb r3,[r0]
    ldrb r3,[r0]
    ldrb r3,[r0]
       
    ldrb r3,[r0]
    ldrb r3,[r0]
    ldrb r3,[r0]
    ldrb r3,[r0]
       
    ldrb r3,[r0]
    ldrb r3,[r0]
    ldrb r3,[r0]
    ldrb r3,[r0]
       
    ldrb r3,[r0]
    ldrb r3,[r0]
    ldrb r3,[r0]
    ldrb r3,[r0]
    sub r1,#1
    bne lbloop
    
    ldr r5,[r2]
    sub r0,r4,r5
    pop {r4,r5}
    bx lr


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

nop
nop
nop
nop
nop
nop
nop
nop

.thumb_func
.globl sbtest
sbtest:
    push {r4,r5}
    ldr r4,[r2]
    
sbloop:
    strb r3,[r0]
    strb r3,[r0]
    strb r3,[r0]
    strb r3,[r0]

    strb r3,[r0]
    strb r3,[r0]
    strb r3,[r0]
    strb r3,[r0]

    strb r3,[r0]
    strb r3,[r0]
    strb r3,[r0]
    strb r3,[r0]

    strb r3,[r0]
    strb r3,[r0]
    strb r3,[r0]
    strb r3,[r0]
    sub r1,#1
    bne sbloop
    
    ldr r5,[r2]
    sub r0,r4,r5
    pop {r4,r5}
    bx lr


/* r0 address */
/* r1 count */
/* r2 timer address */
.thumb_func
.globl nwtest
nwtest:
    push {r4,r5}
    ldr r4,[r2]
    
nwloop:
    str r3,[r0]
    nop
    nop
    nop

    str r3,[r0]
    nop
    nop
    nop

    str r3,[r0]
    nop
    nop
    nop

    str r3,[r0]
    nop
    nop
    nop
    
    sub r1,#1
    bne nwloop
    
    ldr r5,[r2]
    sub r0,r4,r5
    pop {r4,r5}
    bx lr

nop
nop
nop
nop
nop
nop
nop
nop

.thumb_func
.globl nbtest
nbtest:
    push {r4,r5}
    ldr r4,[r2]
    
nbloop:
    strb r3,[r0]
    nop
    nop
    nop

    strb r3,[r0]
    nop
    nop
    nop

    strb r3,[r0]
    nop
    nop
    nop

    strb r3,[r0]
    nop
    nop
    nop
    
    sub r1,#1
    bne nbloop
    
    ldr r5,[r2]
    sub r0,r4,r5
    pop {r4,r5}
    bx lr


.thumb_func
.globl shtest
shtest:
    push {r4,r5}
    ldr r4,[r2]
    
shloop:
    strh r3,[r0]
    strh r3,[r0]
    strh r3,[r0]
    strh r3,[r0]

    strh r3,[r0]
    strh r3,[r0]
    strh r3,[r0]
    strh r3,[r0]

    strh r3,[r0]
    strh r3,[r0]
    strh r3,[r0]
    strh r3,[r0]

    strh r3,[r0]
    strh r3,[r0]
    strh r3,[r0]
    strh r3,[r0]
    
    sub r1,#1
    bne shloop
    
    ldr r5,[r2]
    sub r0,r4,r5
    pop {r4,r5}
    bx lr



.thumb_func
.globl PDISB
PDISB:
    str r1,[r0]
    dsb
    isb
    bx lr






