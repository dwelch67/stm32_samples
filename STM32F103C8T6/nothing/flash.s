
.thumb

.thumb_func
.global _start
_start:
stacktop: .word 0x20001000
.word reset
.word reset
.word reset

.thumb_func
reset:
    b .
