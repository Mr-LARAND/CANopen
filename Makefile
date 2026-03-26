TARGET = can_master

CC = gcc

CFLAGS = -Wall -D_GNU_SOURCE

SRCS = CANopen/main.c CANopen/abstraction_layer.c CANopen/nmt.c CANopen/heartbeat.c CANopen/sdo.c CANopen/pdo.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@echo "Сборка $(TARGET) успешно завершена!"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Очистка завершена."

