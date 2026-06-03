CC = gcc
VERSION = 0.1.0
PROGRAM_NAME = autodialer

CFLAGS = \
		-Wall \
		-Wextra \
		-Werror \
		-O2 \
		-Isrc/include \
		-DVERSION=\"$(VERSION)\" \
		-DPROGRAM_NAME=\"$(PROGRAM_NAME)\" \
		-MMD \
		-MP

ifeq ($(OS),Windows_NT)
	TARGET = autodialer.exe
	ifdef COMSPEC
		RM = del /Q
	else
		RM = rm -f
	endif
else
	TARGET = autodialer
	RM = rm -f
endif

OBJS = main.o cli.o network.o
DEPS = $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

cli.o: src/cli.c
	$(CC) $(CFLAGS) -c $< -o $@

network.o: src/network.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)