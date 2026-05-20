CC = riscv64-unknown-linux-gnu-gcc
CXX = riscv64-unknown-linux-gnu-g++

nonvec=0
RVV_LMUL ?=
LDFLAGS += -static
ifeq ($(nonvec), 1)
	CFLAGS += -march=rv64gc -fno-tree-vectorize #-fopt-info-vec-all
else
	CFLAGS += -march=rv64gcv_zfh_zvfh #-fopt-info-vec-all
ifneq ($(RVV_LMUL),)
	CFLAGS += -mrvv-max-lmul=m$(RVV_LMUL)
endif
endif

build = build/gcc15
