CC  = lcc -Wa-l -Wl-m -Wl-j

all: cookie-hunt.gb

cookie-hunt.gb: main.o ../tile-data/*.o

%.gb:
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $<

%.gb: %.o
	$(CC) -o $@ $<

clean:
	rm -f *.o *.lst *.map *.sym *.s *.asm *.ASM *.gb *.sav

.PHONY: clean
