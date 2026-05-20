#!/bin/bash

set -e

BUILD_DIR=${BUILD_DIR:-build}
LMULS=${LMULS:-"1 2 4 8"}
PROGRAM=gemm

CLANG_CC=${CLANG_CC:-"clang -std=c11 -pthread -static"}
CLANG_CXX=${CLANG_CXX:-"clang++ -std=gnu++20 -pthread -fmax-errors=1 -static"}
GCC_CC=${GCC_CC:-"riscv64-unknown-linux-gnu-gcc -std=c11 -pthread -static"}
GCC_CXX=${GCC_CXX:-"riscv64-unknown-linux-gnu-g++ -std=gnu++20 -pthread -static"}

if [ -n "${PRECISION:-}" ]; then
    PRECISIONS=${PRECISION}
else
    PRECISIONS=${PRECISIONS:-"float double"}
fi

precision_flag() {
    case "$1" in
        float) echo "float=1" ;;
        double) echo "float=0" ;;
        *)
            echo "PRECISIONS/PRECISION entries must be float or double" >&2
            exit 1
            ;;
    esac
}

build_one() {
    local compiler=$1
    local precision=$2
    local output=$3
    local nonvec_flag=$4
    local lmul=$5
    local cc cxx clang_flag precision_make_flag

    if [ "${compiler}" = "clang" ]; then
        cc="${CLANG_CC}"
        cxx="${CLANG_CXX}"
        clang_flag=1
    else
        cc="${GCC_CC}"
        cxx="${GCC_CXX}"
        clang_flag=0
    fi
    precision_make_flag=$(precision_flag "${precision}")

    echo "Building ${BUILD_DIR}/${compiler}/${precision}/${output}"
    make clean
    if [ -n "${lmul}" ]; then
        make ${PROGRAM} CC="${cc}" CXX="${cxx}" clang=${clang_flag} nonvec=${nonvec_flag} RVV_LMUL=${lmul} ${precision_make_flag}
    else
        make ${PROGRAM} CC="${cc}" CXX="${cxx}" clang=${clang_flag} nonvec=${nonvec_flag} ${precision_make_flag}
    fi

    mkdir -p "${BUILD_DIR}/${compiler}/${precision}"
    mv "${PROGRAM}" "${BUILD_DIR}/${compiler}/${precision}/${output}"
    make clean
}

for precision in ${PRECISIONS}; do
    precision_flag "${precision}" >/dev/null
    for compiler in clang gcc; do
        build_one "${compiler}" "${precision}" "gemm_nonvec" 1 ""
        build_one "${compiler}" "${precision}" "gemm_autovec" 0 ""
        for lmul in ${LMULS}; do
            build_one "${compiler}" "${precision}" "gemm_autovec_${lmul}" 0 "${lmul}"
        done
    done
done
