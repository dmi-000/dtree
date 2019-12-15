CC = g++
#CC = clang++
SHELL = /bin/bash
clang++_flags = -isystem /usr/local/Cellar/llvm/9.0.0_1/include/c++/v1
g++_flags =
CXXFLAGS = $($(firstword $(CC))_flags) -v -I . -I util -DTEST_T1 -pthread $(CPPFLAGS)

WORDS = /usr/share/dict/words
CANON = /usr/local/lib/wordlist/canon
GOOCH = /usr/local/lib/wordlist/gooch/bigdatt77.txt
WORDS_DB = /usr/local/lib/wordlist/words.db

exer: exer.cpp
	$(CC) $(CXXFLAGS) -MMD exer.cpp -o exer

exer_radix: exer_radix.cpp
	$(CC) $(CXXFLAGS) -MMD exer_radix.cpp -o exer_radix

exer_radix_64: exer_radix.cpp
	$(CC) $(CXXFLAGS) -DBIT_64 -MMD exer_radix.cpp -o exer_radix_64

exer_char: exer_char.cpp
	$(CC) $(CXXFLAGS) -MMD exer_char.cpp -o exer_char

exer_64: exer.cpp
	$(CC) $(CXXFLAGS) -D BIT_64 -MMD exer.cpp -o exer_64

-include exer.d exer_char.d exer_64.d

dict.rad: exer dict
	exer -ldict -q

dict.map: exer dict
	exer -tdict -q

words.rad: exer words
	exer -lwords -q

words.map: exer words
	exer -twords -q

words: $(WORDS)
	ln -s $(WORDS) words

canon.rad.gz: canon.rad
	gzip -c canon.rad > canon.rad.gz

canon.rad: exer canon
	exer -icanon -q

canon.map: exer canon
	exer -tcanon -q

canon: $(CANON)
	ln -s $(CANON) canon

canon.can: canon ../canonical/canonical
	../canonical/canonical canon > canon.can

canon.sources: canon ../canonical/canonical
	fgrep -iwf canon $(WORDS_DB) > canon.sources

gooch.rad.gz: gooch.rad
	gzip -c gooch.rad > gooch.rad.gz

gooch.rad: exer_64 gooch
	exer_64 -igooch -q

gooch.map: exer gooch
	exer -tgooch -q

gooch: $(GOOCH)
	sed 's/ .*//' $(GOOCH) > gooch

gooch.can: gooch
	../canonical/canonical gooch > gooch.can

# TODO: not working
.can:
	../canonical/canonical < $* > $*.can

archive: shared_memory_allocator.zip

SRC = \
Makefile \
dict \
exer.cpp \
exer_char.cpp \
exer_radix.cpp \
iocommon.h \
ioerror.h \
radix_map.h \
shared_memory_allocator.h

shared_memory_allocator.zip: $(SRC)
	zip shared_memory_allocator.zip $(SRC)

test: producer_comsumer_associative.dat producer_comsumer_sequence.dat
	tail libstdc++-performance.sum

producer_comsumer_associative.dat: associative
	associative
	echo | cat producer_consumer_associative.dat -

producer_comsumer_sequence.dat: sequence
	sequence
	echo | cat producer_consumer_sequence.dat -

copy-7.3.0:
	rm -f ext producer_consumer tr1 util
	ln -s ~/Downloads/gcc-7.3.0/libstdc++-v3/testsuite/performance/23_containers/producer_consumer .
	cp producer_consumer/*.cc .
	ln -s ~/Downloads/gcc-7.3.0/libstdc++-v3/testsuite/ext .
	ln -s ~/Downloads/gcc-7.3.0/libstdc++-v3/testsuite/tr1 .
	ln -s ~/Downloads/gcc-7.3.0/libstdc++-v3/testsuite/util .

copy-7.4.0:
	rm -f ext producer_consumer tr1 util
	ln -s ~/Downloads/gcc-7.4.0/libstdc++-v3/testsuite/performance/23_containers/producer_consumer .
	cp producer_consumer/*.cc .
	ln -s ~/Downloads/gcc-7.4.0/libstdc++-v3/testsuite/ext .
	ln -s ~/Downloads/gcc-7.4.0/libstdc++-v3/testsuite/tr1 .
	ln -s ~/Downloads/gcc-7.4.0/libstdc++-v3/testsuite/util .

copy-8.3.0:
	rm -f ext producer_consumer tr1 util
	ln -s ~/Downloads/gcc-8.3.0/libstdc++-v3/testsuite/performance/23_containers/producer_consumer .
	cp producer_consumer/*.cc .
	ln -s ~/Downloads/gcc-8.3.0/libstdc++-v3/testsuite/ext .
	ln -s ~/Downloads/gcc-8.3.0/libstdc++-v3/testsuite/tr1 .
	ln -s ~/Downloads/gcc-8.3.0/libstdc++-v3/testsuite/util .
