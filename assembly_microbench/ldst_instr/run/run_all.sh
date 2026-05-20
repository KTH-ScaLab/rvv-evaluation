#!/bin/bash
script_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)

"${script_dir}/run_scalar.sh"
"${script_dir}/run_vector_contiguous.sh"
"${script_dir}/run_vector_mask.sh"
"${script_dir}/run_vector_setvl.sh"
"${script_dir}/run_vector_strided.sh"
