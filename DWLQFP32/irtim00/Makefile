
ARMGNU ?= arm-none-eabi
#ARMGNU ?= arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : irtim00.bin

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

irtim00.o : irtim00.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c irtim00.c -o irtim00.o

irtim00.bin : flash.ld flash.o irtim00.o
	$(ARMGNU)-ld -o irtim00.elf -T flash.ld flash.o irtim00.o
	$(ARMGNU)-objdump -D irtim00.elf > irtim00.list
	$(ARMGNU)-objcopy irtim00.elf irtim00.bin -O binary

