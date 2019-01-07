
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : blinker01.bin


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

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

blinker01.o : blinker01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker01.c -o blinker01.o

blinker01.bin : flash.ld flash.o blinker01.o
	$(ARMGNU)-ld -o blinker01.elf -T flash.ld flash.o blinker01.o
	$(ARMGNU)-objdump -D blinker01.elf > blinker01.list
	$(ARMGNU)-objcopy blinker01.elf blinker01.bin -O binary


