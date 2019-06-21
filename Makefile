BUILD_PATH ?= ./build

CC ?= gcc
CFLAGS ?= -I./include -lncurses -D_DEFAULT_SOURCE -Wall -Wextra -Werror -pedantic -std=c11

ifdef DEBUG
	CFLAGS += -g3 -DDEBUG
endif

ifdef STEP
	CFLAGS += -DSTEP
endif

.PHONY: all physics pong buildpath tags clean proto


################
# object files #
################

# compile *.o using source file of same base name
# ===============================================
%.o: src/%.c
	$(CC) $(CFLAGS) $< -c -o "$(BUILD_PATH)"/src/$@

physics/%.o: src/physics/%.c
	$(CC) $(CFLAGS) $< -c -o "$(BUILD_PATH)"/src/$(notdir $@)

###############
# main target #
###############

# creates pong executable in $(BUILD_PATH)
# ========================================
all: buildpath clean util.o pong physics input.o game.o src/main.c
	$(CC) $(CFLAGS) $(shell find "$(BUILD_PATH)"/src/ -name "*.o") src/main.c -o "$(BUILD_PATH)"/pong

physics: physics/collision.o physics/line.o physics/ratio.o physics/vector.o

pong: objects.o screens.o board.o render.o update.o

# ensures proper build layout
# ===========================
buildpath:
	mkdir -p $(BUILD_PATH)/src $(BUILD_PATH)/proto


#####################
# prototype targets #
#####################

# compiles prototypes made as proof-of-concept programs
# =====================================================
%.proto: proto/%.c
	$(CC) $(CFLAGS) $(shell find "$(BUILD_PATH)"/src/ -name "*.o") $^ -o "$(BUILD_PATH)"/proto/$*

# compiles all prototypes
# =======================
proto: all animated_hello_world.proto moving_ball.proto


################
# misc targets #
################

# for faster project navigation via vim. must have universal-ctags
# ================================================================
tags:
	ctags -R

# remove all compiled files
# =========================
clean:
	find "$(BUILD_PATH)" -type f -exec rm -f {} \;
