
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

gcc : uart01.gcc.thumb.flash.bin 

all : gcc clang

clang : uart01.clang.thumb.norm.flash.bin uart01.clang.thumb.opt.flash.bin 

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

uart01.gcc.thumb.o : uart01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c uart01.c -o uart01.gcc.thumb.o

uart01.gcc.thumb.flash.bin : flash.ld flash.o uart01.gcc.thumb.o
	$(ARMGNU)-ld -o uart01.gcc.thumb.flash.elf -T flash.ld flash.o uart01.gcc.thumb.o
	$(ARMGNU)-objdump -D uart01.gcc.thumb.flash.elf > uart01.gcc.thumb.flash.list
	$(ARMGNU)-objcopy uart01.gcc.thumb.flash.elf uart01.gcc.thumb.flash.bin -O binary

#---------------------------------

uart01.clang.bc : uart01.c
	clang $(LOPS) -c uart01.c -o uart01.clang.bc

uart01.clang.thumb.norm.flash.bin : flash.ld flash.o uart01.clang.bc
	#llc $(LLCOPS) uart01.clang.bc -o uart01.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) uart01.clang.thumb.norm.s -o uart01.clang.thumb.norm.o
	llc $(LLCOPS) uart01.clang.bc -filetype=obj -o uart01.clang.thumb.norm.o
	$(ARMGNU)-ld -o uart01.clang.thumb.norm.flash.elf -T flash.ld flash.o uart01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D uart01.clang.thumb.norm.flash.elf > uart01.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy uart01.clang.thumb.norm.flash.elf uart01.clang.thumb.norm.flash.bin -O binary

uart01.clang.thumb.opt.flash.bin : flash.ld flash.o uart01.clang.bc
	opt $(OOPS) uart01.clang.bc -o uart01.clang.thumb.opt.bc
	#llc $(LLCOPS) uart01.clang.thumb.opt.bc -o uart01.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) uart01.clang.thumb.opt.s -o uart01.clang.thumb.opt.o
	llc $(LLCOPS) uart01.clang.thumb.opt.bc -filetype=obj -o uart01.clang.thumb.opt.o
	$(ARMGNU)-ld -o uart01.clang.thumb.opt.flash.elf -T flash.ld flash.o uart01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D uart01.clang.thumb.opt.flash.elf > uart01.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy uart01.clang.thumb.opt.flash.elf uart01.clang.thumb.opt.flash.bin -O binary




