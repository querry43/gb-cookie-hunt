CC  = lcc -Wa-l -Wl-m -Wl-j
GGBGFX = ../node_modules/.bin/ggbgfx

# white should come first
images = \
	../images/white.png \
	../images/black.png \
	../images/cookie.png \
	../images/hud.png \
	../images/background.png

sprites = \
	../images/robot.sprite.png

all: \
	tileset.o \
	white.map.o \
	black.map.o \
	cookie.map.o \
	hud.map.o \
	background.map.o \
	robot.sprite.o

tileset.png: $(images)
	$(GGBGFX) tileset -o $@ $^

tileset.c: tileset.png
	$(GGBGFX) tiledata -o $@ $<

# offset maps by 38 (the size of the font)
%.map.c: ../images/%.png tileset.png
	 $(GGBGFX) tilemap --offset 38 --name $*_map -o $@ $^

%.sprite.c: ../images/%.sprite.png
	$(GGBGFX) sprite --name  $*_sprite -o $@ $<

%.o: %.c
	$(CC) -c -o $@ $<

clean:
	rm -f *.o *.lst *.map *.sym *.s *.asm

distclean: clean
	rm -f *.c tileset.png

.PHONY: clean distclean
.PRECIOUS: %.map.c %.sprite.c
