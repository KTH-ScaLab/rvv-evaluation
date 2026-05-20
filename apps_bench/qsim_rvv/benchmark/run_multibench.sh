THREADS_LIST=(32 64 72)
FUSED_LIST=(3 4)
benchmark=('grover' 'ghz' 'QRC' 'qv' 'qft')
QUBITS=(32 33 34 35 36)

for b in "${benchmark[@]}"; do
  for q in "${QUBITS[@]}"; do
    for FUSED in "${FUSED_LIST[@]}"; do
      for THREADS in "${THREADS_LIST[@]}"; do
         INSTR=3 python benchmark/run_qsim_gt32.py -b $b -q $q -t $THREADS -f $FUSED -r 3
      done
   done
 done
done

