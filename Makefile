BUILD_PATH ?= ./build

CC ?= gcc
CFLAGS ?= -I./include -lncurses -Wall -Wextra -Werror -pedantic -std=c11

ifdef DEBUG
	CFLAGS += -g3 -DDEBUG
endif

.PHONY: all buildpath tags clean


################
# object files #
################

# compile *.o using source file of same base name
# ===============================================
%.o: src/%.c
	$(CC) $(CFLAGS) $< -c -o "$(BUILD_PATH)"/src/$@


###############
# main target #
###############

# creates dulite executable in $(BUILD_PATH)
# ==========================================
all: buildpath clean src/main.c
	$(CC) $(CFLAGS) $(shell find "$(BUILD_PATH)"/src/ -name "*.o") src/main.c -o $(BUILD_PATH)/pong

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
	$(CC) $(CFLAGS) $^ -o "$(BUILD_PATH)"/proto/$*

# compiles all prototypes
# =======================
proto: buildpath animated_hello_world.proto


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