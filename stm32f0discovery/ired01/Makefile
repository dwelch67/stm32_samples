
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

gcc : ired01.gcc.thumb.flash.bin ired01.gcc.thumb.sram.bin

all : gcc clang

clang : ired01.clang.thumb.norm.flash.bin ired01.clang.thumb.opt.flash.bin ired01.clang.thumb.norm.sram.bin ired01.clang.thumb.opt.sram.bin 

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

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

ired01.gcc.thumb.o : ired01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c ired01.c -o ired01.gcc.thumb.o

ired01.gcc.thumb.flash.bin : flash.ld flash.o ired01.gcc.thumb.o
	$(ARMGNU)-ld -o ired01.gcc.thumb.flash.elf -T flash.ld flash.o ired01.gcc.thumb.o
	$(ARMGNU)-objdump -D ired01.gcc.thumb.flash.elf > ired01.gcc.thumb.flash.list
	$(ARMGNU)-objcopy ired01.gcc.thumb.flash.elf ired01.gcc.thumb.flash.bin -O binary

ired01.gcc.thumb.sram.bin : sram.ld sram.o ired01.gcc.thumb.o
	$(ARMGNU)-ld -o ired01.gcc.thumb.sram.elf -T sram.ld sram.o ired01.gcc.thumb.o
	$(ARMGNU)-objdump -D ired01.gcc.thumb.sram.elf > ired01.gcc.thumb.sram.list
	$(ARMGNU)-objcopy ired01.gcc.thumb.sram.elf ired01.gcc.thumb.sram.bin -O binary

#---------------------------------

ired01.clang.bc : ired01.c
	clang $(LOPS) -c ired01.c -o ired01.clang.bc

ired01.clang.thumb.norm.flash.bin : flash.ld flash.o ired01.clang.bc
	#llc $(LLCOPS) ired01.clang.bc -o ired01.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) ired01.clang.thumb.norm.s -o ired01.clang.thumb.norm.o
	llc $(LLCOPS) ired01.clang.bc -filetype=obj -o ired01.clang.thumb.norm.o
	$(ARMGNU)-ld -o ired01.clang.thumb.norm.flash.elf -T flash.ld flash.o ired01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D ired01.clang.thumb.norm.flash.elf > ired01.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy ired01.clang.thumb.norm.flash.elf ired01.clang.thumb.norm.flash.bin -O binary

ired01.clang.thumb.opt.flash.bin : flash.ld flash.o ired01.clang.bc
	opt $(OOPS) ired01.clang.bc -o ired01.clang.thumb.opt.bc
	#llc $(LLCOPS) ired01.clang.thumb.opt.bc -o ired01.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) ired01.clang.thumb.opt.s -o ired01.clang.thumb.opt.o
	llc $(LLCOPS) ired01.clang.thumb.opt.bc -filetype=obj -o ired01.clang.thumb.opt.o
	$(ARMGNU)-ld -o ired01.clang.thumb.opt.flash.elf -T flash.ld flash.o ired01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D ired01.clang.thumb.opt.flash.elf > ired01.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy ired01.clang.thumb.opt.flash.elf ired01.clang.thumb.opt.flash.bin -O binary


ired01.clang.thumb.norm.sram.bin : sram.ld sram.o ired01.clang.bc
	#llc $(LLCOPS) ired01.clang.bc -o ired01.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) ired01.clang.thumb.norm.s -o ired01.clang.thumb.norm.o
	llc $(LLCOPS) ired01.clang.bc -filetype=obj -o ired01.clang.thumb.norm.o
	$(ARMGNU)-ld -o ired01.clang.thumb.norm.sram.elf -T sram.ld sram.o ired01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D ired01.clang.thumb.norm.sram.elf > ired01.clang.thumb.norm.sram.list
	$(ARMGNU)-objcopy ired01.clang.thumb.norm.sram.elf ired01.clang.thumb.norm.sram.bin -O binary

ired01.clang.thumb.opt.sram.bin : sram.ld sram.o ired01.clang.bc
	opt $(OOPS) ired01.clang.bc -o ired01.clang.thumb.opt.bc
	#llc $(LLCOPS) ired01.clang.thumb.opt.bc -o ired01.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) ired01.clang.thumb.opt.s -o ired01.clang.thumb.opt.o
	llc $(LLCOPS) ired01.clang.thumb.opt.bc -filetype=obj -o ired01.clang.thumb.opt.o
	$(ARMGNU)-ld -o ired01.clang.thumb.opt.sram.elf -T sram.ld sram.o ired01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D ired01.clang.thumb.opt.sram.elf > ired01.clang.thumb.opt.sram.list
	$(ARMGNU)-objcopy ired01.clang.thumb.opt.sram.elf ired01.clang.thumb.opt.sram.bin -O binary




