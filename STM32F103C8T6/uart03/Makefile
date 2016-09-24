
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m3
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : uart03.thumb.bin uart03.thumb2.bin

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

uart03.thumb.o : uart03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c uart03.c -o uart03.thumb.o

uart03.thumb2.o : uart03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c uart03.c -o uart03.thumb2.o

uart03.thumb.bin : flash.ld flash.o uart03.thumb.o
	$(ARMGNU)-ld -o uart03.thumb.elf -T flash.ld flash.o uart03.thumb.o
	$(ARMGNU)-objdump -D uart03.thumb.elf > uart03.thumb.list
	$(ARMGNU)-objcopy uart03.thumb.elf uart03.thumb.bin -O binary

uart03.thumb2.bin : flash.ld flash.o uart03.thumb2.o
	$(ARMGNU)-ld -o uart03.thumb2.elf -T flash.ld flash.o uart03.thumb2.o
	$(ARMGNU)-objdump -D uart03.thumb2.elf > uart03.thumb2.list
	$(ARMGNU)-objcopy uart03.thumb2.elf uart03.thumb2.bin -O binary

