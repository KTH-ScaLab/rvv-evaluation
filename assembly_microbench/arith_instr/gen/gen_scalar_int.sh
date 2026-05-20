#!/bin/bash

for OPS in add mul div; do
  for RES in v; do
    for databits in 8 16 32 64; do
      for lmul in m1; do
        for tail in ta; do
	  for mask in ma; do
		block=$(( 8 / ${lmul#m} ))
		echo "${block}"
		python3 kgen.py -f kernels/${OPS}_${databits}_${lmul}_${tail}_${mask}.cpp -u 16 -b rv64gc  ${databits} int ${lmul} ${tail} ${mask} ${block} 1 ${OPS} "a%d" "modrange(0,8,8,1)" "a%d" "modrange(2,10,8,1)" "a%d" "modrange(4,12,8,1)"
	  done
	done
      done
    done
  done
done

