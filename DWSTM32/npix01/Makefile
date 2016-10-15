
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0


all : npix01.flash.bin


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

npix01.o : npix01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c npix01.c -o npix01.o

npix01.flash.bin : flash.ld flash.o npix01.o
	$(ARMGNU)-ld -o npix01.flash.elf -T flash.ld flash.o npix01.o
	$(ARMGNU)-objdump -D npix01.flash.elf > npix01.flash.list
	$(ARMGNU)-objcopy npix01.flash.elf npix01.flash.bin -O binary

