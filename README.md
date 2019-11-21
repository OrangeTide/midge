# Midge: a C framework for writing OpenGL applications

This library creates a GL context and manages a platform specific window for your application. It provides functionality similar to [freeGLUT](http://freeglut.sourceforge.net/), [GLFW](http://www.glfw.org/), [SDL](https://www.libsdl.org/), and [raylib](http://www.raylib.com/). And is inferior to those libraries in nearly every way.

## Table of Contents

- [Building](#installation)
- [Usage](#usage)
- [Support](#support)
- [Contributing](#contributing)

## Building

### Build requirements

Linux & FreeBSD: X11 and GL libraries

### Check-out and Build from source

```sh
git clone git://github.com/OrangeTide/midge
cd midge
make
```

Output is a static library (`libmidge.a`) and some demos.

## Usage

Link your own project to `libmidge.a`, using the demos as a reference.

Entry point into your application is `midge_app_start()`. From register handlers for each event type you intend to support. This start handler is called after a default window and context is created.

Optionally your application may define the entry point `midge_app_configure()`. This configure handler is called instead of creating a default window and context. It is necessary to call `midge_window_new()` one or more times before leaving this function.

It is recommend that your application includes `midge-main.h` rather than defining its own `main()` entry point. Additional information on writing your own `main()` is documented in `midge.h` and `midge-main.h`.

```c
/* yourapp.c
 * build with: gcc -Ipath/to/midge yourapp.c -Lpath/to/midge -lmidge -lmidge -lGL -lX11 -o yourapp
 */
#include <midge.h>
#include <midge-main.h>
void midge_app_start(void) {
    /* register all your handlers here */
}
```

## Support

Please [open an issue](https://github.com/OrangeTide/midge-boilerplate/issues/new) for support.

## Contributing

Please contribute using [Github Flow](https://guides.github.com/introduction/flow/). Create a branch, add commits, and [open a pull request](https://github.com/fraction/readme-boilerplate/compare/).
