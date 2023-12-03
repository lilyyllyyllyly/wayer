# wayer
bare minimum wayland compositor that doesnt use wlroots.

... and when i say *bare minimum* i mean that. This can run barely anything.
One of the few applications that do work is [hello-wayer](https://github.com/lilyyllyyllyly/hello-wayer),
a modified version of [hello-wayland](https://github.com/emersion/hello-wayland) i made specifically for this,
which doesn't use xdg_shell because i couldn't get it to work. Also, it renders windows using raylib, so  you
need a real compositor running in the background to run this, making it completely useless.

so uh, if you wanna test it out you can clone this repo and run `make final`, or `make debug` if you wanna see all the memory
leaks with Address Sanitizer. You should probably also clone [hello-wayer](https://github.com/lilyyllyyllyly/hello-wayer)
because there's probably no app out there that would run at all.

Have fun :)
