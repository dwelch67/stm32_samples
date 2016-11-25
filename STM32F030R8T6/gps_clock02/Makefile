
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all  : gps_clock02.bin 

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.hex

	rm -f donmea

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

gps_clock02.gcc.o : gps_clock02.c mask.h
	$(ARMGNU)-gcc $(COPS) -mthumb -c gps_clock02.c -o gps_clock02.gcc.o

gps_clock02.bin : flash.ld flash.o gps_clock02.gcc.o
	$(ARMGNU)-ld -o gps_clock02.elf -T flash.ld flash.o gps_clock02.gcc.o
	$(ARMGNU)-objdump -D gps_clock02.elf > gps_clock02.list
	$(ARMGNU)-objcopy gps_clock02.elf gps_clock02.bin -O binary

