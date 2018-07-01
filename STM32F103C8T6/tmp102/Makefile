
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings 
 -mcpu=cortex-m3
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : tmp102.thumb.bin tmp102.thumb.dfu.bin tmp102.thumb2.bin tmp102.thumb2.dfu.bin

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

tmp102.thumb.o : tmp102.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c tmp102.c -o tmp102.thumb.o

tmp102.thumb2.o : tmp102.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c tmp102.c -o tmp102.thumb2.o

tmp102.thumb.bin : flash.ld flash.o tmp102.thumb.o
	$(ARMGNU)-ld -o tmp102.thumb.elf -T flash.ld flash.o tmp102.thumb.o
	$(ARMGNU)-objdump -D tmp102.thumb.elf > tmp102.thumb.list
	$(ARMGNU)-objcopy tmp102.thumb.elf tmp102.thumb.bin -O binary

tmp102.thumb2.bin : flash.ld flash.o tmp102.thumb2.o
	$(ARMGNU)-ld -o tmp102.thumb2.elf -T flash.ld flash.o tmp102.thumb2.o
	$(ARMGNU)-objdump -D tmp102.thumb2.elf > tmp102.thumb2.list
	$(ARMGNU)-objcopy tmp102.thumb2.elf tmp102.thumb2.bin -O binary


tmp102.thumb.dfu.bin : flashdfu.ld flash.o tmp102.thumb.o
	$(ARMGNU)-ld -o tmp102.thumb.dfu.elf -T flashdfu.ld flash.o tmp102.thumb.o
	$(ARMGNU)-objdump -D tmp102.thumb.dfu.elf > tmp102.thumb.dfu.list
	$(ARMGNU)-objcopy tmp102.thumb.dfu.elf tmp102.thumb.dfu.bin -O binary

tmp102.thumb2.dfu.bin : flashdfu.ld flash.o tmp102.thumb2.o
	$(ARMGNU)-ld -o tmp102.thumb2.dfu.elf -T flashdfu.ld flash.o tmp102.thumb2.o
	$(ARMGNU)-objdump -D tmp102.thumb2.dfu.elf > tmp102.thumb2.dfu.list
	$(ARMGNU)-objcopy tmp102.thumb2.dfu.elf tmp102.thumb2.dfu.bin -O binary



