
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m3
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : wip.thumb.bin wip.thumb2.bin

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

wip.thumb.o : wip.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c wip.c -o wip.thumb.o

wip.thumb2.o : wip.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c wip.c -o wip.thumb2.o

wip.thumb.bin : flash.ld flash.o wip.thumb.o
	$(ARMGNU)-ld -o wip.thumb.elf -T flash.ld flash.o wip.thumb.o
	$(ARMGNU)-objdump -D wip.thumb.elf > wip.thumb.list
	$(ARMGNU)-objcopy wip.thumb.elf wip.thumb.bin -O binary

wip.thumb2.bin : flash.ld flash.o wip.thumb2.o
	$(ARMGNU)-ld -o wip.thumb2.elf -T flash.ld flash.o wip.thumb2.o
	$(ARMGNU)-objdump -D wip.thumb2.elf > wip.thumb2.list
	$(ARMGNU)-objcopy wip.thumb2.elf wip.thumb2.bin -O binary

