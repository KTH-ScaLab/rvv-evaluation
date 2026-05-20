#!/bin/bash

for OPS in vle; do
  for RES in v; do
    for databits in 8 16 32 64; do
      for lmul in m1; do
        for tail in tu; do
	  for mask in mu; do
	    for maskcode in 1010; do
		  block=30 #$(( 32 / ${lmul#m} ))
		  python3 kgen_vle_vm.py -f kernels/ld/${OPS}_v${RES}_${databits}_${lmul}_${tail}_${mask}_${maskcode}.cpp -u 4 -b rv64gcv ${databits} int ${lmul} ${tail} ${mask} ${block} 1 ${maskcode} ${OPS}${databits}.v "v%d" "repeat_2_30(30)"  
	    done
	  done
	done
      done
    done
  done
done

for OPS in vse; do
  for RES in v; do
    for databits in 8 16 32 64; do
      for lmul in m1; do
        for tail in tu; do
	  for mask in mu; do
	    for maskcode in 1010; do
		  block=30 #$(( 32 / ${lmul#m} ))
		  python3 kgen_vle_vm.py -f kernels/st/${OPS}_v${RES}_${databits}_${lmul}_${tail}_${mask}_${maskcode}.cpp -u 4 -b rv64gcv ${databits} int ${lmul} ${tail} ${mask} ${block} 1 ${maskcode} ${OPS}${databits}.v "v%d" "repeat_2_30(30)"  
	    done
	  done
	done
      done
    done
  done
done

for OPS in vle; do
  for RES in v; do
    for databits in 8; do
      for lmul in m1; do
        for tail in tu; do
	  for mask in mu; do
	    for maskcode in 1 2 4 8 16 32; do
		  block=30 #$(( 32 / ${lmul#m} ))
		  python3 kgen_vle_vm.py -f kernels/ld/${OPS}_v${RES}_${databits}_${lmul}_${tail}_${mask}_${maskcode}.cpp -u 4 -b rv64gcv ${databits} int ${lmul} ${tail} ${mask} ${block} 1 ${maskcode} ${OPS}${databits}.v "v%d" "repeat_2_30(30)"  
	    done
	  done
	done
      done
    done
  done
done

for OPS in vse; do
  for RES in v; do
    for databits in 8; do
      for lmul in m1; do
        for tail in tu; do
	  for mask in mu; do
	    for maskcode in 1 2 4 8 16 32; do
		  block=30 #$(( 32 / ${lmul#m} ))
		  python3 kgen_vle_vm.py -f kernels/st/${OPS}_v${RES}_${databits}_${lmul}_${tail}_${mask}_${maskcode}.cpp -u 4 -b rv64gcv ${databits} int ${lmul} ${tail} ${mask} ${block} 1 ${maskcode} ${OPS}${databits}.v "v%d" "repeat_2_30(30)"  
	    done
	  done
	done
      done
    done
  done
done

