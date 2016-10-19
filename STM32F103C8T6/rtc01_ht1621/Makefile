
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : rtc01_ht1621.bin 

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

rtc01_ht1621.o : rtc01_ht1621.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c rtc01_ht1621.c -o rtc01_ht1621.o

rtc01_ht1621.bin : flash.ld flash.o rtc01_ht1621.o 
	$(ARMGNU)-ld -o rtc01_ht1621.elf -T flash.ld flash.o rtc01_ht1621.o 
	$(ARMGNU)-objdump -D rtc01_ht1621.elf > rtc01_ht1621.list
	$(ARMGNU)-objcopy rtc01_ht1621.elf rtc01_ht1621.bin -O binary

