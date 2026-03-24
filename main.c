#include <stdio.h>
#include <unistd.h> // для usleep

#include "canopen_types.h"
#include "abstraction_layer.h"
#include "heartbeat.h"
// nmt.h больше не нужен в main, так как мы пока ничего не отправляем

int main() {
    int sock = can_init("can0");
    if (sock < 0) {
        fprintf(stderr, "Не удалось запустить CAN. Убедитесь, что 'can0' поднят.\n");
        return 1;
    }

    printf("Ожидание CANopen Heartbeat сообщений... Нажмите Ctrl+C для выхода.\n");

    CAN_Frame received_frame;
    while (1) {
        // Пытаемся принять кадр
        int result = can_receive(sock, &received_frame);

        if (result > 0) { // Если кадр получен
            uint8_t node_id, state;
            // Проверяем, не является ли кадр Heartbeat-сообщением
            if (Heartbeat_Parse(&received_frame, &node_id, &state)) {
                printf("Получен Heartbeat от узла %d. Состояние: %s (0x%02X)\n",
                       node_id, NMT_StateToString(state), state);
            }
            // Здесь можно будет добавить парсеры для SDO, PDO и т.д.
        } else if (result < 0) {
            // Если произошла ошибка, выходим из цикла
            break;
        }

        // Небольшая пауза, чтобы не загружать процессор на 100%
        usleep(1000); // 1 мс
    }

    can_close(sock);
    return 0;
}
