
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : int04.bin

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

int04.o : int04.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c int04.c -o int04.o

int04.sram.bin : sram.ld sram.o int04.o
	$(ARMGNU)-ld -o int04.sram.elf -T sram.ld sram.o int04.o
	$(ARMGNU)-objdump -D int04.sram.elf > int04.sram.list
	$(ARMGNU)-objcopy int04.sram.elf int04.sram.bin -O binary


copyjump : copyjump.c
	gcc -O2 copyjump.c -o copyjump

int04.bin : int04.sram.bin copyjump flash.ld
	./copyjump int04.sram.bin int04.sram.bin.s
	$(ARMGNU)-as $(AOPS) int04.sram.bin.s -o int04.sram.bin.o
	$(ARMGNU)-ld -o int04.elf -T flash.ld int04.sram.bin.o
	$(ARMGNU)-objdump -D int04.elf > int04.list
	$(ARMGNU)-objcopy int04.elf int04.bin -O binary
	
