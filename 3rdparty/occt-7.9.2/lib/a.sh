#!/bin/bash

OCCT_LIB_DIR="/home/xuming/work/occ_viewer/3rdparty/occt-7.9.2/lib"

echo "set(OCCT_LIBS"
for lib in $(ls -1 "$OCCT_LIB_DIR" | grep '\.7\.9\.2$' | sort); do
    echo "    \${OCCT_LIB_DIR}/$lib"
done
echo ")"

