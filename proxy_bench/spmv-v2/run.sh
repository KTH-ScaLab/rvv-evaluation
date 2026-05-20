for compiler in gcc clang; do
  for type in nonvec autovec;do
   for threads in 8; do
        echo "./build/${compiler}/sparse_${type} ${threads} 10 10 200 1"
        perf stat -e instructions,L1-dcache-loads,L1-dcache-load-misses  ./build/${compiler}/sparse_${type} ${threads} 10 10 200 1
    done
  done
done

for compiler in gcc clang; do
  for LMUL in 1 2 4 8; do
   for threads in 8; do
        echo "./build/${compiler}/sparse_autovec_${LMUL} ${threads} 10 10 200 1"
        perf stat -e instructions,L1-dcache-loads,L1-dcache-load-misses  ./build/${compiler}/sparse_autovec_${LMUL} ${threads} 10 10 200 1
    done
  done
done