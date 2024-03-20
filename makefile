#
# use `make TARGET=gcw0` to build for GCW-Zero
# use `make TARGET=pandora` to build for Pandora
# use `make TARGET=pyra` to build for DragonBox Pyra
# and `make` for normal build
#

#TARGET	= gcw0

NAME	= griels

CFLAGS	= -s -O2 `sdl2-config --cflags`
CLIBS	= `sdl2-config --libs` -lSDL2_image -lSDL2_mixer -lm

ifdef RUTAS_RELATIVAS
	CFLAGS += -D_RUTAS_RELATIVAS
endif

# assume cross-compilation
ifeq "$(TARGET)" "gcw0"
    CC		= mipsel-linux-gcc
    PFLAGS	= -mips32 -D_GCW_ZERO
endif
ifeq "$(TARGET)" "pandora"
    CC		= pandora-gcc
    PFLAGS	= -march=armv7-a -mcpu=cortex-a8 -mtune=cortex-a8 -mfpu=vfpv3 -mfloat-abi=softfp -DPANDORA
endif
ifeq "$(TARGET)" "pyra"
    CC		= gcc
    PFLAGS	= -march=armv7ve+simd -mcpu=cortex-a15 -mtune=cortex-a15 -mfpu=neon-vfpv4 -mfloat-abi=hard -mthumb -DPYRA
endif
ifeq "$(TARGET)" ""
    CC		= gcc

    # detect mingw and do some fixes
    ifeq ($(OS),Windows_NT)
        PFLAGS	= -Duint="unsigned int" -D_RUTAS_RELATIVAS
        NAME	= griels.exe
    endif
endif

SRC	= ./src/main.c \
	  ./src/history.c \
	  ./src/intro.c \
	  ./src/game.c \
	  ./src/hud.c \
	  ./src/hero.c \
	  ./src/loading.c \
	  ./src/ending.c \
	  ./src/zoom.c

all: $(NAME)

$(NAME): $(SRC) ./src/comun.h
	$(CC) $(PFLAGS) $(CFLAGS) $(SRC) -o $(NAME) $(CLIBS)

clean:
	rm -f $(NAME)

# Installation
install:
	cp $(NAME) /usr/bin/
	cp griels.desktop /usr/share/applications
	mkdir -p /usr/share/griels/music
	cp ./music/* /usr/share/griels/music
	mkdir -p /usr/share/griels/fx
	cp ./fx/* /usr/share/griels/fx
	mkdir -p /usr/share/griels/data
	cp ./data/* /usr/share/griels/data
	mkdir -p /usr/share/griels/png
	cp ./png/* /usr/share/griels/png
	cp ./png/icon.png /usr/share/pixmaps/griels.png

uninstall:
	rm /usr/bin/$(NAME)
	rm /usr/share/applications/griels.desktop
	rm /usr/share/pixmaps/griels.png
	rm -rf /usr/share/griels

opk:	$(NAME)
	./make_opk.sh
