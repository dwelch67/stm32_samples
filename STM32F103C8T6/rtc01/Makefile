
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m3
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : rtc01.thumb.bin rtc01.thumb2.bin

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

rtc01.thumb.o : rtc01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c rtc01.c -o rtc01.thumb.o

rtc01.thumb2.o : rtc01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c rtc01.c -o rtc01.thumb2.o

rtc01.thumb.bin : flash.ld flash.o rtc01.thumb.o
	$(ARMGNU)-ld -o rtc01.thumb.elf -T flash.ld flash.o rtc01.thumb.o
	$(ARMGNU)-objdump -D rtc01.thumb.elf > rtc01.thumb.list
	$(ARMGNU)-objcopy rtc01.thumb.elf rtc01.thumb.bin -O binary

rtc01.thumb2.bin : flash.ld flash.o rtc01.thumb2.o
	$(ARMGNU)-ld -o rtc01.thumb2.elf -T flash.ld flash.o rtc01.thumb2.o
	$(ARMGNU)-objdump -D rtc01.thumb2.elf > rtc01.thumb2.list
	$(ARMGNU)-objcopy rtc01.thumb2.elf rtc01.thumb2.bin -O binary

