# Midge: a C framework for writing OpenGL applications

This library creates a GL context and manages a platform specific window for your application. It provides functionality similar to [freeGLUT](http://freeglut.sourceforge.net/), [GLFW](http://www.glfw.org/), [SDL](https://www.libsdl.org/), and [raylib](http://www.raylib.com/). And is inferior to those libraries in nearly every way.

## Table of Contents

- [Building](#building)
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

## License

```
Copyright (c) 2019, Jon Mayo
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the Midge project.
```
