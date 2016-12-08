
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : blinker03.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.hex

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

blinker03.o : blinker03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker03.c -o blinker03.o

blinker03.bin : flash.ld flash.o blinker03.o
	$(ARMGNU)-ld -o blinker03.elf -T flash.ld flash.o blinker03.o
	$(ARMGNU)-objdump -D blinker03.elf > blinker03.list
	$(ARMGNU)-objcopy blinker03.elf blinker03.bin -O binary


