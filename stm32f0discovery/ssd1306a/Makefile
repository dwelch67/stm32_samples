
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

gcc : ssd1306a.gcc.thumb.flash.bin ssd1306a.gcc.thumb.sram.bin

all : gcc clang

clang : ssd1306a.clang.thumb.norm.flash.bin ssd1306a.clang.thumb.opt.flash.bin ssd1306a.clang.thumb.norm.sram.bin ssd1306a.clang.thumb.opt.sram.bin 

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

ssd1306a.gcc.thumb.o : ssd1306a.c fontdata.h
	$(ARMGNU)-gcc $(COPS) -mthumb -c ssd1306a.c -o ssd1306a.gcc.thumb.o

ssd1306a.gcc.thumb.flash.bin : flash.ld flash.o ssd1306a.gcc.thumb.o
	$(ARMGNU)-ld -o ssd1306a.gcc.thumb.flash.elf -T flash.ld flash.o ssd1306a.gcc.thumb.o
	$(ARMGNU)-objdump -D ssd1306a.gcc.thumb.flash.elf > ssd1306a.gcc.thumb.flash.list
	$(ARMGNU)-objcopy ssd1306a.gcc.thumb.flash.elf ssd1306a.gcc.thumb.flash.bin -O binary

ssd1306a.gcc.thumb.sram.bin : sram.ld sram.o ssd1306a.gcc.thumb.o
	$(ARMGNU)-ld -o ssd1306a.gcc.thumb.sram.elf -T sram.ld sram.o ssd1306a.gcc.thumb.o
	$(ARMGNU)-objdump -D ssd1306a.gcc.thumb.sram.elf > ssd1306a.gcc.thumb.sram.list
	$(ARMGNU)-objcopy ssd1306a.gcc.thumb.sram.elf ssd1306a.gcc.thumb.sram.bin -O binary

#---------------------------------

ssd1306a.clang.bc : ssd1306a.c fontdata.h
	clang $(LOPS) -c ssd1306a.c -o ssd1306a.clang.bc

ssd1306a.clang.thumb.norm.flash.bin : flash.ld flash.o ssd1306a.clang.bc
	#llc $(LLCOPS) ssd1306a.clang.bc -o ssd1306a.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) ssd1306a.clang.thumb.norm.s -o ssd1306a.clang.thumb.norm.o
	llc $(LLCOPS) ssd1306a.clang.bc -filetype=obj -o ssd1306a.clang.thumb.norm.o
	$(ARMGNU)-ld -o ssd1306a.clang.thumb.norm.flash.elf -T flash.ld flash.o ssd1306a.clang.thumb.norm.o
	$(ARMGNU)-objdump -D ssd1306a.clang.thumb.norm.flash.elf > ssd1306a.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy ssd1306a.clang.thumb.norm.flash.elf ssd1306a.clang.thumb.norm.flash.bin -O binary

ssd1306a.clang.thumb.opt.flash.bin : flash.ld flash.o ssd1306a.clang.bc
	opt $(OOPS) ssd1306a.clang.bc -o ssd1306a.clang.thumb.opt.bc
	#llc $(LLCOPS) ssd1306a.clang.thumb.opt.bc -o ssd1306a.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) ssd1306a.clang.thumb.opt.s -o ssd1306a.clang.thumb.opt.o
	llc $(LLCOPS) ssd1306a.clang.thumb.opt.bc -filetype=obj -o ssd1306a.clang.thumb.opt.o
	$(ARMGNU)-ld -o ssd1306a.clang.thumb.opt.flash.elf -T flash.ld flash.o ssd1306a.clang.thumb.opt.o
	$(ARMGNU)-objdump -D ssd1306a.clang.thumb.opt.flash.elf > ssd1306a.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy ssd1306a.clang.thumb.opt.flash.elf ssd1306a.clang.thumb.opt.flash.bin -O binary


ssd1306a.clang.thumb.norm.sram.bin : sram.ld sram.o ssd1306a.clang.bc
	#llc $(LLCOPS) ssd1306a.clang.bc -o ssd1306a.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) ssd1306a.clang.thumb.norm.s -o ssd1306a.clang.thumb.norm.o
	llc $(LLCOPS) ssd1306a.clang.bc -filetype=obj -o ssd1306a.clang.thumb.norm.o
	$(ARMGNU)-ld -o ssd1306a.clang.thumb.norm.sram.elf -T sram.ld sram.o ssd1306a.clang.thumb.norm.o
	$(ARMGNU)-objdump -D ssd1306a.clang.thumb.norm.sram.elf > ssd1306a.clang.thumb.norm.sram.list
	$(ARMGNU)-objcopy ssd1306a.clang.thumb.norm.sram.elf ssd1306a.clang.thumb.norm.sram.bin -O binary

ssd1306a.clang.thumb.opt.sram.bin : sram.ld sram.o ssd1306a.clang.bc
	opt $(OOPS) ssd1306a.clang.bc -o ssd1306a.clang.thumb.opt.bc
	#llc $(LLCOPS) ssd1306a.clang.thumb.opt.bc -o ssd1306a.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) ssd1306a.clang.thumb.opt.s -o ssd1306a.clang.thumb.opt.o
	llc $(LLCOPS) ssd1306a.clang.thumb.opt.bc -filetype=obj -o ssd1306a.clang.thumb.opt.o
	$(ARMGNU)-ld -o ssd1306a.clang.thumb.opt.sram.elf -T sram.ld sram.o ssd1306a.clang.thumb.opt.o
	$(ARMGNU)-objdump -D ssd1306a.clang.thumb.opt.sram.elf > ssd1306a.clang.thumb.opt.sram.list
	$(ARMGNU)-objcopy ssd1306a.clang.thumb.opt.sram.elf ssd1306a.clang.thumb.opt.sram.bin -O binary




