
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : npix.sram.bin npix.flash.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.hex
	rm -f *.bin.s
	rm -f *.bin.o
	rm -f cpjump

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

npix.o : npix.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c npix.c -o npix.o

cpjump : cpjump.c
	gcc -O2 cpjump.c -o cpjump

npix.flash.bin : flash.ld npix.sram.bin cpjump
	./cpjump npix.sram.bin
	$(ARMGNU)-as $(AOPS) npix.sram.bin.s -o npix.flash.bin.o
	$(ARMGNU)-ld -o npix.flash.elf -T flash.ld npix.flash.bin.o
	$(ARMGNU)-objdump -D npix.flash.elf > npix.flash.list
	$(ARMGNU)-objcopy npix.flash.elf npix.flash.bin -O binary

npix.sram.bin : sram.ld sram.o npix.o
	$(ARMGNU)-ld -o npix.sram.elf -T sram.ld sram.o npix.o
	$(ARMGNU)-objdump -D npix.sram.elf > npix.sram.list
	$(ARMGNU)-objcopy npix.sram.elf npix.sram.hex -O ihex
	$(ARMGNU)-objcopy npix.sram.elf npix.sram.bin -O binary

#---------------------------------

