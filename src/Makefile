CC  = lcc -Wa-l -Wl-m -Wl-j

all: cookie-hunt.gb

cookie-hunt.gb: game.o main.o menu.o text.o score.o title.o ../tile-data/*.o

%.gb:
	$(CC) -Wl-yt2 -Wl-yo4 -Wl-ya4 -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $<

%.bank1.o: %.c
	$(CC) -Wf-bo1 -Wf-ba1 -c -o $@ $<

%.bank2.o: %.c
	$(CC) -Wf-bo2 -Wf-ba2 -c -o $@ $<

clean:
	rm -f *.o *.lst *.map *.sym *.s *.asm *.ASM *.gb *.sav

.PHONY: clean
