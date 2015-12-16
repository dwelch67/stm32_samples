
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : nordicRX.gcc.thumb.flash.bin nordicTX.gcc.thumb.flash.bin  

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

nordicRX.gcc.thumb.o : nordicRX.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c nordicRX.c -o nordicRX.gcc.thumb.o

nordicRX.gcc.thumb.flash.bin : flash.ld flash.o nordicRX.gcc.thumb.o
	$(ARMGNU)-ld -o nordicRX.gcc.thumb.flash.elf -T flash.ld flash.o nordicRX.gcc.thumb.o
	$(ARMGNU)-objdump -D nordicRX.gcc.thumb.flash.elf > nordicRX.gcc.thumb.flash.list
	$(ARMGNU)-objcopy nordicRX.gcc.thumb.flash.elf nordicRX.gcc.thumb.flash.bin -O binary

nordicTX.gcc.thumb.o : nordicTX.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c nordicTX.c -o nordicTX.gcc.thumb.o

nordicTX.gcc.thumb.flash.bin : flash.ld flash.o nordicTX.gcc.thumb.o
	$(ARMGNU)-ld -o nordicTX.gcc.thumb.flash.elf -T flash.ld flash.o nordicTX.gcc.thumb.o
	$(ARMGNU)-objdump -D nordicTX.gcc.thumb.flash.elf > nordicTX.gcc.thumb.flash.list
	$(ARMGNU)-objcopy nordicTX.gcc.thumb.flash.elf nordicTX.gcc.thumb.flash.bin -O binary


