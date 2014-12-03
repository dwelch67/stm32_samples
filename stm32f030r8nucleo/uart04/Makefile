
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

gcc : uart04.gcc.thumb.flash.bin 

all : gcc clang

clang : uart04.clang.thumb.norm.flash.bin uart04.clang.thumb.opt.flash.bin 

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

uart04.gcc.thumb.o : uart04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c uart04.c -o uart04.gcc.thumb.o

uart04.gcc.thumb.flash.bin : flash.ld flash.o uart04.gcc.thumb.o
	$(ARMGNU)-ld -o uart04.gcc.thumb.flash.elf -T flash.ld flash.o uart04.gcc.thumb.o
	$(ARMGNU)-objdump -D uart04.gcc.thumb.flash.elf > uart04.gcc.thumb.flash.list
	$(ARMGNU)-objcopy uart04.gcc.thumb.flash.elf uart04.gcc.thumb.flash.bin -O binary

#---------------------------------

uart04.clang.bc : uart04.c
	clang $(LOPS) -c uart04.c -o uart04.clang.bc

uart04.clang.thumb.norm.flash.bin : flash.ld flash.o uart04.clang.bc
	#llc $(LLCOPS) uart04.clang.bc -o uart04.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) uart04.clang.thumb.norm.s -o uart04.clang.thumb.norm.o
	llc $(LLCOPS) uart04.clang.bc -filetype=obj -o uart04.clang.thumb.norm.o
	$(ARMGNU)-ld -o uart04.clang.thumb.norm.flash.elf -T flash.ld flash.o uart04.clang.thumb.norm.o
	$(ARMGNU)-objdump -D uart04.clang.thumb.norm.flash.elf > uart04.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy uart04.clang.thumb.norm.flash.elf uart04.clang.thumb.norm.flash.bin -O binary

uart04.clang.thumb.opt.flash.bin : flash.ld flash.o uart04.clang.bc
	opt $(OOPS) uart04.clang.bc -o uart04.clang.thumb.opt.bc
	#llc $(LLCOPS) uart04.clang.thumb.opt.bc -o uart04.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) uart04.clang.thumb.opt.s -o uart04.clang.thumb.opt.o
	llc $(LLCOPS) uart04.clang.thumb.opt.bc -filetype=obj -o uart04.clang.thumb.opt.o
	$(ARMGNU)-ld -o uart04.clang.thumb.opt.flash.elf -T flash.ld flash.o uart04.clang.thumb.opt.o
	$(ARMGNU)-objdump -D uart04.clang.thumb.opt.flash.elf > uart04.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy uart04.clang.thumb.opt.flash.elf uart04.clang.thumb.opt.flash.bin -O binary




