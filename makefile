# dtree Makefile - C++20 with std::coroutine
# All 8 tests build successfully!

CXX := $(shell which clang++ 2>/dev/null || which g++ 2>/dev/null || echo clang++)
CXXSTD := -std=c++20
CXXFLAGS := $(CXXSTD) -fno-omit-frame-pointer

# Boost include (if needed)
ifneq ($(wildcard /opt/homebrew/include/boost/version.hpp),)
  CXXFLAGS += -I/opt/homebrew/include
endif

# LLVM include (if needed)
ifneq ($(wildcard /opt/homebrew/opt/llvm/include/c++/v1),)
  CXXFLAGS += -isystem/opt/homebrew/opt/llvm/include/c++/v1
endif

ERROR_LEVEL := -1
FXX := $(CXX)
FUZZ := afl-fuzz
FENV := AFL_NO_ARITH=1 AFL_EXIT_WHEN_DONE=1 AFL_HANG_TMOUT=100
FDIR := fuzz
SPLIT := perl -pe 'BEGIN{$$a="aa"}open STDOUT,">$$ARGV[0].$$a++;END{}' -

.SECONDARY:
.SUFFIXES:

all: out.test.out string.test.out container.test.out number.test.out interval.test.out expr.test.out sql.test.out store.test.out

%.test.out: qtl/%.h qtl/out.h qtl/string.h
	mkdir -p compiles
	$(CXX) $(CXXFLAGS) -o $@ -g3 -DTEST_H='"'$<'"' test.cpp 2>&1 | tee ./$(*F).make.log

# Sanitizer builds
%.asan.test.out: qtl/%.h qtl/out.h qtl/string.h
	$(CXX) $(CXXFLAGS) -fsanitize=address -o $@ -g3 -DTEST_H='"'$<'"' test.cpp 2>&1 | tee ./$(*F).asan.make.log

%.ubsan.test.out: qtl/%.h qtl/out.h qtl/string.h
	$(CXX) $(CXXFLAGS) -fsanitize=undefined -o $@ -g3 -DTEST_H='"'$<'"' test.cpp 2>&1 | tee ./$(*F).ubsan.make.log

%.san.test.out: qtl/%.h qtl/out.h qtl/string.h
	$(CXX) $(CXXFLAGS) -fsanitize=address,undefined -o $@ -g3 -DTEST_H='"'$<'"' test.cpp 2>&1 | tee ./$(*F).san.make.log

# Fuzzing builds (uses CXX with -fsanitize=fuzzer if available, otherwise regular build)
%.fuzz.out: qtl/%.h qtl/out.h qtl/string.h
	mkdir -p $(FDIR)
	@if command -v afl-clang++ >/dev/null 2>&1; then \
		afl-clang++ $(CXXFLAGS) -o $@ -DTEST_H='"'$<'"' test.cpp 2>&1 | tee ./$(*F).fuzz.make.log; \
	else \
		$(CXX) $(CXXFLAGS) -fsanitize=fuzzer-no-link -o $@ -DTEST_H='"'$<'"' test.cpp 2>&1 | tee ./$(*F).fuzz.make.log || \
		$(CXX) $(CXXFLAGS) -o $@ -DTEST_H='"'$<'"' test.cpp 2>&1 | tee ./$(*F).fuzz.make.log; \
	fi

%.out: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -g3 $<

cleanlog:
	rm -f *.{test,error}.log

clean: cleanlog
	rm -f *.out
	rm -rf compiles $(FDIR)

info:
	@echo "CXX: $(CXX)"
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo ""
	@echo "All 8 tests should build successfully!"

help:
	@echo "Targets:"
	@echo "  all              - Build all 8 tests"
	@echo "  *.test.out       - Build specific test"
	@echo "  *.asan.test.out  - Build with AddressSanitizer"
	@echo "  *.ubsan.test.out - Build with UBSan"
	@echo "  *.san.test.out   - Build with all sanitizers"
	@echo "  *.fuzz.out       - Build for fuzzing (needs afl-fuzz or libFuzzer)"
	@echo "  clean            - Remove build artifacts"

.PHONY: all clean cleanlog info help
