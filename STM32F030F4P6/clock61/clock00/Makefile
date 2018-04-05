
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m3
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : clock00.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f *.hex

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

clock00.o : clock00.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c clock00.c -o clock00.o

clock00.bin : flash.ld flash.o clock00.o
	$(ARMGNU)-ld -o clock00.elf -T flash.ld flash.o clock00.o
	$(ARMGNU)-objdump -D clock00.elf > clock00.list
	$(ARMGNU)-objcopy clock00.elf clock00.bin -O binary

