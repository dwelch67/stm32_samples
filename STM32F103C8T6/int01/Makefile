
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=cortex-m0 -march=armv6-m
AOPSx = --warn --fatal-warnings -mcpu=cortex-m3
COPSx = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=cortex-m3 -march=armv7-m

all : notmain.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

putget.o : putget.s
	$(ARMGNU)-as $(AOPS) putget.s -o putget.o

notmain.o : notmain.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c notmain.c -o notmain.o

notmain.bin :       flash.ld flash.o putget.o notmain.o
	$(ARMGNU)-ld -T flash.ld flash.o putget.o notmain.o -o notmain.elf
	$(ARMGNU)-objdump -D notmain.elf > notmain.list
	$(ARMGNU)-objcopy notmain.elf notmain.bin -O binary


