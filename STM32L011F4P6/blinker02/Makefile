
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : blinker02.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f copyjump
	rm -f *.bin.s

#---------------------------------

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

blinker02.o : blinker02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker02.c -o blinker02.o

blinker02.sram.bin : sram.ld sram.o blinker02.o
	$(ARMGNU)-ld -o blinker02.sram.elf -T sram.ld sram.o blinker02.o
	$(ARMGNU)-objdump -D blinker02.sram.elf > blinker02.sram.list
	$(ARMGNU)-objcopy blinker02.sram.elf blinker02.sram.bin -O binary


copyjump : copyjump.c
	gcc -O2 copyjump.c -o copyjump

blinker02.bin : blinker02.sram.bin copyjump flash.ld
	./copyjump blinker02.sram.bin blinker02.sram.bin.s
	$(ARMGNU)-as $(AOPS) blinker02.sram.bin.s -o blinker02.sram.bin.o
	$(ARMGNU)-ld -o blinker02.elf -T flash.ld blinker02.sram.bin.o
	$(ARMGNU)-objdump -D blinker02.elf > blinker02.list
	$(ARMGNU)-objcopy blinker02.elf blinker02.bin -O binary
	
