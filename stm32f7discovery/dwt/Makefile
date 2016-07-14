
ARMGNU ?= arm-none-eabi
#ARMGNU ?= arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : dwt01.gcc.thumb.flash.bin dwt01.gcc.thumb.sram.bin

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

dwt01.gcc.thumb.o : dwt01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c dwt01.c -o dwt01.gcc.thumb.o

dwt01.gcc.thumb.flash.bin : flash.ld flash.o dwt01.gcc.thumb.o
	$(ARMGNU)-ld -o dwt01.gcc.thumb.flash.elf -T flash.ld flash.o dwt01.gcc.thumb.o
	$(ARMGNU)-objdump -D dwt01.gcc.thumb.flash.elf > dwt01.gcc.thumb.flash.list
	$(ARMGNU)-objcopy dwt01.gcc.thumb.flash.elf dwt01.gcc.thumb.flash.bin -O binary

dwt01.gcc.thumb.sram.bin : sram.ld sram.o dwt01.gcc.thumb.o
	$(ARMGNU)-ld -o dwt01.gcc.thumb.sram.elf -T sram.ld sram.o dwt01.gcc.thumb.o
	$(ARMGNU)-objdump -D dwt01.gcc.thumb.sram.elf > dwt01.gcc.thumb.sram.list
	$(ARMGNU)-objcopy dwt01.gcc.thumb.sram.elf dwt01.gcc.thumb.sram.hex -O ihex
	$(ARMGNU)-objcopy dwt01.gcc.thumb.sram.elf dwt01.gcc.thumb.sram.bin -O binary


