
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

gcc : charlie02.gcc.thumb.flash.bin charlie02.gcc.thumb.sram.bin

all : gcc clang

clang : charlie02.clang.thumb.norm.flash.bin charlie02.clang.thumb.opt.flash.bin charlie02.clang.thumb.norm.sram.bin charlie02.clang.thumb.opt.sram.bin 

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

charlie02.gcc.thumb.o : charlie02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c charlie02.c -o charlie02.gcc.thumb.o

charlie02.gcc.thumb.flash.bin : flash.ld flash.o charlie02.gcc.thumb.o
	$(ARMGNU)-ld -o charlie02.gcc.thumb.flash.elf -T flash.ld flash.o charlie02.gcc.thumb.o
	$(ARMGNU)-objdump -D charlie02.gcc.thumb.flash.elf > charlie02.gcc.thumb.flash.list
	$(ARMGNU)-objcopy charlie02.gcc.thumb.flash.elf charlie02.gcc.thumb.flash.bin -O binary

charlie02.gcc.thumb.sram.bin : sram.ld sram.o charlie02.gcc.thumb.o
	$(ARMGNU)-ld -o charlie02.gcc.thumb.sram.elf -T sram.ld sram.o charlie02.gcc.thumb.o
	$(ARMGNU)-objdump -D charlie02.gcc.thumb.sram.elf > charlie02.gcc.thumb.sram.list
	$(ARMGNU)-objcopy charlie02.gcc.thumb.sram.elf charlie02.gcc.thumb.sram.hex -O ihex
	$(ARMGNU)-objcopy charlie02.gcc.thumb.sram.elf charlie02.gcc.thumb.sram.bin -O binary

#---------------------------------

charlie02.clang.bc : charlie02.c
	clang $(LOPS) -c charlie02.c -o charlie02.clang.bc

charlie02.clang.thumb.norm.flash.bin : flash.ld flash.o charlie02.clang.bc
	#llc $(LLCOPS) charlie02.clang.bc -o charlie02.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) charlie02.clang.thumb.norm.s -o charlie02.clang.thumb.norm.o
	llc $(LLCOPS) charlie02.clang.bc -filetype=obj -o charlie02.clang.thumb.norm.o
	$(ARMGNU)-ld -o charlie02.clang.thumb.norm.flash.elf -T flash.ld flash.o charlie02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D charlie02.clang.thumb.norm.flash.elf > charlie02.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy charlie02.clang.thumb.norm.flash.elf charlie02.clang.thumb.norm.flash.bin -O binary

charlie02.clang.thumb.opt.flash.bin : flash.ld flash.o charlie02.clang.bc
	opt $(OOPS) charlie02.clang.bc -o charlie02.clang.thumb.opt.bc
	#llc $(LLCOPS) charlie02.clang.thumb.opt.bc -o charlie02.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) charlie02.clang.thumb.opt.s -o charlie02.clang.thumb.opt.o
	llc $(LLCOPS) charlie02.clang.thumb.opt.bc -filetype=obj -o charlie02.clang.thumb.opt.o
	$(ARMGNU)-ld -o charlie02.clang.thumb.opt.flash.elf -T flash.ld flash.o charlie02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D charlie02.clang.thumb.opt.flash.elf > charlie02.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy charlie02.clang.thumb.opt.flash.elf charlie02.clang.thumb.opt.flash.bin -O binary


charlie02.clang.thumb.norm.sram.bin : sram.ld sram.o charlie02.clang.bc
	#llc $(LLCOPS) charlie02.clang.bc -o charlie02.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) charlie02.clang.thumb.norm.s -o charlie02.clang.thumb.norm.o
	llc $(LLCOPS) charlie02.clang.bc -filetype=obj -o charlie02.clang.thumb.norm.o
	$(ARMGNU)-ld -o charlie02.clang.thumb.norm.sram.elf -T sram.ld sram.o charlie02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D charlie02.clang.thumb.norm.sram.elf > charlie02.clang.thumb.norm.sram.list
	$(ARMGNU)-objcopy charlie02.clang.thumb.norm.sram.elf charlie02.clang.thumb.norm.sram.hex -O ihex
	$(ARMGNU)-objcopy charlie02.clang.thumb.norm.sram.elf charlie02.clang.thumb.norm.sram.bin -O binary

charlie02.clang.thumb.opt.sram.bin : sram.ld sram.o charlie02.clang.bc
	opt $(OOPS) charlie02.clang.bc -o charlie02.clang.thumb.opt.bc
	#llc $(LLCOPS) charlie02.clang.thumb.opt.bc -o charlie02.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) charlie02.clang.thumb.opt.s -o charlie02.clang.thumb.opt.o
	llc $(LLCOPS) charlie02.clang.thumb.opt.bc -filetype=obj -o charlie02.clang.thumb.opt.o
	$(ARMGNU)-ld -o charlie02.clang.thumb.opt.sram.elf -T sram.ld sram.o charlie02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D charlie02.clang.thumb.opt.sram.elf > charlie02.clang.thumb.opt.sram.list
	$(ARMGNU)-objcopy charlie02.clang.thumb.opt.sram.elf charlie02.clang.thumb.opt.sram.hex -O ihex
	$(ARMGNU)-objcopy charlie02.clang.thumb.opt.sram.elf charlie02.clang.thumb.opt.sram.bin -O binary




