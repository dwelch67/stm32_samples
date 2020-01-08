
.cpu cortex-m0
.thumb

.thumb_func
.global _start
_start:
stacktop: .word 0x30030000
.word reset
.word done
.word done

.word done
.word done
.word done
.word done

.word done
.word done
.word done
.word done

.word done
.word done
.word done
.word done

.thumb_func
reset:
    ldr r0,=0xE000ED08
    ldr r1,=0x08000000
    str r1,[r0]
    ldr r2,[r1,#4]
    bx r2

.thumb_func
done:
    b done
    
