
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0


all : rtc_npix02.flash.bin


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

rtc_npix02.o : rtc_npix02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c rtc_npix02.c -o rtc_npix02.o

rtc_npix02.flash.bin : flash.ld flash.o rtc_npix02.o
	$(ARMGNU)-ld -o rtc_npix02.flash.elf -T flash.ld flash.o rtc_npix02.o
	$(ARMGNU)-objdump -D rtc_npix02.flash.elf > rtc_npix02.flash.list
	$(ARMGNU)-objcopy rtc_npix02.flash.elf rtc_npix02.flash.bin -O binary

