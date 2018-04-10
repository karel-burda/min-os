#!/bin/bash

# build.sh -- creates a brand new kernel, with cleaning up the stuff afterwards
# Usage: build.sh <root directory of min-os>

# assert param
if [ -z "$1" ]; then
	echo "Usage: $0 <root directory of min-os>";
	exit -1;
fi

# assert dir
if [ ! -d "$1" ]; then
	echo "'$1' is not a directory";
	exit -1;
fi

## declare vars
# paths
ROOT_DIR=$1
BIN_DIR=$ROOT_DIR/bin
OBJ_DIR=$ROOT_DIR/obj
BOOTLOADER_DIR=$ROOT_DIR/bootloaders
KERNEL_DIR=$ROOT_DIR/src/kernel
LINKER_DIR=$ROOT_DIR/build
EXEC_DIR=$ROOT_DIR/build
LOADER_DIR=$ROOT_DIR/loader
# other
PRINT_PREFIX="=============================="
PRINT_PREFIX_SUB="------------------------------"

# print
echo "Root dir = '$ROOT_DIR'";

## assembler
echo "$PRINT_PREFIX";
echo "Compilation (assembler files)";
echo "$PRINT_PREFIX_SUB";
nasm -f elf -o $OBJ_DIR/boot_grub.o $BOOTLOADER_DIR/boot_grub.asm
# assert
if [ $? != 0 ];
	then
		echo "nasm failed";
		exit -1;
fi;
# print
echo "$PRINT_PREFIX";

## makefile
echo "$PRINT_PREFIX";
echo "Compilation (C files)";
echo "$PRINT_PREFIX_SUB";
make -f $ROOT_DIR/build/Makefile
# assert
if [ $? != 0 ];
	then
		echo "make failed";
		exit -1;
fi;
#print
echo "$PRINT_PREFIX";

## linkage
echo "$PRINT_PREFIX";
echo "Linkage";
echo "$PRINT_PREFIX_SUB";
# link with all .o files in the $OBJ_DIR
OBJ_FILES=`find $OBJ_DIR -iname "*.o" | tr '\n' ' '`
echo "Linking with $OBJ_FILES";
ld -T $LOADER_DIR/loader.ld -m elf_i386 -o $BIN_DIR/kernel.bin $OBJ_FILES
# assert
if [ $? != 0 ];
	then
		echo "linkage failed";
		exit -1;
fi;
echo "$PRINT_PREFIX";

## cleaning
echo "$PRINT_PREFIX";
echo "Cleaning";
echo "$PRINT_PREFIX_SUB";
./scripts/clean.sh $ROOT_DIR
echo "$PRINT_PREFIX";

## ISO
echo "$PRINT_PREFIX";
echo "Creating ISO";
echo "$PRINT_PREFIX_SUB";
cp $BIN_DIR/kernel.bin iso/boot/min-os.bin
./scripts/mk_iso.sh min-os.iso iso
echo "$PRINT_PREFIX";
