
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0

all : flash.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f *.hex

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

flash.bin : flash.ld flash.o
	$(ARMGNU)-ld -o flash.elf -T flash.ld flash.o
	$(ARMGNU)-objdump -D flash.elf > flash.list
	$(ARMGNU)-objcopy flash.elf flash.bin -O binary
