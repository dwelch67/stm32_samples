
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : clock01.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f buildheader

#---------------------------------


stuff.h : buildheader.c
	gcc buildheader.c -o buildheader
	./buildheader > stuff.h

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

clock01.o : clock01.c stuff.h
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c clock01.c -o clock01.o

clock01.bin : flash.ld flash.o clock01.o
	$(ARMGNU)-ld -o clock01.elf -T flash.ld flash.o clock01.o
	$(ARMGNU)-objdump -D clock01.elf > clock01.list
	$(ARMGNU)-objcopy clock01.elf clock01.bin -O binary

