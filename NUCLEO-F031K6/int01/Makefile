
ARMGNU ?= arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0


all : int01.bin 

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

int01.o : int01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c int01.c -o int01.o

int01.bin : flash.ld flash.o int01.o
	$(ARMGNU)-ld -o int01.elf -T flash.ld flash.o int01.o
	$(ARMGNU)-objdump -D int01.elf > int01.list
	$(ARMGNU)-objcopy int01.elf int01.bin -O binary

