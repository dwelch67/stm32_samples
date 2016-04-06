
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
#OOPS = -std-compile-opts
OOPS = -std-link-opts

all  : gps_clock02.gcc.thumb.flash.bin 

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f *.hex

	rm -f donmea

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

gps_clock02.gcc.thumb.o : gps_clock02.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c gps_clock02.c -o gps_clock02.gcc.thumb.o

gps_clock02.gcc.thumb.flash.bin : flash.ld flash.o gps_clock02.gcc.thumb.o
	$(ARMGNU)-ld -o gps_clock02.gcc.thumb.flash.elf -T flash.ld flash.o gps_clock02.gcc.thumb.o
	$(ARMGNU)-objdump -D gps_clock02.gcc.thumb.flash.elf > gps_clock02.gcc.thumb.flash.list
	$(ARMGNU)-objcopy gps_clock02.gcc.thumb.flash.elf gps_clock02.gcc.thumb.flash.bin -O binary

