#!/bin/bash
mkdir -p kernels/ld/
for OPS in lb lh lw ld; do
  for RES in a; do
		  case "$OPS" in lb|lbu)databits=8;;
    			lh|lhu)databits=16;;
    			lw|lwu)databits=32;;
    			ld)databits=64;;
		  esac 
		  python3 kgen_scalar.py -f kernels/ld/${OPS}_${databits}.cpp -u 32 -b rv64gc ${databits} int m1 ta ma 7 1 ${OPS} "a%d" "modrange(1,8,8,1)" 
    done
done


mkdir -p kernels/st/
for OPS in sb sh sw sd; do
  for RES in a; do
		  case "$OPS" in sb) databits=8;;
    			sh|shu)databits=16;;
    			sw|swu)databits=32;;
    			sd)databits=64;;
		  esac 
		  python3 kgen_scalar.py -f kernels/st/${OPS}_${databits}.cpp -u 32 -b rv64gc ${databits} int m1 ta ma 7 1 ${OPS} "a%d" "modrange(1,8,8,1)" 
    done
done

