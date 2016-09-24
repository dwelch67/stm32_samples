
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m3
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : blinker03.thumb.bin blinker03.thumb2.bin

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

blinker03.thumb.o : blinker03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c blinker03.c -o blinker03.thumb.o

blinker03.thumb2.o : blinker03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c blinker03.c -o blinker03.thumb2.o

blinker03.thumb.bin : flash.ld flash.o blinker03.thumb.o
	$(ARMGNU)-ld -o blinker03.thumb.elf -T flash.ld flash.o blinker03.thumb.o
	$(ARMGNU)-objdump -D blinker03.thumb.elf > blinker03.thumb.list
	$(ARMGNU)-objcopy blinker03.thumb.elf blinker03.thumb.bin -O binary

blinker03.thumb2.bin : flash.ld flash.o blinker03.thumb2.o
	$(ARMGNU)-ld -o blinker03.thumb2.elf -T flash.ld flash.o blinker03.thumb2.o
	$(ARMGNU)-objdump -D blinker03.thumb2.elf > blinker03.thumb2.list
	$(ARMGNU)-objcopy blinker03.thumb2.elf blinker03.thumb2.bin -O binary

