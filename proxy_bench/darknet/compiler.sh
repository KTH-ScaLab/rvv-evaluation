#!/bin/bash

EXEC=darknet
BUILD_DIR=build

build_one() {
  local comp=$1
  local mode=$2
  local output=$3
  local lmul=${4:-}
  local config="config_${comp}.mk"

  echo "Building ${comp} ${output}"
  make clean

  if [[ ${mode} == "nonvec" ]]; then
    make CONFIG="${config}" OPENMP=1 nonvec=1
  elif [[ -n ${lmul} ]]; then
    make CONFIG="${config}" OPENMP=1 nonvec=0 RVV_LMUL="${lmul}"
  else
    make CONFIG="${config}" OPENMP=1 nonvec=0
  fi

  mkdir -p "${BUILD_DIR}/${comp}"
  mv "${EXEC}" "${BUILD_DIR}/${comp}/${output}"
  make clean
}

for comp in gcc clang; do
  build_one "${comp}" nonvec "darknet_nonvec"
  build_one "${comp}" autovec "darknet_autovec"

  for lmul in 1 2 4 8; do
    build_one "${comp}" autovec "darknet_autovec_${lmul}" "${lmul}"
  done
done
