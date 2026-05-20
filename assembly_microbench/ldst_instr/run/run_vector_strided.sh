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

  for OPS in vlse vsse; do
    for RES in v; do
      for lmul in m1; do
        for databits in 8 16 32 64; do
          for tail in ta; do
            for mask in ma; do
              exe="${OPS}_v${RES}_${databits}_${lmul}_${tail}_${mask}"
              echo "${exe}"
              "${BUILD_DIR}/${exe}.x"
            done
          done
        done
      done
    done
  done

  for OPS in vle vse; do
    for RES in v; do
      for lmul in m1; do
        for databits in 8 16 32 64; do
          for tail in tu; do
            for mask in mu; do
              for maskcode in 1010; do
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
