
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

gcc : blinker01.gcc.thumb.flash.bin blinker01.gcc.thumb.sram.bin

all : gcc clang

clang : blinker01.clang.thumb.norm.bin blinker01.clang.thumb.opt.bin 

flash.o : flash.s
	$(ARMGNU)-as flash.s -o flash.o

sram.o : sram.s
	$(ARMGNU)-as sram.s -o sram.o

blinker01.gcc.thumb.o : blinker01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker01.c -o blinker01.gcc.thumb.o

blinker01.gcc.thumb.flash.bin : flash.ld flash.o blinker01.gcc.thumb.o
	$(ARMGNU)-ld -o blinker01.gcc.thumb.flash.elf -T flash.ld flash.o blinker01.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker01.gcc.thumb.flash.elf > blinker01.gcc.thumb.flash.list
	$(ARMGNU)-objcopy blinker01.gcc.thumb.flash.elf blinker01.gcc.thumb.flash.bin -O binary

blinker01.gcc.thumb.sram.bin : sram.ld sram.o blinker01.gcc.thumb.o
	$(ARMGNU)-ld -o blinker01.gcc.thumb.sram.elf -T sram.ld sram.o blinker01.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker01.gcc.thumb.sram.elf > blinker01.gcc.thumb.sram.list
	$(ARMGNU)-objcopy blinker01.gcc.thumb.sram.elf blinker01.gcc.thumb.sram.bin -O binary





blinker01.clang.bc : blinker01.c
	clang $(LOPS) -c blinker01.c -o blinker01.clang.bc

blinker01.clang.thumb.norm.bin : flash.ld flash.o blinker01.clang.bc
	llc $(LLCOPS) blinker01.clang.bc -o blinker01.clang.thumb.norm.s
	$(ARMGNU)-as blinker01.clang.thumb.norm.s -o blinker01.clang.thumb.norm.o
	$(ARMGNU)-ld -o blinker01.clang.thumb.norm.elf -T flash.ld flash.o blinker01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D blinker01.clang.thumb.norm.elf > blinker01.clang.thumb.norm.list
	$(ARMGNU)-objcopy blinker01.clang.thumb.norm.elf blinker01.clang.thumb.norm.bin -O binary

blinker01.clang.thumb.opt.bin : flash.ld flash.o blinker01.clang.bc
	opt $(OOPS) blinker01.clang.bc -o blinker01.clang.thumb.opt.bc
	llc $(LLCOPS) blinker01.clang.thumb.opt.bc -o blinker01.clang.thumb.opt.s
	$(ARMGNU)-as blinker01.clang.thumb.opt.s -o blinker01.clang.thumb.opt.o
	$(ARMGNU)-ld -o blinker01.clang.thumb.opt.elf -T flash.ld flash.o blinker01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D blinker01.clang.thumb.opt.elf > blinker01.clang.thumb.opt.list
	$(ARMGNU)-objcopy blinker01.clang.thumb.opt.elf blinker01.clang.thumb.opt.bin -O binary



clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

