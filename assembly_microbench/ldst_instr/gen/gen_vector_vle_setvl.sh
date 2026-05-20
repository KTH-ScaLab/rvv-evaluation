#!/bin/bash

for OPS in vle; do
  for RES in v; do
    for i in 1 2 4 8 16 32; do
       for lmul in m1; do
        for tail in tu; do
	  for mask in mu; do
		  echo $i
		  block=32 #$(( 32 / ${lmul#m} ))
		  python3 kgen_vle_setvl.py -f kernels/ld/${OPS}_v${RES}_8_${lmul}_${tail}_${mask}_setvl$i.cpp -u 4 -b rv64gcv 8 int ${lmul} ${tail} ${mask} ${block} 1 $i ${OPS}8.v "v%d" "modrange(0,32,32,${lmul#m})"  
	  done
	done
      done
    done
  done
done

for OPS in vle vse; do
  for RES in v; do
    for i in 1 2 4 8 16 32; do
       for lmul in m1; do
        for tail in tu; do
	  for mask in mu; do
		  echo $i
		  block=32 #$(( 32 / ${lmul#m} ))
		  python3 kgen_vle_setvl.py -f kernels/st/${OPS}_v${RES}_8_${lmul}_${tail}_${mask}_setvl$i.cpp -u 4 -b rv64gcv 8 int ${lmul} ${tail} ${mask} ${block} 1 $i ${OPS}8.v "v%d" "modrange(0,32,32,${lmul#m})"  
	  done
	done
      done
    done
  done
done

