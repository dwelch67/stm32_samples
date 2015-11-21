
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
#OOPS = -std-compile-opts
OOPS = -std-link-opts

gcc : bootloader01.gcc.thumb.flash.bin 

all : gcc clang

clang : bootloader01.clang.thumb.norm.flash.bin bootloader01.clang.thumb.opt.flash.bin

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

bootloader01.gcc.thumb.o : bootloader01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c bootloader01.c -o bootloader01.gcc.thumb.o

bootloader01.gcc.thumb.flash.bin : flash.ld flash.o bootloader01.gcc.thumb.o
	$(ARMGNU)-ld -o bootloader01.gcc.thumb.flash.elf -T flash.ld flash.o bootloader01.gcc.thumb.o
	$(ARMGNU)-objdump -D bootloader01.gcc.thumb.flash.elf > bootloader01.gcc.thumb.flash.list
	$(ARMGNU)-objcopy bootloader01.gcc.thumb.flash.elf bootloader01.gcc.thumb.flash.bin -O binary

#---------------------------------

bootloader01.clang.bc : bootloader01.c
	clang $(LOPS) -c bootloader01.c -o bootloader01.clang.bc

bootloader01.clang.thumb.norm.flash.bin : flash.ld flash.o bootloader01.clang.bc
	#llc $(LLCOPS) bootloader01.clang.bc -o bootloader01.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) bootloader01.clang.thumb.norm.s -o bootloader01.clang.thumb.norm.o
	llc $(LLCOPS) bootloader01.clang.bc -filetype=obj -o bootloader01.clang.thumb.norm.o
	$(ARMGNU)-ld -o bootloader01.clang.thumb.norm.flash.elf -T flash.ld flash.o bootloader01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D bootloader01.clang.thumb.norm.flash.elf > bootloader01.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy bootloader01.clang.thumb.norm.flash.elf bootloader01.clang.thumb.norm.flash.bin -O binary

bootloader01.clang.thumb.opt.flash.bin : flash.ld flash.o bootloader01.clang.bc
	opt $(OOPS) bootloader01.clang.bc -o bootloader01.clang.thumb.opt.bc
	#llc $(LLCOPS) bootloader01.clang.thumb.opt.bc -o bootloader01.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) bootloader01.clang.thumb.opt.s -o bootloader01.clang.thumb.opt.o
	llc $(LLCOPS) bootloader01.clang.thumb.opt.bc -filetype=obj -o bootloader01.clang.thumb.opt.o
	$(ARMGNU)-ld -o bootloader01.clang.thumb.opt.flash.elf -T flash.ld flash.o bootloader01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D bootloader01.clang.thumb.opt.flash.elf > bootloader01.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy bootloader01.clang.thumb.opt.flash.elf bootloader01.clang.thumb.opt.flash.bin -O binary


