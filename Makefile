CC  = lcc -Wa-l -Wl-m -Wl-j

all: game.gb

game.gb: game.o title.o
	$(CC) -o game.gb $^

%.o: %.c
	$(CC) -c -o $@ $<

%.s: %.c
	$(CC) -S -o $@ $<

%.o: %.s
	$(CC) -c -o $@ $<

%.gb: %.o
	$(CC) -o $@ $<
