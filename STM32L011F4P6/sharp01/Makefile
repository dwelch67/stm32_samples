
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : notmain.bin

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

notmain.o : notmain.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c notmain.c -o notmain.o

notmain.sram.bin : sram.ld sram.o notmain.o
	$(ARMGNU)-ld -o notmain.sram.elf -T sram.ld sram.o notmain.o
	$(ARMGNU)-objdump -D notmain.sram.elf > notmain.sram.list
	$(ARMGNU)-objcopy notmain.sram.elf notmain.sram.bin -O binary


copyjump : copyjump.c
	gcc -O2 copyjump.c -o copyjump

notmain.bin : notmain.sram.bin copyjump flash.ld
	./copyjump notmain.sram.bin notmain.sram.bin.s
	$(ARMGNU)-as $(AOPS) notmain.sram.bin.s -o notmain.sram.bin.o
	$(ARMGNU)-ld -o notmain.elf -T flash.ld notmain.sram.bin.o
	$(ARMGNU)-objdump -D notmain.elf > notmain.list
	$(ARMGNU)-objcopy notmain.elf notmain.bin -O binary
	
