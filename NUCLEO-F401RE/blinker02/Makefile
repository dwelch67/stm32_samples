
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m4
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m4

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LOPS2 = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m4 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
LLCOPS2 = -march=thumb -mcpu=cortex-m4 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

gcc : blinker02.gcc.thumb.flash.bin blinker02.gcc.thumb2.flash.bin

all : gcc clang

clang : blinker02.clang.thumb.norm.flash.bin blinker02.clang.thumb.opt.flash.bin blinker02.clang.thumb2.norm.flash.bin blinker02.clang.thumb2.opt.flash.bin



clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

blinker02.gcc.thumb.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker02.c -o blinker02.gcc.thumb.o

blinker02.gcc.thumb2.flash.o : blinker02.c
	#$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7-m -c blinker02.c -o blinker02.gcc.thumb2.flash.o
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -c blinker02.c -o blinker02.gcc.thumb2.flash.o

blinker02.gcc.thumb.flash.bin : flash.ld flash.o blinker02.gcc.thumb.o
	$(ARMGNU)-ld -o blinker02.gcc.thumb.flash.elf -T flash.ld flash.o blinker02.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker02.gcc.thumb.flash.elf > blinker02.gcc.thumb.flash.list
	$(ARMGNU)-objcopy blinker02.gcc.thumb.flash.elf blinker02.gcc.thumb.flash.bin -O binary

blinker02.gcc.thumb2.flash.bin : flash.ld flash.o blinker02.gcc.thumb2.flash.o
	$(ARMGNU)-ld -o blinker02.gcc.thumb2.flash.elf -T flash.ld flash.o blinker02.gcc.thumb2.flash.o
	$(ARMGNU)-objdump -D blinker02.gcc.thumb2.flash.elf > blinker02.gcc.thumb2.flash.list
	$(ARMGNU)-objcopy blinker02.gcc.thumb2.flash.elf blinker02.gcc.thumb2.flash.bin -O binary

#---------------------------------

blinker02.clang.bc : blinker02.c
	clang $(LOPS) -c blinker02.c -o blinker02.clang.bc

blinker02.clang.thumb.norm.flash.bin : flash.ld flash.o blinker02.clang.bc
	#llc $(LLCOPS) blinker02.clang.bc -o blinker02.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) blinker02.clang.thumb.norm.s -o blinker02.clang.thumb.norm.o
	llc $(LLCOPS) blinker02.clang.bc -filetype=obj -o blinker02.clang.thumb.norm.o
	$(ARMGNU)-ld -o blinker02.clang.thumb.norm.flash.elf -T flash.ld flash.o blinker02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D blinker02.clang.thumb.norm.flash.elf > blinker02.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy blinker02.clang.thumb.norm.flash.elf blinker02.clang.thumb.norm.flash.bin -O binary

blinker02.clang.thumb.opt.flash.bin : flash.ld flash.o blinker02.clang.bc
	opt $(OOPS) blinker02.clang.bc -o blinker02.clang.thumb.opt.bc
	#llc $(LLCOPS) blinker02.clang.thumb.opt.bc -o blinker02.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) blinker02.clang.thumb.opt.s -o blinker02.clang.thumb.opt.o
	llc $(LLCOPS) blinker02.clang.thumb.opt.bc -filetype=obj -o blinker02.clang.thumb.opt.o
	$(ARMGNU)-ld -o blinker02.clang.thumb.opt.flash.elf -T flash.ld flash.o blinker02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D blinker02.clang.thumb.opt.flash.elf > blinker02.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy blinker02.clang.thumb.opt.flash.elf blinker02.clang.thumb.opt.flash.bin -O binary


blinker02.clang.thumb2.norm.flash.bin : flash.ld flash.o blinker02.clang.bc
	#llc $(LLCOPS2) blinker02.clang.bc -o blinker02.clang.thumb2.norm.s
	#$(ARMGNU)-as $(AOPS) blinker02.clang.thumb2.norm.s -o blinker02.clang.thumb2.norm.o
	llc $(LLCOPS2) blinker02.clang.bc -filetype=obj -o blinker02.clang.thumb2.norm.o
	$(ARMGNU)-ld -o blinker02.clang.thumb2.norm.flash.elf -T flash.ld flash.o blinker02.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D blinker02.clang.thumb2.norm.flash.elf > blinker02.clang.thumb2.norm.flash.list
	$(ARMGNU)-objcopy blinker02.clang.thumb2.norm.flash.elf blinker02.clang.thumb2.norm.flash.bin -O binary

blinker02.clang.thumb2.opt.flash.bin : flash.ld flash.o blinker02.clang.bc
	opt $(OOPS) blinker02.clang.bc -o blinker02.clang.thumb2.opt.bc
	#llc $(LLCOPS2) blinker02.clang.thumb2.opt.bc -o blinker02.clang.thumb2.opt.s
	#$(ARMGNU)-as $(AOPS) blinker02.clang.thumb2.opt.s -o blinker02.clang.thumb2.opt.o
	llc $(LLCOPS2) blinker02.clang.thumb2.opt.bc -filetype=obj -o blinker02.clang.thumb2.opt.o
	$(ARMGNU)-ld -o blinker02.clang.thumb2.opt.flash.elf -T flash.ld flash.o blinker02.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D blinker02.clang.thumb2.opt.flash.elf > blinker02.clang.thumb2.opt.flash.list
	$(ARMGNU)-objcopy blinker02.clang.thumb2.opt.flash.elf blinker02.clang.thumb2.opt.flash.bin -O binary



