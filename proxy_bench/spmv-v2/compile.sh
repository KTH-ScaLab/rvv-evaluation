#!/bin/bash

BUILD_DIR=${BUILD_DIR:-build}
LMULS=${LMULS:-"1 2 4 8"}
PROGRAM=sparse

CLANG_CC=${CLANG_CC:-"clang -std=c11 -pthread -Wunused-variable -static"}
CLANG_CXX=${CLANG_CXX:-"clang++ -std=gnu++20 -pthread -static"}
GCC_CC=${GCC_CC:-"riscv64-unknown-linux-gnu-gcc -std=c11 -pthread -Wunused-variable -static"}
GCC_CXX=${GCC_CXX:-"riscv64-unknown-linux-gnu-g++ -std=gnu++20 -pthread -static"}

build_one() {
    local compiler=$1
    local output=$2
    local nonvec_flag=$3
    local lmul=$4
    local cc cxx clang_flag

    if [ "${compiler}" = "clang" ]; then
        cc="${CLANG_CC}"
        cxx="${CLANG_CXX}"
        clang_flag=1
    else
        cc="${GCC_CC}"
        cxx="${GCC_CXX}"
        clang_flag=0
    fi

    echo "Building ${BUILD_DIR}/${compiler}/${output}"
    make clean
    if [ -n "${lmul}" ]; then
        make ${PROGRAM} CC="${cc}" CXX="${cxx}" clang=${clang_flag} nonvec=${nonvec_flag} RVV_LMUL=${lmul}
    else
        make ${PROGRAM} CC="${cc}" CXX="${cxx}" clang=${clang_flag} nonvec=${nonvec_flag}
    fi

    mkdir -p "${BUILD_DIR}/${compiler}"
    mv "${PROGRAM}" "${BUILD_DIR}/${compiler}/${output}"
    make clean
}

for compiler in clang gcc; do
    build_one "${compiler}" "sparse_nonvec" 1 ""
    build_one "${compiler}" "sparse_autovec" 0 ""
    for lmul in ${LMULS}; do
        build_one "${compiler}" "sparse_autovec_${lmul}" 0 "${lmul}"
    done
done
