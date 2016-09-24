
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
#at some point they changed this option, gee thanks
#OOPS = -std-compile-opts
OOPS = -std-link-opts

all : test.bin

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

test.gcc.thumb.o : test.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c test.c -o test.gcc.thumb.o

test.bin : flash.ld flash.o test.gcc.thumb.o
	$(ARMGNU)-ld -o test.gcc.thumb.flash.elf -T flash.ld flash.o test.gcc.thumb.o
	$(ARMGNU)-objdump -D test.gcc.thumb.flash.elf > test.gcc.thumb.flash.list
	$(ARMGNU)-objcopy test.gcc.thumb.flash.elf test.bin -O binary

