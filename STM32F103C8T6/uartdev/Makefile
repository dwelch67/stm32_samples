
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m3
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 
 -mcpu=cortex-m0

all : uartdev.bin notmain.bin

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

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

putget.o : putget.s
	$(ARMGNU)-as $(AOPS) putget.s -o putget.o

uartdev.o : uartdev.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c uartdev.c -o uartdev.o

uartdev.bin :       flash.ld flash.o putget.o uartdev.o
	$(ARMGNU)-ld -T flash.ld flash.o putget.o uartdev.o -o uartdev.elf
	$(ARMGNU)-objdump -D uartdev.elf > uartdev.list
	$(ARMGNU)-objcopy uartdev.elf uartdev.bin -O binary

notmain.o : notmain.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c notmain.c -o notmain.o

notmain.bin :          sram.ld sram.o putget.o notmain.o
	$(ARMGNU)-ld -T sram.ld sram.o putget.o notmain.o -o notmain.elf
	$(ARMGNU)-objdump -D notmain.elf > notmain.list
	$(ARMGNU)-objcopy notmain.elf notmain.bin -O binary

