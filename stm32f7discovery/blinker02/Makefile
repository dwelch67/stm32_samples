
ARMGNU ?= arm-none-eabi
#ARMGNU ?= arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : blinker02.gcc.thumb.flash.bin blinker02.gcc.thumb.sram.bin

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

blinker02.gcc.thumb.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker02.c -o blinker02.gcc.thumb.o

blinker02.gcc.thumb.flash.bin : flash.ld flash.o blinker02.gcc.thumb.o
	$(ARMGNU)-ld -o blinker02.gcc.thumb.flash.elf -T flash.ld flash.o blinker02.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker02.gcc.thumb.flash.elf > blinker02.gcc.thumb.flash.list
	$(ARMGNU)-objcopy blinker02.gcc.thumb.flash.elf blinker02.gcc.thumb.flash.bin -O binary

blinker02.gcc.thumb.sram.bin : sram.ld sram.o blinker02.gcc.thumb.o
	$(ARMGNU)-ld -o blinker02.gcc.thumb.sram.elf -T sram.ld sram.o blinker02.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker02.gcc.thumb.sram.elf > blinker02.gcc.thumb.sram.list
	$(ARMGNU)-objcopy blinker02.gcc.thumb.sram.elf blinker02.gcc.thumb.sram.hex -O ihex
	$(ARMGNU)-objcopy blinker02.gcc.thumb.sram.elf blinker02.gcc.thumb.sram.bin -O binary


