
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : spi05.bin

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

spi05.o : spi05.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c spi05.c -o spi05.o

spi05.bin : flash.ld flash.o spi05.o
	$(ARMGNU)-ld -o spi05.elf -T flash.ld flash.o spi05.o
	$(ARMGNU)-objdump -D spi05.elf > spi05.list
	$(ARMGNU)-objcopy spi05.elf spi05.bin -O binary

