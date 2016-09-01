
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
#at some point they changed this option, gee thanks
#OOPS = -std-compile-opts
OOPS = -std-link-opts

gcc : stepper01.gcc.thumb.flash.bin stepper01.gcc.thumb.sram.bin

all : gcc clang

clang : stepper01.clang.thumb.norm.flash.bin stepper01.clang.thumb.opt.flash.bin stepper01.clang.thumb.norm.sram.bin stepper01.clang.thumb.opt.sram.bin 

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f *.hex

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

stepper01.gcc.thumb.o : stepper01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c stepper01.c -o stepper01.gcc.thumb.o

stepper01.gcc.thumb.flash.bin : flash.ld flash.o stepper01.gcc.thumb.o
	$(ARMGNU)-ld -o stepper01.gcc.thumb.flash.elf -T flash.ld flash.o stepper01.gcc.thumb.o
	$(ARMGNU)-objdump -D stepper01.gcc.thumb.flash.elf > stepper01.gcc.thumb.flash.list
	$(ARMGNU)-objcopy stepper01.gcc.thumb.flash.elf stepper01.gcc.thumb.flash.bin -O binary

stepper01.gcc.thumb.sram.bin : sram.ld sram.o stepper01.gcc.thumb.o
	$(ARMGNU)-ld -o stepper01.gcc.thumb.sram.elf -T sram.ld sram.o stepper01.gcc.thumb.o
	$(ARMGNU)-objdump -D stepper01.gcc.thumb.sram.elf > stepper01.gcc.thumb.sram.list
	$(ARMGNU)-objcopy stepper01.gcc.thumb.sram.elf stepper01.gcc.thumb.sram.hex -O ihex
	$(ARMGNU)-objcopy stepper01.gcc.thumb.sram.elf stepper01.gcc.thumb.sram.bin -O binary

#---------------------------------

stepper01.clang.bc : stepper01.c
	clang $(LOPS) -c stepper01.c -o stepper01.clang.bc

stepper01.clang.thumb.norm.flash.bin : flash.ld flash.o stepper01.clang.bc
	#llc $(LLCOPS) stepper01.clang.bc -o stepper01.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) stepper01.clang.thumb.norm.s -o stepper01.clang.thumb.norm.o
	llc $(LLCOPS) stepper01.clang.bc -filetype=obj -o stepper01.clang.thumb.norm.o
	$(ARMGNU)-ld -o stepper01.clang.thumb.norm.flash.elf -T flash.ld flash.o stepper01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D stepper01.clang.thumb.norm.flash.elf > stepper01.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy stepper01.clang.thumb.norm.flash.elf stepper01.clang.thumb.norm.flash.bin -O binary

stepper01.clang.thumb.opt.flash.bin : flash.ld flash.o stepper01.clang.bc
	opt $(OOPS) stepper01.clang.bc -o stepper01.clang.thumb.opt.bc
	#llc $(LLCOPS) stepper01.clang.thumb.opt.bc -o stepper01.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) stepper01.clang.thumb.opt.s -o stepper01.clang.thumb.opt.o
	llc $(LLCOPS) stepper01.clang.thumb.opt.bc -filetype=obj -o stepper01.clang.thumb.opt.o
	$(ARMGNU)-ld -o stepper01.clang.thumb.opt.flash.elf -T flash.ld flash.o stepper01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D stepper01.clang.thumb.opt.flash.elf > stepper01.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy stepper01.clang.thumb.opt.flash.elf stepper01.clang.thumb.opt.flash.bin -O binary


stepper01.clang.thumb.norm.sram.bin : sram.ld sram.o stepper01.clang.bc
	#llc $(LLCOPS) stepper01.clang.bc -o stepper01.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) stepper01.clang.thumb.norm.s -o stepper01.clang.thumb.norm.o
	llc $(LLCOPS) stepper01.clang.bc -filetype=obj -o stepper01.clang.thumb.norm.o
	$(ARMGNU)-ld -o stepper01.clang.thumb.norm.sram.elf -T sram.ld sram.o stepper01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D stepper01.clang.thumb.norm.sram.elf > stepper01.clang.thumb.norm.sram.list
	$(ARMGNU)-objcopy stepper01.clang.thumb.norm.sram.elf stepper01.clang.thumb.norm.sram.hex -O ihex
	$(ARMGNU)-objcopy stepper01.clang.thumb.norm.sram.elf stepper01.clang.thumb.norm.sram.bin -O binary

stepper01.clang.thumb.opt.sram.bin : sram.ld sram.o stepper01.clang.bc
	opt $(OOPS) stepper01.clang.bc -o stepper01.clang.thumb.opt.bc
	#llc $(LLCOPS) stepper01.clang.thumb.opt.bc -o stepper01.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) stepper01.clang.thumb.opt.s -o stepper01.clang.thumb.opt.o
	llc $(LLCOPS) stepper01.clang.thumb.opt.bc -filetype=obj -o stepper01.clang.thumb.opt.o
	$(ARMGNU)-ld -o stepper01.clang.thumb.opt.sram.elf -T sram.ld sram.o stepper01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D stepper01.clang.thumb.opt.sram.elf > stepper01.clang.thumb.opt.sram.list
	$(ARMGNU)-objcopy stepper01.clang.thumb.opt.sram.elf stepper01.clang.thumb.opt.sram.hex -O ihex
	$(ARMGNU)-objcopy stepper01.clang.thumb.opt.sram.elf stepper01.clang.thumb.opt.sram.bin -O binary




