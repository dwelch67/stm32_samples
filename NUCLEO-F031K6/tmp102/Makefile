
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0


all : tmp102.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.hex

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

tmp102.o : tmp102.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c tmp102.c -o tmp102.o

tmp102.bin : flash.ld flash.o tmp102.o
	$(ARMGNU)-ld -o tmp102.elf -T flash.ld flash.o tmp102.o
	$(ARMGNU)-objdump -D tmp102.elf > tmp102.list
	$(ARMGNU)-objcopy tmp102.elf tmp102.bin -O binary


