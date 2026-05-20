#!/bin/bash
script_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)

"${script_dir}/run_scalar_int.sh"
"${script_dir}/run_scalar_fp.sh"
"${script_dir}/run_vector_int.sh"
"${script_dir}/run_vector_fp.sh"
