
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

gcc : nmea.gcc.thumb.flash.bin nmea.gcc.thumb.sram.bin

all : gcc clang

clang : nmea.clang.thumb.norm.flash.bin nmea.clang.thumb.opt.flash.bin nmea.clang.thumb.norm.sram.bin nmea.clang.thumb.opt.sram.bin 

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

nmea.gcc.thumb.o : nmea.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c nmea.c -o nmea.gcc.thumb.o

nmea.gcc.thumb.flash.bin : flash.ld flash.o nmea.gcc.thumb.o
	$(ARMGNU)-ld -o nmea.gcc.thumb.flash.elf -T flash.ld flash.o nmea.gcc.thumb.o
	$(ARMGNU)-objdump -D nmea.gcc.thumb.flash.elf > nmea.gcc.thumb.flash.list
	$(ARMGNU)-objcopy nmea.gcc.thumb.flash.elf nmea.gcc.thumb.flash.bin -O binary

nmea.gcc.thumb.sram.bin : sram.ld sram.o nmea.gcc.thumb.o
	$(ARMGNU)-ld -o nmea.gcc.thumb.sram.elf -T sram.ld sram.o nmea.gcc.thumb.o
	$(ARMGNU)-objdump -D nmea.gcc.thumb.sram.elf > nmea.gcc.thumb.sram.list
	$(ARMGNU)-objcopy nmea.gcc.thumb.sram.elf nmea.gcc.thumb.sram.bin -O binary

#---------------------------------

nmea.clang.bc : nmea.c
	clang $(LOPS) -c nmea.c -o nmea.clang.bc

nmea.clang.thumb.norm.flash.bin : flash.ld flash.o nmea.clang.bc
	#llc $(LLCOPS) nmea.clang.bc -o nmea.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) nmea.clang.thumb.norm.s -o nmea.clang.thumb.norm.o
	llc $(LLCOPS) nmea.clang.bc -filetype=obj -o nmea.clang.thumb.norm.o
	$(ARMGNU)-ld -o nmea.clang.thumb.norm.flash.elf -T flash.ld flash.o nmea.clang.thumb.norm.o
	$(ARMGNU)-objdump -D nmea.clang.thumb.norm.flash.elf > nmea.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy nmea.clang.thumb.norm.flash.elf nmea.clang.thumb.norm.flash.bin -O binary

nmea.clang.thumb.opt.flash.bin : flash.ld flash.o nmea.clang.bc
	opt $(OOPS) nmea.clang.bc -o nmea.clang.thumb.opt.bc
	#llc $(LLCOPS) nmea.clang.thumb.opt.bc -o nmea.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) nmea.clang.thumb.opt.s -o nmea.clang.thumb.opt.o
	llc $(LLCOPS) nmea.clang.thumb.opt.bc -filetype=obj -o nmea.clang.thumb.opt.o
	$(ARMGNU)-ld -o nmea.clang.thumb.opt.flash.elf -T flash.ld flash.o nmea.clang.thumb.opt.o
	$(ARMGNU)-objdump -D nmea.clang.thumb.opt.flash.elf > nmea.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy nmea.clang.thumb.opt.flash.elf nmea.clang.thumb.opt.flash.bin -O binary


nmea.clang.thumb.norm.sram.bin : sram.ld sram.o nmea.clang.bc
	#llc $(LLCOPS) nmea.clang.bc -o nmea.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) nmea.clang.thumb.norm.s -o nmea.clang.thumb.norm.o
	llc $(LLCOPS) nmea.clang.bc -filetype=obj -o nmea.clang.thumb.norm.o
	$(ARMGNU)-ld -o nmea.clang.thumb.norm.sram.elf -T sram.ld sram.o nmea.clang.thumb.norm.o
	$(ARMGNU)-objdump -D nmea.clang.thumb.norm.sram.elf > nmea.clang.thumb.norm.sram.list
	$(ARMGNU)-objcopy nmea.clang.thumb.norm.sram.elf nmea.clang.thumb.norm.sram.bin -O binary

nmea.clang.thumb.opt.sram.bin : sram.ld sram.o nmea.clang.bc
	opt $(OOPS) nmea.clang.bc -o nmea.clang.thumb.opt.bc
	#llc $(LLCOPS) nmea.clang.thumb.opt.bc -o nmea.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) nmea.clang.thumb.opt.s -o nmea.clang.thumb.opt.o
	llc $(LLCOPS) nmea.clang.thumb.opt.bc -filetype=obj -o nmea.clang.thumb.opt.o
	$(ARMGNU)-ld -o nmea.clang.thumb.opt.sram.elf -T sram.ld sram.o nmea.clang.thumb.opt.o
	$(ARMGNU)-objdump -D nmea.clang.thumb.opt.sram.elf > nmea.clang.thumb.opt.sram.list
	$(ARMGNU)-objcopy nmea.clang.thumb.opt.sram.elf nmea.clang.thumb.opt.sram.bin -O binary




