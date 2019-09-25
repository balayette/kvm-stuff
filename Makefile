BINARY_OUT = main

CFLAGS = -Wall -Wextra -std=c11 -pedantic -g -MMD
CPPFLAGS = -Iinclude -D_GNU_SOURCE

OBJ = \
      src/main.o \
      src/vcpu.o \
      src/vm.o \
      src/kvmw.o \


DEP = $(OBJ:.o=.d)

.PHONY: opti
opti: CFLAGS += -O3
opti: all

.PHONY: lto
lto: CFLAGS += -flto
lto: LDFLAGS += -flto
lto: opti

.PHONY: debug
debug: CFLAGS += -Og
debug: all

.PHONY: all
all: $(BINARY_OUT)

$(BINARY_OUT): src/main
	cp src/main $(BINARY_OUT)

src/main: $(OBJ)

.PHONY: clean
clean:
	$(RM) $(OBJ) $(BINARY_OUT) $(DEP) src/main 

-include $(DEP)
