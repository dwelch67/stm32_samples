
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
 -mcpu=cortex-m3
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m3

all : adc01.thumb.bin adc01.thumb2.bin

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

adc01.thumb.o : adc01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c adc01.c -o adc01.thumb.o

adc01.thumb2.o : adc01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c adc01.c -o adc01.thumb2.o

adc01.thumb.bin : flash.ld flash.o adc01.thumb.o
	$(ARMGNU)-ld -o adc01.thumb.elf -T flash.ld flash.o adc01.thumb.o
	$(ARMGNU)-objdump -D adc01.thumb.elf > adc01.thumb.list
	$(ARMGNU)-objcopy adc01.thumb.elf adc01.thumb.bin -O binary

adc01.thumb2.bin : flash.ld flash.o adc01.thumb2.o
	$(ARMGNU)-ld -o adc01.thumb2.elf -T flash.ld flash.o adc01.thumb2.o
	$(ARMGNU)-objdump -D adc01.thumb2.elf > adc01.thumb2.list
	$(ARMGNU)-objcopy adc01.thumb2.elf adc01.thumb2.bin -O binary
