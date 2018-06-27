
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : spi04.bin

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

spi04.o : spi04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c spi04.c -o spi04.o

spi04.bin : flash.ld flash.o spi04.o
	$(ARMGNU)-ld -o spi04.elf -T flash.ld flash.o spi04.o
	$(ARMGNU)-objdump -D spi04.elf > spi04.list
	$(ARMGNU)-objcopy spi04.elf spi04.bin -O binary

