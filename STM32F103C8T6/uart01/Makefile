
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m3
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : uart01.thumb.bin uart01.thumb2.bin

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

uart01.thumb.o : uart01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c uart01.c -o uart01.thumb.o

uart01.thumb2.o : uart01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c uart01.c -o uart01.thumb2.o

uart01.thumb.bin : flash.ld flash.o uart01.thumb.o
	$(ARMGNU)-ld -o uart01.thumb.elf -T flash.ld flash.o uart01.thumb.o
	$(ARMGNU)-objdump -D uart01.thumb.elf > uart01.thumb.list
	$(ARMGNU)-objcopy uart01.thumb.elf uart01.thumb.bin -O binary

uart01.thumb2.bin : flash.ld flash.o uart01.thumb2.o
	$(ARMGNU)-ld -o uart01.thumb2.elf -T flash.ld flash.o uart01.thumb2.o
	$(ARMGNU)-objdump -D uart01.thumb2.elf > uart01.thumb2.list
	$(ARMGNU)-objcopy uart01.thumb2.elf uart01.thumb2.bin -O binary

