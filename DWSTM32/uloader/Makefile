
progstm : progstm.c ser.c ser.h blinker.bin.h
	gcc progstm.c ser.c -o progstm

blinker.bin.h : bintoc blinker.bin
	./bintoc blinker.bin

bintoc : bintoc.c
	gcc bintoc.c -o bintoc

clean :
	rm -f progstm
	rm -f bintoc

