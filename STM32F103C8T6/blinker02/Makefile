
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m3
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : blinker02.thumb.bin blinker02.thumb2.bin

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

blinker02.thumb.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c blinker02.c -o blinker02.thumb.o

blinker02.thumb2.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c blinker02.c -o blinker02.thumb2.o

blinker02.thumb.bin : flash.ld flash.o blinker02.thumb.o
	$(ARMGNU)-ld -o blinker02.thumb.elf -T flash.ld flash.o blinker02.thumb.o
	$(ARMGNU)-objdump -D blinker02.thumb.elf > blinker02.thumb.list
	$(ARMGNU)-objcopy blinker02.thumb.elf blinker02.thumb.bin -O binary

blinker02.thumb2.bin : flash.ld flash.o blinker02.thumb2.o
	$(ARMGNU)-ld -o blinker02.thumb2.elf -T flash.ld flash.o blinker02.thumb2.o
	$(ARMGNU)-objdump -D blinker02.thumb2.elf > blinker02.thumb2.list
	$(ARMGNU)-objcopy blinker02.thumb2.elf blinker02.thumb2.bin -O binary

