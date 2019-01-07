
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : clock04.bin

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

clock04.o : clock04.c twostuff.h
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c clock04.c -o clock04.o

clock04.bin : flash.ld flash.o clock04.o
	$(ARMGNU)-ld -o clock04.elf -T flash.ld flash.o clock04.o
	$(ARMGNU)-objdump -D clock04.elf > clock04.list
	$(ARMGNU)-objcopy clock04.elf clock04.bin -O binary

