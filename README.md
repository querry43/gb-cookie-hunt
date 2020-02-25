cookie-hunt
===========

![screenshots](https://raw.githubusercontent.com/querry43/gb-cookie-hunt/master/screenshots.gif)

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

TODO
----
- [x] Initial title screen
- [x] Initial level select screen
- [x] Initial score screen
- [x] Initial energy meter
- [x] Initial sound
- [x] Level 1, just cookies
- [ ] Level 2, cookies and barriers
- [ ] Level 3, cookies, barriers, and enemies
- [ ] Improve assets
- [ ] Skip text on level select
- [ ] Remember level on level select

Bugs
----
- [ ] Overlapping cookies
- [ ] Split cookies (on edge of background?)
- [ ] Background tiles on the HUD
