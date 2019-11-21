#OPTIMIZE := -O3 -flto
all :: libmidge.a
clean :: ; $(RM) libmidge.a midge.o notify.o drv-x11.o
libmidge.a : libmidge.a(midge.o notify.o drv-x11.o)
PKGS.libmidge.a := gl x11
CFLAGS.libmidge.a := $(shell pkg-config --cflags $(PKGS.libmidge.a))
LDFLAGS.libmidge.a := $(shell pkg-config --libs $(PKGS.libmidge.a))
libmidge.a : CFLAGS = -Wall -W -g $(OPTIMIZE) $(CFLAGS.libmidge.a)
#
all :: demo1
clean :: ; $(RM) demo1 demo1.o
demo1 : demo1.o -lmidge
demo1 : CFLAGS = -Wall -W -g $(OPTIMIZE) $(CFLAGS.libmidge.a)
demo1 : LDFLAGS = $(LDFLAGS.libmidge.a)
#
all :: demo2
clean :: ; $(RM) demo2 demo2.o
demo2 : demo2.o -lmidge
demo2 : CFLAGS = -Wall -W -g $(OPTIMIZE) $(CFLAGS.libmidge.a)
demo2 : LDFLAGS = $(LDFLAGS.libmidge.a)
