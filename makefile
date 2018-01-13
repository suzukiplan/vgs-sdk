CC = /usr/bin/gcc

all: bin\
	bin/vgsrom \
	bin/vgsbmp \
	bin/vgswav \
	bin/vgsmml \
	bin/vgsasm

bin:
	mkdir bin

bin/vgsrom: src/vgsrom.c
	$(CC) -o bin/vgsrom src/vgsrom.c

bin/vgsbmp: src/vgsbmp.c
	$(CC) -o bin/vgsbmp src/vgsbmp.c

bin/vgswav: src/vgswav.c
	$(CC) -o bin/vgswav src/vgswav.c

bin/vgsmml:
	cd vgs-mml-compiler && make
	cp vgs-mml-compiler/vgs2mml bin/vgsmml

bin/vgsasm:
	cd vgs-cpu && make build
	cp vgs-cpu/vgsasm bin
	cp vgs-cpu/vgsdrun bin

