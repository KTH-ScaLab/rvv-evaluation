#!/bin/bash

for OPS in vadd vmul vdiv vmacc; do
  for RES in v; do
    for databits in 8 16 32 64; do
      for lmul in m1; do
        for tail in ta; do
	  for mask in ma; do
		block=$(( 32 / ${lmul#m} ))
		echo "${block}"
		python3 kgen.py -f kernels/${OPS}_v${RES}_${databits}_${lmul}_${tail}_${mask}.cpp -u 4 -b rv64gcv ${databits} int ${lmul} ${tail} ${mask} ${block} 1 ${OPS}.v${RES} "v%d" "modrange(0,32,32,${lmul#m})" "${RES}%d" "modrange(8,40, 32,${lmul#m})"  "v%d" "modrange(16,48, 32,${lmul#m})"
	  done
	done
      done
    done
  done
done

