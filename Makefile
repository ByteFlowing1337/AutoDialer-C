CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 -Isrc/include
TARGET = autodialer

VERSION = 0.1.0

INCLUDE = src/include/*.h
SRC = src/cli/cli.c src/network/network.c

all: $(TARGET)

$(TARGET): src/main.c $(SRC) $(INCLUDE)
	$(CC) $(CFLAGS) -DVERSION=\"$(VERSION)\" src/main.c $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)