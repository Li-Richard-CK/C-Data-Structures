CC = clang

CFLAGS := -I.
CFLAGS += -arch arm64
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpedantic
CFLAGS += -O2
CFLAGS += -O3
CFLAGS += -Wconversion
CFLAGS += -Wunused
CFLAGS += -std=c23
CFLAGS += -DDEBUG

SOURCES = $(shell find . -name "*.c")
OBJECTS = $(SOURCES:./%.c=build/%.o)
TARGET = build/test

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $^ -o $@

build/%.o: ./%.c
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: clean

clean:
	rm $(TARGET)
	rm $(OBJECTS)

all: $(TARGET)
