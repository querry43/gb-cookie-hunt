CC  = lcc -Wa-l -Wl-m -Wl-j
GGBGFX = ../node_modules/.bin/ggbgfx

game_images = \
	../images/background.png \
	../images/black.png \
	../images/cookie.png \
	../images/hud.png \
	../images/white.png

title_images = \
	../images/title.png

menu_images = \
	../images/menu.png \
	../images/score.png

game_sprites = \
	../images/robot.sprite.png

all: \
	tileset.game.o \
	tileset.menu.o \
	tileset.title.o \
	\
	background.game.map.o \
	cookie.game.map.o \
	hud.game.map.o \
	\
	robot.game.sprite.o \
	\
	menu.menu.map.o \
	score.menu.map.o \
	white.menu.map.o \
	\
	title.title.map.o

tileset.game.png: $(game_images)

tileset.menu.png: $(menu_images)

tileset.title.png: $(title_images)

tileset.%.png:
	$(GGBGFX) tileset -o $@ $^

tileset.%.c: tileset.%.png
	$(GGBGFX) tiledata --name $*_tileset -o $@ $<

# offset game maps by 38 (the size of the font)
%.game.map.c: ../images/%.png tileset.game.png
	$(GGBGFX) tilemap --offset 38 --name $*_game_map -o $@ $^

%.menu.map.c: ../images/%.png tileset.menu.png
	$(GGBGFX) tilemap --name $*_menu_map -o $@ $^

%.title.map.c: ../images/%.png tileset.title.png
	$(GGBGFX) tilemap --name $*_title_map -o $@ $^

%.game.sprite.c: ../images/%.sprite.png
	$(GGBGFX) sprite --name  $*_game_sprite -o $@ $<

%.o: %.c
	$(CC) -c -o $@ $<

clean:
	rm -f *.o *.lst *.map *.sym *.s *.asm

distclean: clean
	rm -f *.c tileset.*.png

.PHONY: clean distclean
.PRECIOUS: tileset.%.c %.game.map.c %.menu.map.c %.title.map.c %.game.sprite.c
