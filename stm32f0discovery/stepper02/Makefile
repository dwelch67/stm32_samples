
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

gcc : stepper02.gcc.thumb.flash.bin stepper02.gcc.thumb.sram.bin

all : gcc clang

clang : stepper02.clang.thumb.norm.flash.bin stepper02.clang.thumb.opt.flash.bin stepper02.clang.thumb.norm.sram.bin stepper02.clang.thumb.opt.sram.bin 

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

stepper02.gcc.thumb.o : stepper02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c stepper02.c -o stepper02.gcc.thumb.o

stepper02.gcc.thumb.flash.bin : flash.ld flash.o stepper02.gcc.thumb.o
	$(ARMGNU)-ld -o stepper02.gcc.thumb.flash.elf -T flash.ld flash.o stepper02.gcc.thumb.o
	$(ARMGNU)-objdump -D stepper02.gcc.thumb.flash.elf > stepper02.gcc.thumb.flash.list
	$(ARMGNU)-objcopy stepper02.gcc.thumb.flash.elf stepper02.gcc.thumb.flash.bin -O binary

stepper02.gcc.thumb.sram.bin : sram.ld sram.o stepper02.gcc.thumb.o
	$(ARMGNU)-ld -o stepper02.gcc.thumb.sram.elf -T sram.ld sram.o stepper02.gcc.thumb.o
	$(ARMGNU)-objdump -D stepper02.gcc.thumb.sram.elf > stepper02.gcc.thumb.sram.list
	$(ARMGNU)-objcopy stepper02.gcc.thumb.sram.elf stepper02.gcc.thumb.sram.bin -O binary

#---------------------------------

stepper02.clang.bc : stepper02.c
	clang $(LOPS) -c stepper02.c -o stepper02.clang.bc

stepper02.clang.thumb.norm.flash.bin : flash.ld flash.o stepper02.clang.bc
	#llc $(LLCOPS) stepper02.clang.bc -o stepper02.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) stepper02.clang.thumb.norm.s -o stepper02.clang.thumb.norm.o
	llc $(LLCOPS) stepper02.clang.bc -filetype=obj -o stepper02.clang.thumb.norm.o
	$(ARMGNU)-ld -o stepper02.clang.thumb.norm.flash.elf -T flash.ld flash.o stepper02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D stepper02.clang.thumb.norm.flash.elf > stepper02.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy stepper02.clang.thumb.norm.flash.elf stepper02.clang.thumb.norm.flash.bin -O binary

stepper02.clang.thumb.opt.flash.bin : flash.ld flash.o stepper02.clang.bc
	opt $(OOPS) stepper02.clang.bc -o stepper02.clang.thumb.opt.bc
	#llc $(LLCOPS) stepper02.clang.thumb.opt.bc -o stepper02.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) stepper02.clang.thumb.opt.s -o stepper02.clang.thumb.opt.o
	llc $(LLCOPS) stepper02.clang.thumb.opt.bc -filetype=obj -o stepper02.clang.thumb.opt.o
	$(ARMGNU)-ld -o stepper02.clang.thumb.opt.flash.elf -T flash.ld flash.o stepper02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D stepper02.clang.thumb.opt.flash.elf > stepper02.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy stepper02.clang.thumb.opt.flash.elf stepper02.clang.thumb.opt.flash.bin -O binary


stepper02.clang.thumb.norm.sram.bin : sram.ld sram.o stepper02.clang.bc
	#llc $(LLCOPS) stepper02.clang.bc -o stepper02.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) stepper02.clang.thumb.norm.s -o stepper02.clang.thumb.norm.o
	llc $(LLCOPS) stepper02.clang.bc -filetype=obj -o stepper02.clang.thumb.norm.o
	$(ARMGNU)-ld -o stepper02.clang.thumb.norm.sram.elf -T sram.ld sram.o stepper02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D stepper02.clang.thumb.norm.sram.elf > stepper02.clang.thumb.norm.sram.list
	$(ARMGNU)-objcopy stepper02.clang.thumb.norm.sram.elf stepper02.clang.thumb.norm.sram.bin -O binary

stepper02.clang.thumb.opt.sram.bin : sram.ld sram.o stepper02.clang.bc
	opt $(OOPS) stepper02.clang.bc -o stepper02.clang.thumb.opt.bc
	#llc $(LLCOPS) stepper02.clang.thumb.opt.bc -o stepper02.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) stepper02.clang.thumb.opt.s -o stepper02.clang.thumb.opt.o
	llc $(LLCOPS) stepper02.clang.thumb.opt.bc -filetype=obj -o stepper02.clang.thumb.opt.o
	$(ARMGNU)-ld -o stepper02.clang.thumb.opt.sram.elf -T sram.ld sram.o stepper02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D stepper02.clang.thumb.opt.sram.elf > stepper02.clang.thumb.opt.sram.list
	$(ARMGNU)-objcopy stepper02.clang.thumb.opt.sram.elf stepper02.clang.thumb.opt.sram.bin -O binary




