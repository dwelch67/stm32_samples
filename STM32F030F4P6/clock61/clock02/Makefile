
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : clock02.bin

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

clock02.o : clock02.c twostuff.h
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c clock02.c -o clock02.o

clock02.bin : flash.ld flash.o clock02.o
	$(ARMGNU)-ld -o clock02.elf -T flash.ld flash.o clock02.o
	$(ARMGNU)-objdump -D clock02.elf > clock02.list
	$(ARMGNU)-objcopy clock02.elf clock02.bin -O binary

