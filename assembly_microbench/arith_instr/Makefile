CONFIGS := config_clang.mk config_gcc.mk

.PHONY: all clang gcc build-one clean clean-one veryclean veryclean-one

all: clang gcc

clang:
	$(MAKE) CONFIG=config_clang.mk build-one

gcc:
	$(MAKE) CONFIG=config_gcc.mk build-one

clean:
	for config in $(CONFIGS); do $(MAKE) CONFIG=$$config clean-one; done

veryclean:
	for config in $(CONFIGS); do $(MAKE) CONFIG=$$config veryclean-one; done

ifdef CONFIG
include $(CONFIG)

OBJDIR = $(build)/obj
DRIVER_OBJ = $(OBJDIR)/driver.o

KERNEL_SRCS := $(wildcard kernels/*.cpp)
KERNEL_OBJS := $(patsubst kernels/%.cpp,$(OBJDIR)/kernels/%.o,$(KERNEL_SRCS))
EXE         := $(patsubst kernels/%.cpp,$(build)/%.x,$(KERNEL_SRCS))

build-one: $(EXE)

$(build) $(OBJDIR) $(OBJDIR)/kernels:
	mkdir -p $@

$(build)/%.x: $(OBJDIR)/kernels/%.o $(DRIVER_OBJ) | $(build)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/kernels/%.o: kernels/%.cpp | $(OBJDIR)/kernels
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(DRIVER_OBJ): driver.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean-one:
	rm -f $(KERNEL_OBJS) $(DRIVER_OBJ) $(EXE)

veryclean-one: clean-one
	rm -f kernels/*.cpp
endif
