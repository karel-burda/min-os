#!/bin/bash

# clean.sh -- performs a clean step
# Usage: clean.sh <root directory of min-os>

# assert param
if [ -z "$1" ]; then
	echo "Usage: $0 <root directory of min-os>";
	exit -1;
fi

make -f $1/build/Makefile clean
