
ARMGNU = arm-eabi

XCPU = -mcpu=cortex-m4

AOPS = --warn --fatal-warnings $(XCPU)
COPS = -Wall -O2 $(XCPU)
LOPS = -nostdlib -nostartfiles

all : notmain.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.ali

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

dummy.o : dummy.adb
	$(ARMGNU)-gcc $(COPS) -mthumb -c dummy.adb -o dummy.o

notmain.o : notmain.adb
	$(ARMGNU)-gcc $(COPS) -mthumb -c notmain.adb -o notmain.o

notmain.bin :               flash.ld flash.o notmain.o dummy.o
	$(ARMGNU)-ld $(LOPS) -T flash.ld flash.o notmain.o dummy.o -o notmain.elf
	$(ARMGNU)-objdump -D notmain.elf > notmain.list
	$(ARMGNU)-objcopy -O binary notmain.elf notmain.bin
