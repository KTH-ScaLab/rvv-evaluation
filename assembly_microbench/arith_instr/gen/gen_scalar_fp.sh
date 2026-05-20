#!/bin/bash

for OPS in fadd fmul fmadd fdiv; do
  for RES in v; do
    for databits in 16 32 64; do
      for lmul in m1; do
        for tail in ta; do
	  for mask in ma; do
		case "$databits" in
  		16) suf=h ;; 32) suf=s ;; 64) suf=d ;;   
		esac
		echo ${suf}
		if [[ ${OPS} == fmadd ]]; then
		  python3 kgen.py -f kernels/${OPS}_${databits}_${lmul}_${tail}_${mask}.cpp -u 4 -b rv64gc  ${databits} float ${lmul} ${tail} ${mask} 32 1 ${OPS}.${suf} "f%d" "modrange(0,32,32,1)" "f%d" "modrange(8,40,32,1)" "f%d" "modrange(16,48,32,1)" "f%d" "modrange(16,48,32,1)"
	        else
		  python3 kgen.py -f kernels/${OPS}_${databits}_${lmul}_${tail}_${mask}.cpp -u 4 -b rv64gc  ${databits} float ${lmul} ${tail} ${mask} 32 1 ${OPS}.${suf} "f%d" "modrange(0,32,32,1)" "f%d" "modrange(8,40,32,1)" "f%d" "modrange(16,48,32,1)"
		fi
	  done
	done
      done
    done
  done
done

