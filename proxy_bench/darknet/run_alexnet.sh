wget -nc https://pjreddie.com/media/files/alexnet.weights
for compiler in gcc clang; do
  for type in nonvec autovec;do
    for threads in 8; do
          echo "./build/${compiler}/darknet_${type}"
          export OMP_NUM_THREADS=${threads}
          perf stat -e instructions,L1-dcache-loads,L1-dcache-load-misses  ./build/${compiler}/darknet_${type} classifier predict cfg/imagenet1k.data cfg/alexnet.cfg alexnet.weights data/dog.jpg
     done
    done
    for LMUL in 1 2 4 8;do
      for threads in 8; do
          export OMP_NUM_THREADS=${threads}
          echo "./build/${compiler}/darknet_autovec_${LMUL}"
          perf stat -e instructions,L1-dcache-loads,L1-dcache-load-misses ./build/${compiler}/darknet_autovec_${LMUL} classifier predict cfg/imagenet1k.data cfg/alexnet.cfg alexnet.weights data/dog.jpg
     done
    done
done