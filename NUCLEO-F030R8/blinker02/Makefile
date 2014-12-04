
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

gcc : blinker02.gcc.thumb.flash.bin gcc2a

all : gcc clang

clang : blinker02.clang.thumb.norm.flash.bin blinker02.clang.thumb.opt.flash.bin

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

blinker02.gcc.thumb.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker02.c -o blinker02.gcc.thumb.o

blinker02.gcc.thumb.flash.bin : flash.ld flash.o blinker02.gcc.thumb.o
	$(ARMGNU)-ld -o blinker02.gcc.thumb.flash.elf -T flash.ld flash.o blinker02.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker02.gcc.thumb.flash.elf > blinker02.gcc.thumb.flash.list
	$(ARMGNU)-objcopy blinker02.gcc.thumb.flash.elf blinker02.gcc.thumb.flash.bin -O binary

#---------------------------------

blinker02.clang.bc : blinker02.c
	clang $(LOPS) -c blinker02.c -o blinker02.clang.bc

blinker02.clang.thumb.norm.flash.bin : flash.ld flash.o blinker02.clang.bc
	#llc $(LLCOPS) blinker02.clang.bc -o blinker02.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) blinker02.clang.thumb.norm.s -o blinker02.clang.thumb.norm.o
	llc $(LLCOPS) blinker02.clang.bc -filetype=obj -o blinker02.clang.thumb.norm.o
	$(ARMGNU)-ld -o blinker02.clang.thumb.norm.flash.elf -T flash.ld flash.o blinker02.clang.thumb.norm.o
	$(ARMGNU)-objdump -D blinker02.clang.thumb.norm.flash.elf > blinker02.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy blinker02.clang.thumb.norm.flash.elf blinker02.clang.thumb.norm.flash.bin -O binary

blinker02.clang.thumb.opt.flash.bin : flash.ld flash.o blinker02.clang.bc
	opt $(OOPS) blinker02.clang.bc -o blinker02.clang.thumb.opt.bc
	#llc $(LLCOPS) blinker02.clang.thumb.opt.bc -o blinker02.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) blinker02.clang.thumb.opt.s -o blinker02.clang.thumb.opt.o
	llc $(LLCOPS) blinker02.clang.thumb.opt.bc -filetype=obj -o blinker02.clang.thumb.opt.o
	$(ARMGNU)-ld -o blinker02.clang.thumb.opt.flash.elf -T flash.ld flash.o blinker02.clang.thumb.opt.o
	$(ARMGNU)-objdump -D blinker02.clang.thumb.opt.flash.elf > blinker02.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy blinker02.clang.thumb.opt.flash.elf blinker02.clang.thumb.opt.flash.bin -O binary


gcc2a : blinker02a.gcc.thumb.flash.bin 

clang2a : blinker02a.clang.thumb.norm.flash.bin blinker02a.clang.thumb.opt.flash.bin


#---------------------------------

blinker02a.gcc.thumb.o : blinker02a.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker02a.c -o blinker02a.gcc.thumb.o

blinker02a.gcc.thumb.flash.bin : flash.ld flash.o blinker02a.gcc.thumb.o
	$(ARMGNU)-ld -o blinker02a.gcc.thumb.flash.elf -T flash.ld flash.o blinker02a.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker02a.gcc.thumb.flash.elf > blinker02a.gcc.thumb.flash.list
	$(ARMGNU)-objcopy blinker02a.gcc.thumb.flash.elf blinker02a.gcc.thumb.flash.bin -O binary

#---------------------------------

blinker02a.clang.bc : blinker02a.c
	clang $(LOPS) -c blinker02a.c -o blinker02a.clang.bc

blinker02a.clang.thumb.norm.flash.bin : flash.ld flash.o blinker02a.clang.bc
	#llc $(LLCOPS) blinker02a.clang.bc -o blinker02a.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) blinker02a.clang.thumb.norm.s -o blinker02a.clang.thumb.norm.o
	llc $(LLCOPS) blinker02a.clang.bc -filetype=obj -o blinker02a.clang.thumb.norm.o
	$(ARMGNU)-ld -o blinker02a.clang.thumb.norm.flash.elf -T flash.ld flash.o blinker02a.clang.thumb.norm.o
	$(ARMGNU)-objdump -D blinker02a.clang.thumb.norm.flash.elf > blinker02a.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy blinker02a.clang.thumb.norm.flash.elf blinker02a.clang.thumb.norm.flash.bin -O binary

blinker02a.clang.thumb.opt.flash.bin : flash.ld flash.o blinker02a.clang.bc
	opt $(OOPS) blinker02a.clang.bc -o blinker02a.clang.thumb.opt.bc
	#llc $(LLCOPS) blinker02a.clang.thumb.opt.bc -o blinker02a.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) blinker02a.clang.thumb.opt.s -o blinker02a.clang.thumb.opt.o
	llc $(LLCOPS) blinker02a.clang.thumb.opt.bc -filetype=obj -o blinker02a.clang.thumb.opt.o
	$(ARMGNU)-ld -o blinker02a.clang.thumb.opt.flash.elf -T flash.ld flash.o blinker02a.clang.thumb.opt.o
	$(ARMGNU)-objdump -D blinker02a.clang.thumb.opt.flash.elf > blinker02a.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy blinker02a.clang.thumb.opt.flash.elf blinker02a.clang.thumb.opt.flash.bin -O binary


