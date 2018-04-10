#!/bin/bash

# mk_iso.sh -- creates an ISO file containing OS kernel (using GRUB)
# Usage: mk_iso.sh <destination dir> <source iso dir>

# assert params
if [ -z "$1" ] || [ -z "$2" ]; then
	echo "Usage: $0 <destination dir> <source iso dir>";
	exit -1;
fi

# assert src dir
if [ ! -d "$2" ]; then
	echo "'$2' is not a directory";
	exit -1;
fi

# create ISO
grub-mkrescue -o $1 $2

# ret value
if [ $? != 0 ];
	then
		echo "$0 failed";
		exit -1;
fi;

exit 0;
