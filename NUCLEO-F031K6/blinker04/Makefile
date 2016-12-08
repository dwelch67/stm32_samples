
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : blinker04.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.hex

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

blinker04.gcc.o : blinker04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker04.c -o blinker04.gcc.o

blinker04.bin : flash.ld flash.o blinker04.gcc.o
	$(ARMGNU)-ld -o blinker04.elf -T flash.ld flash.o blinker04.gcc.o
	$(ARMGNU)-objdump -D blinker04.elf > blinker04.list
	$(ARMGNU)-objcopy blinker04.elf blinker04.bin -O binary


