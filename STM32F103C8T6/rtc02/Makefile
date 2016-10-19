
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m3
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : rtc02.thumb.bin rtc02.thumb2.bin

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

rtc02.thumb.o : rtc02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c rtc02.c -o rtc02.thumb.o

rtc02.thumb2.o : rtc02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c rtc02.c -o rtc02.thumb2.o

rtc02.thumb.bin : flash.ld flash.o rtc02.thumb.o
	$(ARMGNU)-ld -o rtc02.thumb.elf -T flash.ld flash.o rtc02.thumb.o
	$(ARMGNU)-objdump -D rtc02.thumb.elf > rtc02.thumb.list
	$(ARMGNU)-objcopy rtc02.thumb.elf rtc02.thumb.bin -O binary

rtc02.thumb2.bin : flash.ld flash.o rtc02.thumb2.o
	$(ARMGNU)-ld -o rtc02.thumb2.elf -T flash.ld flash.o rtc02.thumb2.o
	$(ARMGNU)-objdump -D rtc02.thumb2.elf > rtc02.thumb2.list
	$(ARMGNU)-objcopy rtc02.thumb2.elf rtc02.thumb2.bin -O binary

