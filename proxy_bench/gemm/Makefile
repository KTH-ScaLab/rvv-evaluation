include ./common/OPENMP.defs

##### User configurable options #####

OPTFLAGS     = $(DEFAULT_OPT_FLAGS) 
#description: change above into something that is a decent optimization on you system

#uncomment any of the following flags (and change values) to change defaults

USERFLAGS     =    
#description: parameter to specify optional flags

EXTOBJS      = 
#LIBS         = -lmkl_ia32
#LIBPATHS     = -L/opt/intel/mkl/10.0.3.020/lib/32
#INCLUDEPATHS = -I/opt/intel/mkl/10.0.3.020/include

#LIBS += -lmyperf
#LIBPATHS += -L../libperf
#INCLUDEPATHS += -I../libperf

### End User configurable options ###

ifndef MKL
  MKL=0
endif
#description: set this flag to call the tuned mkl library

ifndef VERBOSE
  VERBOSE=0
endif
#description: default diagnostic style is silent

ifndef RESTRICT_KEYWORD
  RESTRICT_KEYWORD=0
endif
#description: the "restrict" keyword can be used on IA platforms to disambiguate  
#             data accessed through pointers (requires -restrict compiler flag)

ifndef DEFAULTBLOCK
  DEFAULTBLOCK=32
endif
#description: default tile size is 32 (used only for non-MKL version)

ifndef BOFFSET
  BOFFSET=12
endif
#description: set this flag to some value to override default first array 
#             dimension padding (12) of tiles used in non-MKL version

ifndef MAXTHREADS
  MAXTHREADS=512
endif
#description: default thread limit is 512

nonvec=0
clang=0
RVV_LMUL ?=
USERFLAGS += -static
ifeq (${nonvec},1)
	ifeq (${clang},1)
		USERFLAGS += -march=rv64gc -mllvm -scalable-vectorization=off
	else
		USERFLAGS += -march=rv64gc -fno-tree-vectorize
	endif
endif

ifeq (${nonvec},0)
	ifeq (${clang},1)
		USERFLAGS += -march=rv64gcv -mllvm -scalable-vectorization=on
		ifneq ($(RVV_LMUL),)
			USERFLAGS += -mllvm -riscv-v-register-bit-width-lmul=$(RVV_LMUL)
		endif
	else
		USERFLAGS += -march=rv64gcv
		ifneq ($(RVV_LMUL),)
			USERFLAGS += -mrvv-max-lmul=m$(RVV_LMUL)
		endif
	endif
endif

float=0
ifeq ($(float),1)
        USERFLAGS += -DMTYPE=float
endif


PROGRAM     = gemm
MKLFLAG     = -DMKL=$(MKL)
VERBOSEFLAG = -DVERBOSE=$(VERBOSE)
RESTRICTFLAG= -DRESTRICT_KEYWORD=$(RESTRICT_KEYWORD)
BLOCKFLAG   = -DDEFAULTBLOCK=$(DEFAULTBLOCK)
OFFSETFLAG  = -DBOFFSET=$(BOFFSET)
NTHREADFLAG = -DMAXTHREADS=$(MAXTHREADS)

OPTIONSSTRING="Make options:\n\
OPTION                 MEANING                                              DEFAULT \n\
MKL=0/1                disable/enable MKL library                              [0]  \n\
RESTRICT_KEYWORD=0/1   disable/enable restrict keyword (aliasing)              [0]  \n\
DEFAULTBLOCK=?         set tile size (non-MKL version)                         [32] \n\
BOFFSET=?              override default first array dimension padding of tiles [12] \n\
MAXTHREADS=?           set maximum number of OpenMP threads                    [512]\n\
VERBOSE=0/1            omit/include verbose run information                    [0]"

TUNEFLAGS   = $(BLOCKFLAG)   $(MKLFLAG)     $(OFFSETFLAG) $(RESTRICTFLAG) \
              $(VERBOSEFLAG) $(NTHREADFLAG) $(USERFLAGS)
OBJS        = $(PROGRAM).o $(COMOBJS) 

include ./common/make.common
