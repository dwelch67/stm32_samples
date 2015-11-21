
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
#OOPS = -std-compile-opts
OOPS = -std-link-opts

all : uart03.gcc.thumb.flash.bin

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

uart03.gcc.thumb.o : uart03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c uart03.c -o uart03.gcc.thumb.o

uart03.gcc.thumb.flash.bin : flash.ld flash.o uart03.gcc.thumb.o
	$(ARMGNU)-ld -o uart03.gcc.thumb.flash.elf -T flash.ld flash.o uart03.gcc.thumb.o
	$(ARMGNU)-objdump -D uart03.gcc.thumb.flash.elf > uart03.gcc.thumb.flash.list
	$(ARMGNU)-objcopy uart03.gcc.thumb.flash.elf uart03.gcc.thumb.flash.bin -O binary

#---------------------------------

uart03.clang.bc : uart03.c
	clang $(LOPS) -c uart03.c -o uart03.clang.bc

uart03.clang.thumb.norm.flash.bin : flash.ld flash.o uart03.clang.bc
	#llc $(LLCOPS) uart03.clang.bc -o uart03.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) uart03.clang.thumb.norm.s -o uart03.clang.thumb.norm.o
	llc $(LLCOPS) uart03.clang.bc -filetype=obj -o uart03.clang.thumb.norm.o
	$(ARMGNU)-ld -o uart03.clang.thumb.norm.flash.elf -T flash.ld flash.o uart03.clang.thumb.norm.o
	$(ARMGNU)-objdump -D uart03.clang.thumb.norm.flash.elf > uart03.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy uart03.clang.thumb.norm.flash.elf uart03.clang.thumb.norm.flash.bin -O binary

uart03.clang.thumb.opt.flash.bin : flash.ld flash.o uart03.clang.bc
	opt $(OOPS) uart03.clang.bc -o uart03.clang.thumb.opt.bc
	#llc $(LLCOPS) uart03.clang.thumb.opt.bc -o uart03.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) uart03.clang.thumb.opt.s -o uart03.clang.thumb.opt.o
	llc $(LLCOPS) uart03.clang.thumb.opt.bc -filetype=obj -o uart03.clang.thumb.opt.o
	$(ARMGNU)-ld -o uart03.clang.thumb.opt.flash.elf -T flash.ld flash.o uart03.clang.thumb.opt.o
	$(ARMGNU)-objdump -D uart03.clang.thumb.opt.flash.elf > uart03.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy uart03.clang.thumb.opt.flash.elf uart03.clang.thumb.opt.flash.bin -O binary

