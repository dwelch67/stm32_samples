
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : int03.bin

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

int03.o : int03.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c int03.c -o int03.o

int03.sram.bin : sram.ld sram.o int03.o
	$(ARMGNU)-ld -o int03.sram.elf -T sram.ld sram.o int03.o
	$(ARMGNU)-objdump -D int03.sram.elf > int03.sram.list
	$(ARMGNU)-objcopy int03.sram.elf int03.sram.bin -O binary


copyjump : copyjump.c
	gcc -O2 copyjump.c -o copyjump

int03.bin : int03.sram.bin copyjump flash.ld
	./copyjump int03.sram.bin int03.sram.bin.s
	$(ARMGNU)-as $(AOPS) int03.sram.bin.s -o int03.sram.bin.o
	$(ARMGNU)-ld -o int03.elf -T flash.ld int03.sram.bin.o
	$(ARMGNU)-objdump -D int03.elf > int03.list
	$(ARMGNU)-objcopy int03.elf int03.bin -O binary
	
