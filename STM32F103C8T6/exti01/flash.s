
.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
.word 0x20005000
.word reset
.word hang @ NMI
.word hang @ Hard Fault
.word hang @ MMU Fault
.word hang @ Bus Fault
.word hang @ Usage Fault
.word hang
.word hang
.word hang
.word hang
.word hang @ svcall
.word hang
.word hang
.word hang @ pendsv
.word hang @ systick

.word hang @ irq  0
.word hang @ irq  1
.word hang @ irq  2
.word hang @ irq  3
.word hang @ irq  4
.word hang @ irq  5
.word hang @ irq  6
.word hang @ irq  7
.word hang @ irq  8
.word hang @ irq  9
.word hang @ irq 10
.word hang @ irq 11
.word hang @ irq 12
.word hang @ irq 13
.word hang @ irq 14
.word hang @ irq 15
.word hang @ irq 16
.word hang @ irq 17
.word hang @ irq 18
.word hang @ irq 19
.word hang @ irq 20
.word hang @ irq 21
.word hang @ irq 22
.word int_handler @ irq 23
.word hang @ irq 24
.word hang @ irq 25
.word hang @ irq 26
.word hang @ irq 27
.word hang @ irq 28
.word hang @ irq 29

.thumb_func
reset:
    bl notmain
    b hang
.thumb_func
hang:   b .

.end

