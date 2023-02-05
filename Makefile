TARGET = reinforth
CC = gcc
LDFLAGS = -g -O2
CFLAGS = -g -O2 -Isrc/

src =$(shell find src/ -name '*.c' -not -name 'main.c')
obj = $(src:.c=.o)

tests=$(shell find tests/ -name '*.c')
tests_bin=$(tests:.c=.bin)

all: $(TARGET)

$(TARGET): $(obj) src/main.c
	$(CC) $(LDFLAGS) -o $@ $(obj) src/main.c
 
#test: $(tests_bin)
#	@echo
#	@echo "Run tests:"
#	@scripts/runall.sh $^

#$(tests_bin):%.bin:%.c $(obj)
#	$(CC) $(CFLAGS) $(LDFLAGS) $< $(obj) -MD -MF $@.d -o $@

$(obj):%.o:%.c
	$(CC) -c $(CFLAGS) $< -MD -MF $@.d -o $@


clean:
	-rm $(TARGET) $(obj) $(tests_bin)
	-rm $(shell find . -name '*.d')
	-rm -rf build

fmt:
	-scripts/format.sh

install: $(TARGET)
	cp $(TARGET) ~/.local/bin/

DEPS := $(shell find . -name *.d)
ifneq ($(DEPS),)
include $(DEPS)
endif
