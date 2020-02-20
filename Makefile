all: src

tile-data:
	$(MAKE) -C tile-data

src: tile-data
	$(MAKE) -C src

run:
	open src/cookie-hunt.gb

clean:
	$(MAKE) -C tile-data clean
	$(MAKE) -C src clean

distclean: clean
	$(MAKE) -C tile-data distclean

.PHONY: tile-data src
