#!/bin/bash

for OPS in vlse; do
  for RES in v; do
    for databits in 8 16 32 64; do
      for lmul in m1; do
        for tail in ta; do
	  for mask in ma; do
		  block=$(( 32 / ${lmul#m} ))
		  python3 kgen_vlse.py -f kernels/ld/${OPS}_v${RES}_${databits}_${lmul}_${tail}_${mask}.cpp -u 4 -b rv64gcv ${databits} int ${lmul} ${tail} ${mask} ${block} 1 ${OPS}${databits}.v "v%d" "modrange(0,32,32,${lmul#m})" 
	  done
	done
      done
    done
  done
done

for OPS in vsse; do
  for RES in v; do
    for databits in 8 16 32 64; do
      for lmul in m1; do
        for tail in ta; do
	  for mask in ma; do
		  block=$(( 32 / ${lmul#m} ))
		  python3 kgen_vlse.py -f kernels/st/${OPS}_v${RES}_${databits}_${lmul}_${tail}_${mask}.cpp -u 4 -b rv64gcv ${databits} int ${lmul} ${tail} ${mask} ${block} 1 ${OPS}${databits}.v "v%d" "modrange(0,32,32,${lmul#m})" 
	  done
	done
      done
    done
  done
done
