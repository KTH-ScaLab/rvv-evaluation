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

KERNEL_DIRS := kernels kernels/ld kernels/st
vpath %.cpp $(KERNEL_DIRS)

KERNEL_SRCS := $(foreach dir,$(KERNEL_DIRS),$(wildcard $(dir)/*.cpp))
KERNEL_NAMES := $(basename $(notdir $(KERNEL_SRCS)))
EXE := $(addprefix $(build)/,$(addsuffix .x,$(KERNEL_NAMES)))

build-one: $(EXE)

$(build):
	mkdir -p $@

$(build)/%.x: %.cpp driver.cpp | $(build)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean-one:
	rm -f $(EXE)
	rm -f $(build)/*.x $(build)/ld/*.x $(build)/st/*.x
	rm -rf $(build)/obj

veryclean-one: clean-one
	rm -f kernels/*.cpp kernels/ld/*.cpp kernels/st/*.cpp
endif
