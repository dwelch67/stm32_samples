
ARMGNU ?= arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0


all : int02.gcc.thumb.flash.bin 

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

int02.gcc.thumb.o : int02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c int02.c -o int02.gcc.thumb.o

int02.gcc.thumb.flash.bin : flash.ld flash.o int02.gcc.thumb.o
	$(ARMGNU)-ld -o int02.gcc.thumb.flash.elf -T flash.ld flash.o int02.gcc.thumb.o
	$(ARMGNU)-objdump -D int02.gcc.thumb.flash.elf > int02.gcc.thumb.flash.list
	$(ARMGNU)-objcopy int02.gcc.thumb.flash.elf int02.gcc.thumb.flash.bin -O binary

