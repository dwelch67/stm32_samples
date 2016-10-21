
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0


all : npix02.flash.bin


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

npix02.o : npix02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c npix02.c -o npix02.o

npix02.flash.bin : flash.ld flash.o npix02.o
	$(ARMGNU)-ld -o npix02.flash.elf -T flash.ld flash.o npix02.o
	$(ARMGNU)-objdump -D npix02.flash.elf > npix02.flash.list
	$(ARMGNU)-objcopy npix02.flash.elf npix02.flash.bin -O binary

