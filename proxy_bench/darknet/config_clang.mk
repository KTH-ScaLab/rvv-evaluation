CC = clang
CXX = clang++
nonvec=0
RVV_LMUL ?=
LDFLAGS += -static
ifeq ($(nonvec), 1)
        CFLAGS += -march=rv64gc -mllvm -scalable-vectorization=off #-Rpass=loop-vectorize  -Rpass-analysis=loop-vectorize
else
        CFLAGS += -march=rv64gcv_zfh_zvfh -mllvm -scalable-vectorization=on #-Rpass=loop-vectorize
ifneq ($(RVV_LMUL),)
        CFLAGS += -mllvm -riscv-v-register-bit-width-lmul=$(RVV_LMUL)
endif
endif

build = build/clang21
