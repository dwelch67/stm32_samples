
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m3
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : rtc04.thumb.bin rtc04.thumb2.bin

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

rtc04.thumb.o : rtc04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c rtc04.c -o rtc04.thumb.o

rtc04.thumb2.o : rtc04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c rtc04.c -o rtc04.thumb2.o

rtc04.thumb.bin : flash.ld flash.o rtc04.thumb.o
	$(ARMGNU)-ld -o rtc04.thumb.elf -T flash.ld flash.o rtc04.thumb.o
	$(ARMGNU)-objdump -D rtc04.thumb.elf > rtc04.thumb.list
	$(ARMGNU)-objcopy rtc04.thumb.elf rtc04.thumb.bin -O binary

rtc04.thumb2.bin : flash.ld flash.o rtc04.thumb2.o
	$(ARMGNU)-ld -o rtc04.thumb2.elf -T flash.ld flash.o rtc04.thumb2.o
	$(ARMGNU)-objdump -D rtc04.thumb2.elf > rtc04.thumb2.list
	$(ARMGNU)-objcopy rtc04.thumb2.elf rtc04.thumb2.bin -O binary

