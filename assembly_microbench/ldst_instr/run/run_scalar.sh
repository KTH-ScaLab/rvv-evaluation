#!/bin/bash
BUILD_DIRS=${BUILD_DIRS:-"build/clang21 build/gcc15"}

for BUILD_DIR in ${BUILD_DIRS}; do
  echo "== ${BUILD_DIR} =="
  for exe in \
    lb_8 lh_16 lw_32 ld_64 \
    sb_8 sh_16 sw_32 sd_64 

  do
    echo "${exe}"
    "${BUILD_DIR}/${exe}.x"
  done
done
