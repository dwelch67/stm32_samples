
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

all : ssd1306gps_clock.gcc.thumb.flash.bin ssd1306gps_clock.gcc.thumb.sram.bin


clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bin.s
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f bintoc

#---------------------------------

bintoc : bintoc.c
	gcc bintoc.c -o bintoc

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

ssd1306gps_clock.gcc.thumb.o : ssd1306gps_clock.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c ssd1306gps_clock.c -o ssd1306gps_clock.gcc.thumb.o


ssd1306gps_clock.gcc.thumb.sram.bin : sram.ld sram.o ssd1306gps_clock.gcc.thumb.o
	$(ARMGNU)-ld -o ssd1306gps_clock.gcc.thumb.sram.elf -T sram.ld sram.o ssd1306gps_clock.gcc.thumb.o
	$(ARMGNU)-objdump -D ssd1306gps_clock.gcc.thumb.sram.elf > ssd1306gps_clock.gcc.thumb.sram.list
	$(ARMGNU)-objcopy ssd1306gps_clock.gcc.thumb.sram.elf ssd1306gps_clock.gcc.thumb.sram.bin -O binary



ssd1306gps_clock.gcc.thumb.flash.bin : flash.ld ssd1306gps_clock.gcc.thumb.sram.bin bintoc
	./bintoc ssd1306gps_clock.gcc.thumb.sram.bin
	$(ARMGNU)-as $(AOPS) ssd1306gps_clock.gcc.thumb.sram.bin.s -o ssd1306gps_clock.gcc.thumb.flash.o
	$(ARMGNU)-ld -o ssd1306gps_clock.gcc.thumb.flash.elf -T flash.ld ssd1306gps_clock.gcc.thumb.flash.o
	$(ARMGNU)-objdump -D ssd1306gps_clock.gcc.thumb.flash.elf > ssd1306gps_clock.gcc.thumb.flash.list
	$(ARMGNU)-objcopy ssd1306gps_clock.gcc.thumb.flash.elf ssd1306gps_clock.gcc.thumb.flash.bin -O binary




