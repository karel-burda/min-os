# params
# TODO: use i686-elf
CC = gcc
CFLAGS = -m32 -fno-stack-protector -fno-builtin -O2 -Wall -Wextra -std=gnu99 -c -nostdlib -ffreestanding -pedantic
LDFLAGS =
LINK_TARGET = kernel

# dirs
ROOT_DIR = .
OBJ_DIR = $(ROOT_DIR)/obj

# includes
INC=terminal video kernel common strings standard-lib pic keyboard io shell programs
INCLUDE_DIRS=-I$(ROOT_DIR)/source/include $(INC:%=-I$(ROOT_DIR)/source/include/%)

# objects
OBJS =  \
 $(ROOT_DIR)/source/src/video/video.o \
 $(ROOT_DIR)/source/src/terminal/terminal.o \
 $(ROOT_DIR)/source/src/io/io.o \
 $(ROOT_DIR)/source/src/common/chars.o \
 $(ROOT_DIR)/source/src/common/common.o \
 $(ROOT_DIR)/source/src/strings/strings.o \
 $(ROOT_DIR)/source/src/pic/pic.o \
 $(ROOT_DIR)/source/src/standard-lib/std.o \
 $(ROOT_DIR)/source/src/keyboard/keyboard_layout.o \
 $(ROOT_DIR)/source/src/keyboard/keyboard.o \
 $(ROOT_DIR)/source/src/shell/shell.o \
 $(ROOT_DIR)/source/src/terminal/terminal_test.o \
 $(ROOT_DIR)/source/src/kernel/kernel_test.o \
 $(ROOT_DIR)/source/src/programs/logo/logo.o \

# buildables
REBUILDABLES = $(OBJS) $(LINK_TARGET)

# targets
all : $(LINK_TARGET)
	@echo "All done"

clean :
	rm -f $(REBUILDABLES)

# $@ expands to the rule's target, in this case "test_me.exe".
# $^ expands to the rule's dependencies, in this case the three files
# main.o, test1.o, and  test2.o.
$(LINK_TARGET) : $(OBJS)
	@echo "Compiling '$@'"
	$(CC) $(LDFLAGS) $(CFLAGS) -o $(OBJ_DIR)/kernel.o ./source/src/kernel/kernel.c $(INCLUDE_DIRS)

# Here is a Pattern Rule, often used for compile-line.
# It says how to create a file with a .o suffix, given a file with a .cpp suffix.
# The rule's command uses some built-in Make Macros:
# $@ for the pattern-matched target
# $< for the pattern-matched dependency
%.o : %.c
	@echo "Compiling '$@'"
	$(CC) $(CFLAGS) -o $@ $(INCLUDE_DIRS) $<
	cp $@ $(OBJ_DIR)/$(basename $(notdir $@)).o

# dependency Rules
 %.dep : %.c
#   g++ -M $(FLAGS) $< > $@
# include $(OBJS:.o=.dep)

