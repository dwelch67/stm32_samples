
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : test.bin

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

test.thumb.o : test.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c test.c -o test.thumb.o

test.bin : flash.ld flash.o test.thumb.o
	$(ARMGNU)-ld -o test.thumb.elf -T flash.ld flash.o test.thumb.o
	$(ARMGNU)-objdump -D test.thumb.elf > test.thumb.list
	$(ARMGNU)-objcopy test.thumb.elf test.bin -O binary

