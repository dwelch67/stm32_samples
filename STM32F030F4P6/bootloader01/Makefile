
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

all : notmain.bin testsrec.srec

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.srec

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

notmain.o : notmain.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c notmain.c -o notmain.o

notmain.bin : flash.ld flash.o notmain.o
	$(ARMGNU)-ld -o notmain.elf -T flash.ld flash.o notmain.o
	$(ARMGNU)-objdump -D notmain.elf > notmain.list
	$(ARMGNU)-objcopy notmain.elf notmain.bin -O binary


sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

testsrec.o : testsrec.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c testsrec.c -o testsrec.o

testsrec.srec : sram.ld sram.o testsrec.o
	$(ARMGNU)-ld -o testsrec.elf -T sram.ld sram.o testsrec.o
	$(ARMGNU)-objdump -D testsrec.elf > testsrec.list
	$(ARMGNU)-objcopy --srec-forceS3 testsrec.elf -O srec testsrec.srec

