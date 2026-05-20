for datatype in float double; do
for compiler in gcc clang; do
  for type in nonvec autovec;do
   for size in 2000; do
   for threads in 8; do
        echo "./build/${compiler}/${datatype}/gemm_${type} ${threads} 10 2000 16"
        perf stat -e instructions,L1-dcache-loads,L1-dcache-load-misses  ./build/${compiler}/${datatype}/gemm_${type} ${threads} 10 2000 16
    done
    done
  done
done

for compiler in gcc clang; do
  for LMUL in 1 2 4 8; do
   for size in 2000; do
   for threads in 8; do
        echo "./build/${compiler}/${datatype}/gemm_autovec_${LMUL} ${threads} 10 2000 16"
        perf stat -e instructions,L1-dcache-loads,L1-dcache-load-misses  ./build/${compiler}/${datatype}/gemm_autovec_${LMUL} ${threads} 10 2000 16
    done
    done
  done
done
done
