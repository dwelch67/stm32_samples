
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

gcc : charlie04.gcc.thumb.flash.bin charlie04.gcc.thumb.sram.bin

all : gcc clang

clang : charlie04.clang.thumb.norm.flash.bin charlie04.clang.thumb.opt.flash.bin charlie04.clang.thumb.norm.sram.bin charlie04.clang.thumb.opt.sram.bin 

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f *.hex
	rm -f precomp
	rm -f data.h


data.h : precomp.c
	gcc precomp.c -o precomp
	./precomp > data.h

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

charlie04.gcc.thumb.o : charlie04.c data.h
	$(ARMGNU)-gcc $(COPS) -mthumb -c charlie04.c -o charlie04.gcc.thumb.o

charlie04.gcc.thumb.flash.bin : flash.ld flash.o charlie04.gcc.thumb.o
	$(ARMGNU)-ld -o charlie04.gcc.thumb.flash.elf -T flash.ld flash.o charlie04.gcc.thumb.o
	$(ARMGNU)-objdump -D charlie04.gcc.thumb.flash.elf > charlie04.gcc.thumb.flash.list
	$(ARMGNU)-objcopy charlie04.gcc.thumb.flash.elf charlie04.gcc.thumb.flash.bin -O binary

charlie04.gcc.thumb.sram.bin : sram.ld sram.o charlie04.gcc.thumb.o
	$(ARMGNU)-ld -o charlie04.gcc.thumb.sram.elf -T sram.ld sram.o charlie04.gcc.thumb.o
	$(ARMGNU)-objdump -D charlie04.gcc.thumb.sram.elf > charlie04.gcc.thumb.sram.list
	$(ARMGNU)-objcopy charlie04.gcc.thumb.sram.elf charlie04.gcc.thumb.sram.hex -O ihex
	$(ARMGNU)-objcopy charlie04.gcc.thumb.sram.elf charlie04.gcc.thumb.sram.bin -O binary

#---------------------------------

charlie04.clang.bc : charlie04.c data.h
	clang $(LOPS) -c charlie04.c -o charlie04.clang.bc

charlie04.clang.thumb.norm.flash.bin : flash.ld flash.o charlie04.clang.bc
	#llc $(LLCOPS) charlie04.clang.bc -o charlie04.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) charlie04.clang.thumb.norm.s -o charlie04.clang.thumb.norm.o
	llc $(LLCOPS) charlie04.clang.bc -filetype=obj -o charlie04.clang.thumb.norm.o
	$(ARMGNU)-ld -o charlie04.clang.thumb.norm.flash.elf -T flash.ld flash.o charlie04.clang.thumb.norm.o
	$(ARMGNU)-objdump -D charlie04.clang.thumb.norm.flash.elf > charlie04.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy charlie04.clang.thumb.norm.flash.elf charlie04.clang.thumb.norm.flash.bin -O binary

charlie04.clang.thumb.opt.flash.bin : flash.ld flash.o charlie04.clang.bc
	opt $(OOPS) charlie04.clang.bc -o charlie04.clang.thumb.opt.bc
	#llc $(LLCOPS) charlie04.clang.thumb.opt.bc -o charlie04.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) charlie04.clang.thumb.opt.s -o charlie04.clang.thumb.opt.o
	llc $(LLCOPS) charlie04.clang.thumb.opt.bc -filetype=obj -o charlie04.clang.thumb.opt.o
	$(ARMGNU)-ld -o charlie04.clang.thumb.opt.flash.elf -T flash.ld flash.o charlie04.clang.thumb.opt.o
	$(ARMGNU)-objdump -D charlie04.clang.thumb.opt.flash.elf > charlie04.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy charlie04.clang.thumb.opt.flash.elf charlie04.clang.thumb.opt.flash.bin -O binary


charlie04.clang.thumb.norm.sram.bin : sram.ld sram.o charlie04.clang.bc
	#llc $(LLCOPS) charlie04.clang.bc -o charlie04.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) charlie04.clang.thumb.norm.s -o charlie04.clang.thumb.norm.o
	llc $(LLCOPS) charlie04.clang.bc -filetype=obj -o charlie04.clang.thumb.norm.o
	$(ARMGNU)-ld -o charlie04.clang.thumb.norm.sram.elf -T sram.ld sram.o charlie04.clang.thumb.norm.o
	$(ARMGNU)-objdump -D charlie04.clang.thumb.norm.sram.elf > charlie04.clang.thumb.norm.sram.list
	$(ARMGNU)-objcopy charlie04.clang.thumb.norm.sram.elf charlie04.clang.thumb.norm.sram.hex -O ihex
	$(ARMGNU)-objcopy charlie04.clang.thumb.norm.sram.elf charlie04.clang.thumb.norm.sram.bin -O binary

charlie04.clang.thumb.opt.sram.bin : sram.ld sram.o charlie04.clang.bc
	opt $(OOPS) charlie04.clang.bc -o charlie04.clang.thumb.opt.bc
	#llc $(LLCOPS) charlie04.clang.thumb.opt.bc -o charlie04.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) charlie04.clang.thumb.opt.s -o charlie04.clang.thumb.opt.o
	llc $(LLCOPS) charlie04.clang.thumb.opt.bc -filetype=obj -o charlie04.clang.thumb.opt.o
	$(ARMGNU)-ld -o charlie04.clang.thumb.opt.sram.elf -T sram.ld sram.o charlie04.clang.thumb.opt.o
	$(ARMGNU)-objdump -D charlie04.clang.thumb.opt.sram.elf > charlie04.clang.thumb.opt.sram.list
	$(ARMGNU)-objcopy charlie04.clang.thumb.opt.sram.elf charlie04.clang.thumb.opt.sram.hex -O ihex
	$(ARMGNU)-objcopy charlie04.clang.thumb.opt.sram.elf charlie04.clang.thumb.opt.sram.bin -O binary




