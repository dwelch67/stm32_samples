
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
#OOPS = -std-compile-opts
OOPS = -std-link-opts

gcc : notmain.gcc.thumb.flash.bin

all : gcc clang

clang : notmain.clang.thumb.norm.flash.bin notmain.clang.thumb.opt.flash.bin 

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

notmain.gcc.thumb.o : notmain.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c notmain.c -o notmain.gcc.thumb.o

notmain.gcc.thumb.flash.bin : flash.ld flash.o notmain.gcc.thumb.o
	$(ARMGNU)-ld -o notmain.gcc.thumb.flash.elf -T flash.ld flash.o notmain.gcc.thumb.o
	$(ARMGNU)-objdump -D notmain.gcc.thumb.flash.elf > notmain.gcc.thumb.flash.list
	$(ARMGNU)-objcopy notmain.gcc.thumb.flash.elf notmain.gcc.thumb.flash.bin -O binary

#---------------------------------

notmain.clang.bc : notmain.c
	clang $(LOPS) -c notmain.c -o notmain.clang.bc

notmain.clang.thumb.norm.flash.bin : flash.ld flash.o notmain.clang.bc
	#llc $(LLCOPS) notmain.clang.bc -o notmain.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) notmain.clang.thumb.norm.s -o notmain.clang.thumb.norm.o
	llc $(LLCOPS) notmain.clang.bc -filetype=obj -o notmain.clang.thumb.norm.o
	$(ARMGNU)-ld -o notmain.clang.thumb.norm.flash.elf -T flash.ld flash.o notmain.clang.thumb.norm.o
	$(ARMGNU)-objdump -D notmain.clang.thumb.norm.flash.elf > notmain.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy notmain.clang.thumb.norm.flash.elf notmain.clang.thumb.norm.flash.bin -O binary

notmain.clang.thumb.opt.flash.bin : flash.ld flash.o notmain.clang.bc
	opt $(OOPS) notmain.clang.bc -o notmain.clang.thumb.opt.bc
	#llc $(LLCOPS) notmain.clang.thumb.opt.bc -o notmain.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) notmain.clang.thumb.opt.s -o notmain.clang.thumb.opt.o
	llc $(LLCOPS) notmain.clang.thumb.opt.bc -filetype=obj -o notmain.clang.thumb.opt.o
	$(ARMGNU)-ld -o notmain.clang.thumb.opt.flash.elf -T flash.ld flash.o notmain.clang.thumb.opt.o
	$(ARMGNU)-objdump -D notmain.clang.thumb.opt.flash.elf > notmain.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy notmain.clang.thumb.opt.flash.elf notmain.clang.thumb.opt.flash.bin -O binary



