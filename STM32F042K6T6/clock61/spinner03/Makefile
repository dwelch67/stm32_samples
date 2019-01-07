
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : spinner03.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f buildheader

#---------------------------------


twostuff.h : buildheader.c
	gcc buildheader.c -o buildheader
	./buildheader > twostuff.h

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

spinner03.o : spinner03.c twostuff.h
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c spinner03.c -o spinner03.o

spinner03.bin : flash.ld flash.o spinner03.o
	$(ARMGNU)-ld -o spinner03.elf -T flash.ld flash.o spinner03.o
	$(ARMGNU)-objdump -D spinner03.elf > spinner03.list
	$(ARMGNU)-objcopy spinner03.elf spinner03.bin -O binary

