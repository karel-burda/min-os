#!/bin/bash

# build_and_run.sh -- performs a build & run step
# Usage: build_and_run.sh <root directory of min-os>

## run build
./$ROOT_DIR/scripts/build.sh $1
# assert
if [ $? != 0 ];
	then
		echo "build failed";
		exit -1;
fi;

## run qemu
./$ROOT_DIR/scripts/run_qemu.sh

exit 0;
