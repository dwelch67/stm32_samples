
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m4
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m4

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LOPS2 = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m4 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
LLCOPS2 = -march=thumb -mcpu=cortex-m4 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

gcc : ds3231.gcc.thumb.flash.bin ds3231.gcc.thumb2.flash.bin

all : gcc clang

clang : ds3231.clang.thumb.norm.flash.bin ds3231.clang.thumb.opt.flash.bin ds3231.clang.thumb2.norm.flash.bin ds3231.clang.thumb2.opt.flash.bin



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

ds3231.gcc.thumb.o : ds3231.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c ds3231.c -o ds3231.gcc.thumb.o

ds3231.gcc.thumb2.flash.o : ds3231.c
	#$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -march=armv7-m -c ds3231.c -o ds3231.gcc.thumb2.flash.o
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -c ds3231.c -o ds3231.gcc.thumb2.flash.o

ds3231.gcc.thumb.flash.bin : flash.ld flash.o ds3231.gcc.thumb.o
	$(ARMGNU)-ld -o ds3231.gcc.thumb.flash.elf -T flash.ld flash.o ds3231.gcc.thumb.o
	$(ARMGNU)-objdump -D ds3231.gcc.thumb.flash.elf > ds3231.gcc.thumb.flash.list
	$(ARMGNU)-objcopy ds3231.gcc.thumb.flash.elf ds3231.gcc.thumb.flash.bin -O binary

ds3231.gcc.thumb2.flash.bin : flash.ld flash.o ds3231.gcc.thumb2.flash.o
	$(ARMGNU)-ld -o ds3231.gcc.thumb2.flash.elf -T flash.ld flash.o ds3231.gcc.thumb2.flash.o
	$(ARMGNU)-objdump -D ds3231.gcc.thumb2.flash.elf > ds3231.gcc.thumb2.flash.list
	$(ARMGNU)-objcopy ds3231.gcc.thumb2.flash.elf ds3231.gcc.thumb2.flash.bin -O binary

#---------------------------------

ds3231.clang.bc : ds3231.c
	clang $(LOPS) -c ds3231.c -o ds3231.clang.bc

ds3231.clang.thumb.norm.flash.bin : flash.ld flash.o ds3231.clang.bc
	#llc $(LLCOPS) ds3231.clang.bc -o ds3231.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) ds3231.clang.thumb.norm.s -o ds3231.clang.thumb.norm.o
	llc $(LLCOPS) ds3231.clang.bc -filetype=obj -o ds3231.clang.thumb.norm.o
	$(ARMGNU)-ld -o ds3231.clang.thumb.norm.flash.elf -T flash.ld flash.o ds3231.clang.thumb.norm.o
	$(ARMGNU)-objdump -D ds3231.clang.thumb.norm.flash.elf > ds3231.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy ds3231.clang.thumb.norm.flash.elf ds3231.clang.thumb.norm.flash.bin -O binary

ds3231.clang.thumb.opt.flash.bin : flash.ld flash.o ds3231.clang.bc
	opt $(OOPS) ds3231.clang.bc -o ds3231.clang.thumb.opt.bc
	#llc $(LLCOPS) ds3231.clang.thumb.opt.bc -o ds3231.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) ds3231.clang.thumb.opt.s -o ds3231.clang.thumb.opt.o
	llc $(LLCOPS) ds3231.clang.thumb.opt.bc -filetype=obj -o ds3231.clang.thumb.opt.o
	$(ARMGNU)-ld -o ds3231.clang.thumb.opt.flash.elf -T flash.ld flash.o ds3231.clang.thumb.opt.o
	$(ARMGNU)-objdump -D ds3231.clang.thumb.opt.flash.elf > ds3231.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy ds3231.clang.thumb.opt.flash.elf ds3231.clang.thumb.opt.flash.bin -O binary


ds3231.clang.thumb2.norm.flash.bin : flash.ld flash.o ds3231.clang.bc
	#llc $(LLCOPS2) ds3231.clang.bc -o ds3231.clang.thumb2.norm.s
	#$(ARMGNU)-as $(AOPS) ds3231.clang.thumb2.norm.s -o ds3231.clang.thumb2.norm.o
	llc $(LLCOPS2) ds3231.clang.bc -filetype=obj -o ds3231.clang.thumb2.norm.o
	$(ARMGNU)-ld -o ds3231.clang.thumb2.norm.flash.elf -T flash.ld flash.o ds3231.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D ds3231.clang.thumb2.norm.flash.elf > ds3231.clang.thumb2.norm.flash.list
	$(ARMGNU)-objcopy ds3231.clang.thumb2.norm.flash.elf ds3231.clang.thumb2.norm.flash.bin -O binary

ds3231.clang.thumb2.opt.flash.bin : flash.ld flash.o ds3231.clang.bc
	opt $(OOPS) ds3231.clang.bc -o ds3231.clang.thumb2.opt.bc
	#llc $(LLCOPS2) ds3231.clang.thumb2.opt.bc -o ds3231.clang.thumb2.opt.s
	#$(ARMGNU)-as $(AOPS) ds3231.clang.thumb2.opt.s -o ds3231.clang.thumb2.opt.o
	llc $(LLCOPS2) ds3231.clang.thumb2.opt.bc -filetype=obj -o ds3231.clang.thumb2.opt.o
	$(ARMGNU)-ld -o ds3231.clang.thumb2.opt.flash.elf -T flash.ld flash.o ds3231.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D ds3231.clang.thumb2.opt.flash.elf > ds3231.clang.thumb2.opt.flash.list
	$(ARMGNU)-objcopy ds3231.clang.thumb2.opt.flash.elf ds3231.clang.thumb2.opt.flash.bin -O binary



