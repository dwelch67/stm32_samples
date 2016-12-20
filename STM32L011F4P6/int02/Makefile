
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : int02.bin

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

int02.o : int02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c int02.c -o int02.o

int02.sram.bin : sram.ld sram.o int02.o
	$(ARMGNU)-ld -o int02.sram.elf -T sram.ld sram.o int02.o
	$(ARMGNU)-objdump -D int02.sram.elf > int02.sram.list
	$(ARMGNU)-objcopy int02.sram.elf int02.sram.bin -O binary


copyjump : copyjump.c
	gcc -O2 copyjump.c -o copyjump

int02.bin : int02.sram.bin copyjump flash.ld
	./copyjump int02.sram.bin int02.sram.bin.s
	$(ARMGNU)-as $(AOPS) int02.sram.bin.s -o int02.sram.bin.o
	$(ARMGNU)-ld -o int02.elf -T flash.ld int02.sram.bin.o
	$(ARMGNU)-objdump -D int02.elf > int02.list
	$(ARMGNU)-objcopy int02.elf int02.bin -O binary
	
