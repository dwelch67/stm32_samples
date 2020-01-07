
.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
stacktop: .word 0x30021000
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
    ldr r0,=0xE000ED00
    ldr r1,[r0]
    ldr r0,=0x30020004
    str r1,[r0]
    ldr r0,=0x30020008
    ldr r1,=0x11223344
    str r1,[r0]
    b hang
.thumb_func
hang:   b .
