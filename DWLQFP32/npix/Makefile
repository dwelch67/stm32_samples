
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0


all : npix.flash.bin


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

npix.o : npix.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c npix.c -o npix.o

npix.flash.bin : flash.ld flash.o npix.o
	$(ARMGNU)-ld -o npix.flash.elf -T flash.ld flash.o npix.o
	$(ARMGNU)-objdump -D npix.flash.elf > npix.flash.list
	$(ARMGNU)-objcopy npix.flash.elf npix.flash.bin -O binary

