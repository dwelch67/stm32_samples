

.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
stacktop: .word 0x20001000
.word reset
.word loop
.word loop
.word loop
.word loop
.word loop
.word loop
.word loop
.word loop
.word loop
.word loop
.word loop
.word loop
.word loop
.word loop /*systick_handler*/


.thumb_func
reset:
    bl notmain
    b loop
.thumb_func
loop:   b .

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

.end

