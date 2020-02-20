cookie-hunt
===========

![screenshot](https://raw.githubusercontent.com/querry43/gb-cookie-hunt/master/screenshot.png)

This is a classic GameBoy game featuring our favorite robot: Gumdrop.  Help
Gumdrop find web cookies to replenish energy while avoiding obstacles.  Keep
Gumdrop moving as long as you can!

Dependencies
------------
* nvm (https://github.com/nvm-sh/nvm)
* gbdk (https://www.gbdkjs.com/)

Building
--------

Gather the node dependencies:
```
nvm use 12
npm install
```

Build the game:
```
npm run build
```

Running
-------
The ROM builds into src/cookie-hunt.gb .  Run it in your favorite GameBoy
emulator.
