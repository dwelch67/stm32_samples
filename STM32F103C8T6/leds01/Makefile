
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m3
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : leds01.thumb.bin leds01.thumb2.bin

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

leds01.thumb.o : leds01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c leds01.c -o leds01.thumb.o

leds01.thumb2.o : leds01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c leds01.c -o leds01.thumb2.o

leds01.thumb.bin : flash.ld flash.o leds01.thumb.o
	$(ARMGNU)-ld -o leds01.thumb.elf -T flash.ld flash.o leds01.thumb.o
	$(ARMGNU)-objdump -D leds01.thumb.elf > leds01.thumb.list
	$(ARMGNU)-objcopy leds01.thumb.elf leds01.thumb.bin -O binary

leds01.thumb2.bin : flash.ld flash.o leds01.thumb2.o
	$(ARMGNU)-ld -o leds01.thumb2.elf -T flash.ld flash.o leds01.thumb2.o
	$(ARMGNU)-objdump -D leds01.thumb2.elf > leds01.thumb2.list
	$(ARMGNU)-objcopy leds01.thumb2.elf leds01.thumb2.bin -O binary

