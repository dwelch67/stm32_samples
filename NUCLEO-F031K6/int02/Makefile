
ARMGNU ?= arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0


all : int02.bin 

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

int02.o : int02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c int02.c -o int02.o

int02.bin : flash.ld flash.o int02.o
	$(ARMGNU)-ld -o int02.elf -T flash.ld flash.o int02.o
	$(ARMGNU)-objdump -D int02.elf > int02.list
	$(ARMGNU)-objcopy int02.elf int02.bin -O binary

