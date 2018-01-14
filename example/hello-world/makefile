ASM = ../../bin/vgsasm
ROM = ../../bin/vgsrom

all: HELLO.ROM

HELLO.ROM: PSLOT000.BIN
	$(ROM) . HELLO.ROM	

PSLOT000.BIN: PSLOT000.asm
	$(ASM) -o PSLOT000.BIN PSLOT000.asm

