
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

gcc : 7x4led.gcc.thumb.flash.bin 7x4led.gcc.thumb.sram.bin

all : gcc clang

clang : 7x4led.clang.thumb.norm.flash.bin 7x4led.clang.thumb.opt.flash.bin 7x4led.clang.thumb.norm.sram.bin 7x4led.clang.thumb.opt.sram.bin 

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

7x4led.gcc.thumb.o : 7x4led.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c 7x4led.c -o 7x4led.gcc.thumb.o

7x4led.gcc.thumb.flash.bin : flash.ld flash.o 7x4led.gcc.thumb.o
	$(ARMGNU)-ld -o 7x4led.gcc.thumb.flash.elf -T flash.ld flash.o 7x4led.gcc.thumb.o
	$(ARMGNU)-objdump -D 7x4led.gcc.thumb.flash.elf > 7x4led.gcc.thumb.flash.list
	$(ARMGNU)-objcopy 7x4led.gcc.thumb.flash.elf 7x4led.gcc.thumb.flash.bin -O binary

7x4led.gcc.thumb.sram.bin : sram.ld sram.o 7x4led.gcc.thumb.o
	$(ARMGNU)-ld -o 7x4led.gcc.thumb.sram.elf -T sram.ld sram.o 7x4led.gcc.thumb.o
	$(ARMGNU)-objdump -D 7x4led.gcc.thumb.sram.elf > 7x4led.gcc.thumb.sram.list
	$(ARMGNU)-objcopy 7x4led.gcc.thumb.sram.elf 7x4led.gcc.thumb.sram.bin -O binary

#---------------------------------

7x4led.clang.bc : 7x4led.c
	clang $(LOPS) -c 7x4led.c -o 7x4led.clang.bc

7x4led.clang.thumb.norm.flash.bin : flash.ld flash.o 7x4led.clang.bc
	#llc $(LLCOPS) 7x4led.clang.bc -o 7x4led.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) 7x4led.clang.thumb.norm.s -o 7x4led.clang.thumb.norm.o
	llc $(LLCOPS) 7x4led.clang.bc -filetype=obj -o 7x4led.clang.thumb.norm.o
	$(ARMGNU)-ld -o 7x4led.clang.thumb.norm.flash.elf -T flash.ld flash.o 7x4led.clang.thumb.norm.o
	$(ARMGNU)-objdump -D 7x4led.clang.thumb.norm.flash.elf > 7x4led.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy 7x4led.clang.thumb.norm.flash.elf 7x4led.clang.thumb.norm.flash.bin -O binary

7x4led.clang.thumb.opt.flash.bin : flash.ld flash.o 7x4led.clang.bc
	opt $(OOPS) 7x4led.clang.bc -o 7x4led.clang.thumb.opt.bc
	#llc $(LLCOPS) 7x4led.clang.thumb.opt.bc -o 7x4led.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) 7x4led.clang.thumb.opt.s -o 7x4led.clang.thumb.opt.o
	llc $(LLCOPS) 7x4led.clang.thumb.opt.bc -filetype=obj -o 7x4led.clang.thumb.opt.o
	$(ARMGNU)-ld -o 7x4led.clang.thumb.opt.flash.elf -T flash.ld flash.o 7x4led.clang.thumb.opt.o
	$(ARMGNU)-objdump -D 7x4led.clang.thumb.opt.flash.elf > 7x4led.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy 7x4led.clang.thumb.opt.flash.elf 7x4led.clang.thumb.opt.flash.bin -O binary


7x4led.clang.thumb.norm.sram.bin : sram.ld sram.o 7x4led.clang.bc
	#llc $(LLCOPS) 7x4led.clang.bc -o 7x4led.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) 7x4led.clang.thumb.norm.s -o 7x4led.clang.thumb.norm.o
	llc $(LLCOPS) 7x4led.clang.bc -filetype=obj -o 7x4led.clang.thumb.norm.o
	$(ARMGNU)-ld -o 7x4led.clang.thumb.norm.sram.elf -T sram.ld sram.o 7x4led.clang.thumb.norm.o
	$(ARMGNU)-objdump -D 7x4led.clang.thumb.norm.sram.elf > 7x4led.clang.thumb.norm.sram.list
	$(ARMGNU)-objcopy 7x4led.clang.thumb.norm.sram.elf 7x4led.clang.thumb.norm.sram.bin -O binary

7x4led.clang.thumb.opt.sram.bin : sram.ld sram.o 7x4led.clang.bc
	opt $(OOPS) 7x4led.clang.bc -o 7x4led.clang.thumb.opt.bc
	#llc $(LLCOPS) 7x4led.clang.thumb.opt.bc -o 7x4led.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) 7x4led.clang.thumb.opt.s -o 7x4led.clang.thumb.opt.o
	llc $(LLCOPS) 7x4led.clang.thumb.opt.bc -filetype=obj -o 7x4led.clang.thumb.opt.o
	$(ARMGNU)-ld -o 7x4led.clang.thumb.opt.sram.elf -T sram.ld sram.o 7x4led.clang.thumb.opt.o
	$(ARMGNU)-objdump -D 7x4led.clang.thumb.opt.sram.elf > 7x4led.clang.thumb.opt.sram.list
	$(ARMGNU)-objcopy 7x4led.clang.thumb.opt.sram.elf 7x4led.clang.thumb.opt.sram.bin -O binary




