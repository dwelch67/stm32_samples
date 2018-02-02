
.cpu cortex-m7
.syntax unified
.thumb


.thumb_func
.globl test1
test1:
    orr r0,#0x3200
    bx lr

.thumb_func
.globl test2
test2:
    orr r0,#0x3201
    bx lr

