
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m3
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : ht1621_clock.thumb.bin ht1621_clock.thumb2.bin

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

ht1621_clock.thumb.o : ht1621_clock.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c ht1621_clock.c -o ht1621_clock.thumb.o

ht1621_clock.thumb2.o : ht1621_clock.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c ht1621_clock.c -o ht1621_clock.thumb2.o

ht1621_clock.thumb.bin : flash.ld flash.o ht1621_clock.thumb.o
	$(ARMGNU)-ld -o ht1621_clock.thumb.elf -T flash.ld flash.o ht1621_clock.thumb.o
	$(ARMGNU)-objdump -D ht1621_clock.thumb.elf > ht1621_clock.thumb.list
	$(ARMGNU)-objcopy ht1621_clock.thumb.elf ht1621_clock.thumb.bin -O binary

ht1621_clock.thumb2.bin : flash.ld flash.o ht1621_clock.thumb2.o
	$(ARMGNU)-ld -o ht1621_clock.thumb2.elf -T flash.ld flash.o ht1621_clock.thumb2.o
	$(ARMGNU)-objdump -D ht1621_clock.thumb2.elf > ht1621_clock.thumb2.list
	$(ARMGNU)-objcopy ht1621_clock.thumb2.elf ht1621_clock.thumb2.bin -O binary

