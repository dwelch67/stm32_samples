
.cpu cortex-m0
.thumb

.global _start
_start:

.word 0x20001000
.word reset
.thumb_func
reset:
    b .
