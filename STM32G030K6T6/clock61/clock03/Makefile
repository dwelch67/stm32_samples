
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : clock03.bin

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

clock03.o : clock03.c twostuff.h
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c clock03.c -o clock03.o

clock03.bin : flash.ld flash.o clock03.o
	$(ARMGNU)-ld -o clock03.elf -T flash.ld flash.o clock03.o
	$(ARMGNU)-objdump -D clock03.elf > clock03.list
	$(ARMGNU)-objcopy clock03.elf clock03.bin -O binary

