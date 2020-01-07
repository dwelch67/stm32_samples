
ARMGNU = arm-none-eabi

#AOPS = --warn --fatal-warnings -mcpu=cortex-m4
#COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m4

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : notmain4.bin notmain7.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f mkbin

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

flash4.o : flash4.s
	$(ARMGNU)-as $(AOPS) flash4.s -o flash4.o

notmain.o : notmain.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c notmain.c -o notmain.o

notmain4.bin :      flash4.ld flash4.o
	$(ARMGNU)-ld -T flash4.ld flash4.o -o notmain4.elf 
	$(ARMGNU)-objdump -D notmain4.elf > notmain4.list
	$(ARMGNU)-objcopy notmain4.elf notmain4.bin -O binary

notmain7.bin :      flash7.ld flash.o notmain.o
	$(ARMGNU)-ld -T flash7.ld flash.o notmain.o -o notmain7.elf 
	$(ARMGNU)-objdump -D notmain7.elf > notmain7.list
	$(ARMGNU)-objcopy notmain7.elf notmain7.bin -O binary

