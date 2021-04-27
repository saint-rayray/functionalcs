#
# Makefile for cprogramminglab
#
SHELL = /bin/bash
CC = clang
CFLAGS = -O0 -g -Wall -Werror -std=gnu99

# Path to LLVM binaries
LLVM_PATH = /usr/local/depot/llvm-7.0/bin/
ifneq (,$(wildcard /usr/lib/llvm-7/bin/))
  LLVM_PATH = /usr/lib/llvm-7/bin/
endif

# Targets to compile
HANDIN_TAR = cprogramminglab-handin.tar
FILES = qtest $(HANDIN_TAR)

.PHONY: all
all: $(FILES)

# List header dependencies
qtest.o: harness.h queue.h report.h console.h
harness.o: harness.h
console.o: report.h console.h
report.o: report.h
queue.o: harness.h queue.h

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

# Compile qtest binaries
qtest: qtest.o report.o console.o harness.o queue.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compile certain targets with ASAN
ASAN_TARGETS = qtest

$(ASAN_TARGETS): CFLAGS += -fsanitize=address
ASAN_ARCHIVE = /afs/cs.cmu.edu/academic/class/15213/lib/clang/7.0.0/lib/linux/libclang_rt.asan-x86_64.a
ifeq (,$(wildcard $(ASAN_ARCHIVE)))
  ASAN_ARCHIVE = ./libclang_rt.asan-x86_64.a
endif
ifneq (,$(wildcard $(ASAN_ARCHIVE)))
  $(ASAN_TARGETS): LDFLAGS += \
    -Wl,--whole-archive,$(ASAN_ARCHIVE) \
    -Wl,--no-whole-archive \
    -Wl,--export-dynamic \
    -Wl,--no-as-needed
  $(ASAN_TARGETS): LDLIBS += -lpthread -lrt -lm -ldl
else
  $(ASAN_TARGETS): LDLIBS += -fsanitize=address
endif

.PHONY: test
test: qtest driver.py
	chmod +x driver.py
	./driver.py

.PHONY: clean
clean:
	rm -rf $(FILES)
	rm -rf *.o *~ *.pyc *.dSYM
	(cd traces; rm -f *~)

# Include rules for submit, format, etc
FORMAT_FILES = queue.c queue.h
HANDIN_FILES = queue.c queue.h .clang-format
include helper.mk

# Add check-format dependencies
submit: | check-format
$(FILES): | check-format
