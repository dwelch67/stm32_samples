
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : blinker02.bin seven

seven : seven.c
	gcc -O2 seven.c -o seven


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f seven

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

blinker02.o : blinker02.c mask.h
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker02.c -o blinker02.o

blinker02.bin : flash.ld flash.o blinker02.o
	$(ARMGNU)-ld -o blinker02.elf -T flash.ld flash.o blinker02.o
	$(ARMGNU)-objdump -D blinker02.elf > blinker02.list
	$(ARMGNU)-objcopy blinker02.elf blinker02.bin -O binary

