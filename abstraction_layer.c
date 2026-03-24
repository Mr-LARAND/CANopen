#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

/* Эти заголовочные файлы специфичны для SocketCAN в Linux! */
#include <linux/can.h>
#include <linux/can/raw.h>

#include "abstraction_layer.h"

int can_init(const char *ifname) 
{
    int sock_fd;
    struct sockaddr_can addr;
    struct ifreq ifr;

    // 1. Создаем сокет
    if ((sock_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Ошибка создания сокета");
        return -1;
    }

    // 2. Указываем имя CAN-интерфейса (например, "can0")
    strcpy(ifr.ifr_name, ifname);
    if (ioctl(sock_fd, SIOCGIFINDEX, &ifr) < 0) {
        perror("Ошибка ioctl");
        close(sock_fd);
        return -1;
    }

    // 3. Привязываем сокет к CAN-интерфейсу
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Ошибка привязки сокета");
        close(sock_fd);
        return -1;
    }

    printf("CAN-интерфейс %s успешно инициализирован.\n", ifname);
    return sock_fd;
}

int can_send(int sock_fd, const CAN_Frame *frame) 
{
    // Структура кадра, которую "понимает" ядро Linux
    struct can_frame linux_frame;

    // Конвертируем нашу универсальную структуру в структуру SocketCAN
    linux_frame.can_id = frame->id;
    linux_frame.can_dlc = frame->dlc;
    memcpy(linux_frame.data, frame->data, frame->dlc);
    
    // Отправляем кадр
    if (write(sock_fd, &linux_frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
        perror("Ошибка отправки кадра");
        return -1;
    }

    return 0;
}

void can_close(int sock_fd) {
    close(sock_fd);
}
