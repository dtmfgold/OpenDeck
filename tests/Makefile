.DEFAULT_GOAL := test

TARGETNAME := fw_mega2560
BUILD_DIR_BASE := ./build
BUILD_DIR := $(BUILD_DIR_BASE)/$(TARGETNAME)
SCRIPTS_DIR := ../scripts

ifeq ($(shell uname), Linux)
    FIND := find
else ifeq ($(shell uname), Darwin)
    FIND := gfind
else
    $(error Unsupported platform)
endif

include Defines.mk
include Sources.mk
-include Objects.mk

C_COMPILER := clang-9
CPP_COMPILER := clang++-9
LLVM_PROFDATA := llvm-profdata-9
LLVM_COV := llvm-cov-9
CLANG_FORMAT := clang-format-9

TEST_BINARIES := $(addprefix -object ,$(shell $(FIND) $(BUILD_DIR_BASE) -name "*.out" 2>/dev/null))

#do not allow overriding this variable via command-line
override undefine TESTS_EXPANDED

FW_ROOT_DIR := ../src

#passed to both c and c++ compiler
COMMON_FLAGS := \
-fprofile-instr-generate \
-fcoverage-mapping \
-Og \
-g \
-Wall

#c++ compiler only
CPP_FLAGS := \
-std=c++11

#c compiler only
C_FLAGS := \
-std=c11

#linker
LDFLAGS :=

$(BUILD_DIR)/%.c.o $(BUILD_DIR_BASE)/%.c.o: %.c
	@mkdir -p $(@D)
	@echo Building: $<
	@$(C_COMPILER) $(COMMON_FLAGS) $(C_FLAGS) $(addprefix -D,$(DEFINES)) $(INCLUDE_DIRS_COMMON) $(INCLUDE_FILES_COMMON) -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -c "$<" -o "$@"

$(BUILD_DIR)/%.cpp.o $(BUILD_DIR_BASE)/%.cpp.o: %.cpp
	@mkdir -p $(@D)
	@echo Building: $<
	@$(CPP_COMPILER) $(COMMON_FLAGS) $(CPP_FLAGS) $(addprefix -D,$(DEFINES)) $(INCLUDE_DIRS_COMMON) $(INCLUDE_FILES_COMMON) -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -c "$<" -o "$@"

define LINK_OBJECTS
	@echo Creating executable: $@
	@$(CPP_COMPILER) $(LDFLAGS) $(COMMON_FLAGS) $(CPP_FLAGS) $^ -o $@
endef

pre-build:
	@chmod +x $(SCRIPTS_DIR)/gen_test_targets.sh && $(SCRIPTS_DIR)/gen_test_targets.sh
	@chmod +x $(SCRIPTS_DIR)/gen_test_runners.sh && $(SCRIPTS_DIR)/gen_test_runners.sh

exec:
	@echo Running all compiled tests.
	@rm -rf $(wildcard $(BUILD_DIR_BASE)/*.profraw)
	@export LLVM_PROFILE_FILE="$(BUILD_DIR_BASE)/test-%p.profraw" && \
	$(FIND) $(BUILD_DIR_BASE) -type f -name "*.out" -printf 'Running %p\n' -exec {} \;

coverage:
	@echo Creating coverage report.
	@$(LLVM_PROFDATA) merge $(wildcard $(BUILD_DIR_BASE)/*.profraw) -o $(BUILD_DIR_BASE)/tests.profdata
	@$(LLVM_COV) report $(TEST_BINARIES) -instr-profile=$(BUILD_DIR_BASE)/tests.profdata -ignore-filename-regex="test|modules" -use-color
	@$(LLVM_COV) show $(TEST_BINARIES) -instr-profile=$(BUILD_DIR_BASE)/tests.profdata -ignore-filename-regex="test|modules" -use-color --format html > $(BUILD_DIR_BASE)/coverage.html
	@echo ***Generated HTML coverage report in $(BUILD_DIR_BASE)/coverage.html***

format:
	@echo Checking code formatting...
	@$(FIND) . -regex '.*\.\(cpp\|hpp\|h\|cc\|cxx\)' \
	-exec $(CLANG_FORMAT) -style=file -i {} \;

clean:
	@echo Cleaning up.
	@rm -rf $(BUILD_DIR_BASE)/
	@rm -rf gen/

#debugging
print-%:
	@echo '$($*)'
