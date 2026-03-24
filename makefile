CXX=clang++

# C++23 for coroutines support (final standard, not early C++20)
CXXFLAGS= -v -Xlinker -L/opt/homebrew/opt/llvm/lib -fno-omit-frame-pointer -I/opt/homebrew/include

clang++_flags= -glldb --std=c++23 -fdebug-macro -fstandalone-debug -fdiagnostics-show-template-tree
g++_flags=--std=c++23 -fno-omit-frame-pointer -fpermissive

CXXFLAGS+=$($(firstword $(CXX))_flags)

ERROR_LEVEL=-1
FXX=/usr/local/bin/afl/afl-clang++
FUZZ=/usr/local/bin/afl/afl-fuzz
FENV=AFL_NO_ARITH=1 AFL_EXIT_WHEN_DONE=1 AFL_HANG_TMOUT=100
FDIR=fuzz
SPLIT=perl -pe 'BEGIN{$$a="aa"}open STDOUT,">$$ARGV[0]".$$a++;END{}' -

.SECONDARY:
.SUFFIXES:

%.test.log: %.test.out $(FDIR)/%.in/aa
	perl -MList::Util=shuffle -e 'print shuffle <>' $(FDIR)/$(*F).in/* | tee $(*F).test.in | ./$(*F).test.out 2>&1 | tee  $@

all: out.test.out string.test.out container.test.out number.test.out interval.test.out expr.test.out sql.test.out store.test.out

tests: out.test.log string.test.log container.test.log number.test.log interval.test.log expr.test.log sql.test.log store.test.log

interval.test.out: qtl/number.h qtl/container.h

store.test.out: qtl/number.h qtl/tree.h qtl/randstream.h qtl/interval.h qtl/container.h

sql.test.out: qtl/tree.h qtl/expr.h qtl/number.h qtl/interval.h qtl/store.h qtl/container.h

canonical.out: canonical.cpp

%.out: %.cpp
	$(CXX) $(CXXFLAGS) -v -o $@ -g3 $<

%.test.out: qtl/%.h qtl/out.h qtl/string.h
	( $(CXX) $(CXXFLAGS) -v -o $@ -g3  -DTEST_H='"'$<'"' -fdiagnostics-color=always test.cpp 2>&1 && cp -av $? compiles  ) | tee ./$(*F).make.$(subst /,_,$(firstword ${CXX})).log

%.test.out: qtl/%.hpp
	$(CXX) -o $@ $(CXXFLAGS) -DTEST_H='"'$<'"'  test.cpp

%.error.log: %.error.out
	 ./$(*F).error.out | tee  $@

%.error.out: qtl/%.h
	$(CXX) $(CXXFLAGS) -o $@  -DERROR_INJECT=$(ERROR_LEVEL) -fdiagnostics-color=always -DTEST_H='"'$<'"' test.cpp  2>&1 | tee ./$(*F).error.make.${CXX}.log

%.fuzz: %.fuzz.out $(FDIR)/%.cmin $(FDIR)/%.dict
	$(FENV) $(FUZZ) $(FFLAGS) -i $(FDIR)/$(*F).cmin -x $(FDIR)/$(*F).dict -o $(FDIR)/$(*F).out -- ./$(*F).fuzz.out

%.fuzz.log: %.fuzz.out $(FDIR)/%.in
	$(FUZZ) -i $(FDIR)/$(*F).in -o $(FDIR)/$(*F).out -- ./$(*F).fuzz.out 2>&1 | tee  $@

$(FDIR)/%.cmin:
	mkdir -pv $(FDIR)/$(*F).cmin/ && /usr/local/bin/afl/afl-cmin -i $(FDIR)/$(*F).in/  -o  $(FDIR)/$(*F).cmin/ -- ./$(*F).fuzz.out

$(FDIR)/%.in/aa: %.test.out
	mkdir -pv $(FDIR)/$(*F).in/ && ./$(*F).test.out fuzz | $(SPLIT) $(FDIR)/$(*F).in/

$(FDIR)/%.dict: %.fuzz.out
	./$(*F).fuzz.out dict > $(FDIR)/$(*F).dict;

$(FDIR)/%.in: %.fuzz.out
	mkdir -pv $(FDIR)/$(*F).in/ ;  ./$(*F).fuzz.out fuzz | $(SPLIT) $(FDIR)/$(*F).in/

%.fuzz.out: qtl/%.h qtl/out.h qtl/string.h
	AFL_HARDEN=1 $(FXX) -DFUZZING -D'TRACE(x)=' -o $@ -g3  $(CXXFLAGS)  -DTEST_H='"'$<'"' -fdiagnostics-color=always test.cpp 2>&1 | tee ./$(*F).fuzz.make.log

errors: out.error

test: cleanlog tests

%.debug: qtl/%.h
	 $(CXX) -glldb $(CXXFLAGS) -D__TEST='"'$<'"' test.cpp && gdb a.out

cleanlog:
	rm -f *.{test,error}.log

clean: cleanlog
	rm -f *.out
