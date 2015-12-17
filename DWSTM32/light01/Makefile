
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
#OOPS = -std-compile-opts
OOPS = -std-link-opts

gcc : light01.gcc.thumb.flash.bin light01.gcc.thumb.sram.bin

all : gcc clang

clang : light01.clang.thumb.norm.flash.bin light01.clang.thumb.opt.flash.bin light01.clang.thumb.norm.sram.bin light01.clang.thumb.opt.sram.bin 

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

light01.gcc.thumb.o : light01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c light01.c -o light01.gcc.thumb.o

light01.gcc.thumb.flash.bin : flash.ld flash.o light01.gcc.thumb.o
	$(ARMGNU)-ld -o light01.gcc.thumb.flash.elf -T flash.ld flash.o light01.gcc.thumb.o
	$(ARMGNU)-objdump -D light01.gcc.thumb.flash.elf > light01.gcc.thumb.flash.list
	$(ARMGNU)-objcopy light01.gcc.thumb.flash.elf light01.gcc.thumb.flash.bin -O binary

light01.gcc.thumb.sram.bin : sram.ld sram.o light01.gcc.thumb.o
	$(ARMGNU)-ld -o light01.gcc.thumb.sram.elf -T sram.ld sram.o light01.gcc.thumb.o
	$(ARMGNU)-objdump -D light01.gcc.thumb.sram.elf > light01.gcc.thumb.sram.list
	$(ARMGNU)-objcopy light01.gcc.thumb.sram.elf light01.gcc.thumb.sram.hex -O ihex
	$(ARMGNU)-objcopy light01.gcc.thumb.sram.elf light01.gcc.thumb.sram.bin -O binary

#---------------------------------

light01.clang.bc : light01.c
	clang $(LOPS) -c light01.c -o light01.clang.bc

light01.clang.thumb.norm.flash.bin : flash.ld flash.o light01.clang.bc
	#llc $(LLCOPS) light01.clang.bc -o light01.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) light01.clang.thumb.norm.s -o light01.clang.thumb.norm.o
	llc $(LLCOPS) light01.clang.bc -filetype=obj -o light01.clang.thumb.norm.o
	$(ARMGNU)-ld -o light01.clang.thumb.norm.flash.elf -T flash.ld flash.o light01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D light01.clang.thumb.norm.flash.elf > light01.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy light01.clang.thumb.norm.flash.elf light01.clang.thumb.norm.flash.bin -O binary

light01.clang.thumb.opt.flash.bin : flash.ld flash.o light01.clang.bc
	opt $(OOPS) light01.clang.bc -o light01.clang.thumb.opt.bc
	#llc $(LLCOPS) light01.clang.thumb.opt.bc -o light01.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) light01.clang.thumb.opt.s -o light01.clang.thumb.opt.o
	llc $(LLCOPS) light01.clang.thumb.opt.bc -filetype=obj -o light01.clang.thumb.opt.o
	$(ARMGNU)-ld -o light01.clang.thumb.opt.flash.elf -T flash.ld flash.o light01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D light01.clang.thumb.opt.flash.elf > light01.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy light01.clang.thumb.opt.flash.elf light01.clang.thumb.opt.flash.bin -O binary


light01.clang.thumb.norm.sram.bin : sram.ld sram.o light01.clang.bc
	#llc $(LLCOPS) light01.clang.bc -o light01.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) light01.clang.thumb.norm.s -o light01.clang.thumb.norm.o
	llc $(LLCOPS) light01.clang.bc -filetype=obj -o light01.clang.thumb.norm.o
	$(ARMGNU)-ld -o light01.clang.thumb.norm.sram.elf -T sram.ld sram.o light01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D light01.clang.thumb.norm.sram.elf > light01.clang.thumb.norm.sram.list
	$(ARMGNU)-objcopy light01.clang.thumb.norm.sram.elf light01.clang.thumb.norm.sram.hex -O ihex
	$(ARMGNU)-objcopy light01.clang.thumb.norm.sram.elf light01.clang.thumb.norm.sram.bin -O binary

light01.clang.thumb.opt.sram.bin : sram.ld sram.o light01.clang.bc
	opt $(OOPS) light01.clang.bc -o light01.clang.thumb.opt.bc
	#llc $(LLCOPS) light01.clang.thumb.opt.bc -o light01.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) light01.clang.thumb.opt.s -o light01.clang.thumb.opt.o
	llc $(LLCOPS) light01.clang.thumb.opt.bc -filetype=obj -o light01.clang.thumb.opt.o
	$(ARMGNU)-ld -o light01.clang.thumb.opt.sram.elf -T sram.ld sram.o light01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D light01.clang.thumb.opt.sram.elf > light01.clang.thumb.opt.sram.list
	$(ARMGNU)-objcopy light01.clang.thumb.opt.sram.elf light01.clang.thumb.opt.sram.hex -O ihex
	$(ARMGNU)-objcopy light01.clang.thumb.opt.sram.elf light01.clang.thumb.opt.sram.bin -O binary




