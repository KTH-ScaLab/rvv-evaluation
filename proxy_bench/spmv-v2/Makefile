include common/OPENMP.defs

##### User configurable options #####

OPTFLAGS    = $(DEFAULT_OPT_FLAGS) 
#description: change above into something that is a decent optimization on you system

#uncomment any of the following flags (and change values) to change defaults

USERFLAGS     =  
#description: parameter to specify optional flags

EXTOBJS      = 
LIBS         =
LIBPATHS     = 
INCLUDEPATHS = 


nonvec=0
clang=0
RVV_LMUL ?=
USERFLAGS += -static
ifeq (${nonvec},1)
	ifeq (${clang},1)
		USERFLAGS += -march=rv64gc -mllvm -scalable-vectorization=off -Rpass=loop-vectorize -Rpass-analysis=loop-vectorize
	else
		USERFLAGS += -march=rv64gc -fno-tree-vectorize -fopt-info-vec
	endif
endif

ifeq (${nonvec},0)
	ifeq (${clang},1)
		USERFLAGS += -march=rv64gcv -mllvm -scalable-vectorization=on -Rpass=loop-vectorize
		ifneq ($(RVV_LMUL),)
			USERFLAGS += -mllvm -riscv-v-register-bit-width-lmul=$(RVV_LMUL)
		endif
	else
		USERFLAGS += -march=rv64gcv -fopt-info-vec
		ifneq ($(RVV_LMUL),)
			USERFLAGS += -mrvv-max-lmul=m$(RVV_LMUL)
		endif
	endif
endif

float=0
ifeq ($(float),1)
        USERFLAGS += -DMTYPE=float
endif


ifndef SCRAMBLE
 SCRAMBLE=1
endif
#description: if flag is true, grid indices are scrambled to produce irregular stride

ifndef RESTRICT_KEYWORD
  RESTRICT_KEYWORD=0
endif
#description: the "restrict" keyword can be used on IA platforms to disambiguate  
#             data accessed through pointers (requires -restrict compiler flag)

ifndef MAXTHREADS
  MAXTHREADS=512
endif
#description: default thread limit is 512

ifndef VERBOSE
  VERBOSE=0
endif

VERBOSEFLAG = -DVERBOSE=$(VERBOSE)
RESTRICTFLAG= -DRESTRICT_KEYWORD=$(RESTRICT_KEYWORD)
NTHREADFLAG = -DMAXTHREADS=$(MAXTHREADS)
SCRAMBLEFLAG= -DSCRAMBLE=$(SCRAMBLE)

OPTIONSSTRING="Make options:\n\
OPTION                 MEANING                                  DEFAULT\n\
SCRAMBLE=0/1           regular/irregular sparsity pattern         [1]  \n\
RESTRICT_KEYWORD=0/1   disable/enable restrict keyword (aliasing) [0]  \n\
MAXTHREADS=?           set maximum number of OpenMP threads       [512]\n\
VERBOSE=0/1            omit/include verbose run information       [0]"

TUNEFLAGS   = $(VERBOSEFLAG) $(NTHREADFLAG) $(USERFLAGS) $(SCRAMBLEFLAG) \
              $(RESTRICTFLAG)
PROGRAM     = sparse
OBJS        = $(PROGRAM).o $(COMOBJS)

include common/make.common
