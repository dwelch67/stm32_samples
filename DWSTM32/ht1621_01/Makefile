
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding

all : ht1621_01.bin 

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

ht1621_01.o : ht1621_01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c ht1621_01.c -o ht1621_01.o

ht1621_01.bin : flash.ld flash.o ht1621_01.o
	$(ARMGNU)-ld -o ht1621_01.elf -T flash.ld flash.o ht1621_01.o
	$(ARMGNU)-objdump -D ht1621_01.elf > ht1621_01.list
	$(ARMGNU)-objcopy ht1621_01.elf ht1621_01.bin -O binary

