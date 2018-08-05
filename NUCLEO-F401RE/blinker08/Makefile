
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m4
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m4

gcc : blinker08.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

blinker08.o : blinker08.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker08.c -o blinker08.o

blinker08.thumb2.o : blinker08.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m4 -c blinker08.c -o blinker08.thumb2.o

blinker08.bin : flash.ld flash.o blinker08.o
	$(ARMGNU)-ld -o blinker08.elf -T flash.ld flash.o blinker08.o
	$(ARMGNU)-objdump -D blinker08.elf > blinker08.list
	$(ARMGNU)-objcopy blinker08.elf blinker08.bin -O binary

