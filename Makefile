TARGET = gol

CC = gcc

DIR_BUILD = build
DIR_SRC = src
DIR_EXT = external

CFLAGS = -MMD -MP -Wall -Wextra -pedantic -std=c99
LFLAGS = -lm -lX11

INCLUDE = $(shell pkg-config --cflags raylib)
LFLAGS += $(shell pkg-config --libs raylib)

SRC = $(wildcard $(DIR_SRC)/*.c)
OBJS = $(patsubst $(DIR_SRC)/%.c,$(DIR_BUILD)/%.o,$(SRC))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) -o $@ $(OBJS)

$(DIR_BUILD)/%.o: $(DIR_SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

.PHONY: all clean

clean:
	rm -rf $(DIR_BUILD) $(TARGET)

