CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -Iinclude -O2
SRC = src/modbus_rtu.c src/modbus_crc.c tests/test_modbus.c
TARGET = test_modbus

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o *.exe
