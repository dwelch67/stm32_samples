
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : brself.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

brself.o : brself.s
	$(ARMGNU)-as $(AOPS) brself.s -o brself.o

brself.bin : flash.ld brself.o 
	$(ARMGNU)-ld -o brself.elf -T flash.ld brself.o 
	$(ARMGNU)-objdump -D brself.elf > brself.list
	$(ARMGNU)-objcopy brself.elf brself.bin -O binary

