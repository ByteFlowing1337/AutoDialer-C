CC = gcc
FLAGS = -Wall -Wextra -Werror -O2
TARGET = autodialer

all: $(TARGET)

$(TARGET): src/cli.c src/cli.h
	$(CC) $(FLAGS) src/cli.c -o $(TARGET)

clean:
	rm -f $(TARGET)