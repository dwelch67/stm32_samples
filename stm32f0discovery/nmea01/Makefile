
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

gcc : nmea01.gcc.thumb.flash.bin nmea01.gcc.thumb.sram.bin

all : gcc clang

clang : nmea01.clang.thumb.norm.flash.bin nmea01.clang.thumb.opt.flash.bin nmea01.clang.thumb.norm.sram.bin nmea01.clang.thumb.opt.sram.bin 

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

nmea01.gcc.thumb.o : nmea01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c nmea01.c -o nmea01.gcc.thumb.o

nmea01.gcc.thumb.flash.bin : flash.ld flash.o nmea01.gcc.thumb.o
	$(ARMGNU)-ld -o nmea01.gcc.thumb.flash.elf -T flash.ld flash.o nmea01.gcc.thumb.o
	$(ARMGNU)-objdump -D nmea01.gcc.thumb.flash.elf > nmea01.gcc.thumb.flash.list
	$(ARMGNU)-objcopy nmea01.gcc.thumb.flash.elf nmea01.gcc.thumb.flash.bin -O binary

nmea01.gcc.thumb.sram.bin : sram.ld sram.o nmea01.gcc.thumb.o
	$(ARMGNU)-ld -o nmea01.gcc.thumb.sram.elf -T sram.ld sram.o nmea01.gcc.thumb.o
	$(ARMGNU)-objdump -D nmea01.gcc.thumb.sram.elf > nmea01.gcc.thumb.sram.list
	$(ARMGNU)-objcopy nmea01.gcc.thumb.sram.elf nmea01.gcc.thumb.sram.bin -O binary

#---------------------------------

nmea01.clang.bc : nmea01.c
	clang $(LOPS) -c nmea01.c -o nmea01.clang.bc

nmea01.clang.thumb.norm.flash.bin : flash.ld flash.o nmea01.clang.bc
	#llc $(LLCOPS) nmea01.clang.bc -o nmea01.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) nmea01.clang.thumb.norm.s -o nmea01.clang.thumb.norm.o
	llc $(LLCOPS) nmea01.clang.bc -filetype=obj -o nmea01.clang.thumb.norm.o
	$(ARMGNU)-ld -o nmea01.clang.thumb.norm.flash.elf -T flash.ld flash.o nmea01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D nmea01.clang.thumb.norm.flash.elf > nmea01.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy nmea01.clang.thumb.norm.flash.elf nmea01.clang.thumb.norm.flash.bin -O binary

nmea01.clang.thumb.opt.flash.bin : flash.ld flash.o nmea01.clang.bc
	opt $(OOPS) nmea01.clang.bc -o nmea01.clang.thumb.opt.bc
	#llc $(LLCOPS) nmea01.clang.thumb.opt.bc -o nmea01.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) nmea01.clang.thumb.opt.s -o nmea01.clang.thumb.opt.o
	llc $(LLCOPS) nmea01.clang.thumb.opt.bc -filetype=obj -o nmea01.clang.thumb.opt.o
	$(ARMGNU)-ld -o nmea01.clang.thumb.opt.flash.elf -T flash.ld flash.o nmea01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D nmea01.clang.thumb.opt.flash.elf > nmea01.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy nmea01.clang.thumb.opt.flash.elf nmea01.clang.thumb.opt.flash.bin -O binary


nmea01.clang.thumb.norm.sram.bin : sram.ld sram.o nmea01.clang.bc
	#llc $(LLCOPS) nmea01.clang.bc -o nmea01.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) nmea01.clang.thumb.norm.s -o nmea01.clang.thumb.norm.o
	llc $(LLCOPS) nmea01.clang.bc -filetype=obj -o nmea01.clang.thumb.norm.o
	$(ARMGNU)-ld -o nmea01.clang.thumb.norm.sram.elf -T sram.ld sram.o nmea01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D nmea01.clang.thumb.norm.sram.elf > nmea01.clang.thumb.norm.sram.list
	$(ARMGNU)-objcopy nmea01.clang.thumb.norm.sram.elf nmea01.clang.thumb.norm.sram.bin -O binary

nmea01.clang.thumb.opt.sram.bin : sram.ld sram.o nmea01.clang.bc
	opt $(OOPS) nmea01.clang.bc -o nmea01.clang.thumb.opt.bc
	#llc $(LLCOPS) nmea01.clang.thumb.opt.bc -o nmea01.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) nmea01.clang.thumb.opt.s -o nmea01.clang.thumb.opt.o
	llc $(LLCOPS) nmea01.clang.thumb.opt.bc -filetype=obj -o nmea01.clang.thumb.opt.o
	$(ARMGNU)-ld -o nmea01.clang.thumb.opt.sram.elf -T sram.ld sram.o nmea01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D nmea01.clang.thumb.opt.sram.elf > nmea01.clang.thumb.opt.sram.list
	$(ARMGNU)-objcopy nmea01.clang.thumb.opt.sram.elf nmea01.clang.thumb.opt.sram.bin -O binary




