CC = cc
CFLAGS = -Wall -Wextra -I..
SRC = main.c sql3parse_debug.c ../sql3parse_table.c
TARGET = sql3parse_debug

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: clean
