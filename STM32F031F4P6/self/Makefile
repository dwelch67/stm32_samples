
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi-gcc

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

gcc : self.gcc.thumb.flash.bin self.gcc.thumb.sram.bin

all : gcc

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

self.gcc.thumb.flash.bin : flash.ld flash.o 
	$(ARMGNU)-ld -o self.gcc.thumb.flash.elf -T flash.ld flash.o
	$(ARMGNU)-objdump -D self.gcc.thumb.flash.elf > self.gcc.thumb.flash.list
	$(ARMGNU)-objcopy self.gcc.thumb.flash.elf self.gcc.thumb.flash.bin -O binary

self.gcc.thumb.sram.bin : sram.ld sram.o 
	$(ARMGNU)-ld -o self.gcc.thumb.sram.elf -T sram.ld sram.o
	$(ARMGNU)-objdump -D self.gcc.thumb.sram.elf > self.gcc.thumb.sram.list
	$(ARMGNU)-objcopy self.gcc.thumb.sram.elf self.gcc.thumb.sram.bin -O binary

#---------------------------------

