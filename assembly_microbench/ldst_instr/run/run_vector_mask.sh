#!/bin/bash
BUILD_DIRS=${BUILD_DIRS:-"build/clang21 build/gcc15"}

for BUILD_DIR in ${BUILD_DIRS}; do
  echo "== ${BUILD_DIR} =="
  for OPS in vle vse; do
    for RES in v; do
      for lmul in m1; do
        for databits in 8 16 32 64; do
          for tail in tu; do
            for mask in mu; do
              for maskcode in 1010 1 2 4 8 16 32; do
                exe="${OPS}_v${RES}_${databits}_${lmul}_${tail}_${mask}_${maskcode}"
                echo "${exe}"
                "${BUILD_DIR}/${exe}.x"
              done
            done
          done
        done
      done
    done
  done
done
