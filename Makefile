BINARY_OUT = main

CFLAGS = -Wall -Wextra -std=c11 -pedantic -g -MMD
CPPFLAGS = -Iinclude -D_GNU_SOURCE

OBJ = \
    src/main.o \
    src/vcpu.o \
    src/vm.o \
    src/kvmw.o \
    src/exits.o \
    src/portio.o \
    src/uart.o \
    src/utils.o \
    src/load.o \
    src/gdt.o \


TEST_OBJ = \
    test/portio.o \
    test/simplest.o \

TEST_BINARY = $(notdir $(TEST_OBJ:.o=.binary))

DEP = $(OBJ:.o=.d)

.PHONY: debug
debug: CFLAGS += -Og
debug: all

.PHONY: opti
opti: CFLAGS += -O3
opti: all

.PHONY: lto
lto: CFLAGS += -flto
lto: LDFLAGS += -flto
lto: opti

.PHONY: all
all: $(BINARY_OUT) $(TEST_BINARY)

$(BINARY_OUT): src/main
	cp src/main $(BINARY_OUT)

src/main: $(OBJ)

test/%.o: test/%.S

%.binary: test/%.o
	objcopy -O binary $^ $@

%.elf: test/%.o
	gcc -T test/test.lds $^ -o $@

.PHONY: clean
clean:
	$(RM) $(OBJ) $(BINARY_OUT) $(DEP) src/main $(TEST_BINARY) $(TEST_OBJ)

-include $(DEP)
