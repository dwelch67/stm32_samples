
.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
    ldr r0,stack_init
    mov sp,r0
    bl notmain
    b hang
.thumb_func
hang:   b .
.align
stack_init: .word 0x20005000
