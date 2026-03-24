#include <stdio.h>
#include "canopen_types.h"
#include "nmt.h"
#include "abstraction_layer.h"

int main() 
{
    // 1. Инициализируем CAN-интерфейс "can0"
    int sock = can_init("can0");
    if (sock < 0) {
        fprintf(stderr, "Не удалось запустить CAN. Убедитесь, что интерфейс 'can0' поднят.\n");
        return 1;
    }

    // 2. Создаем пустой CAN-кадр
    CAN_Frame my_frame;

    // 3. Заполняем его NMT-командой "Start Node" для узла с ID = 1
    // (поменяйте 1 на ID вашего устройства, если он другой)
    uint8_t node_id_to_start = 1; 
    NMT_Create_Command(&my_frame, NMT_OPERATIONAL, node_id_to_start);

    printf("Отправка NMT-команды: Start Node %d\n", node_id_to_start);
    printf("ID: 0x%X, DLC: %d, Data: %02X %02X\n", 
           my_frame.id, my_frame.dlc, my_frame.data[0], my_frame.data[1]);

    // 4. Отправляем кадр в шину
    if (can_send(sock, &my_frame) == 0) {
        printf("Команда успешно отправлена!\n");
    } else {
        fprintf(stderr, "Не удалось отправить команду.\n");
    }

    // 5. Закрываем соединение
    can_close(sock);

    return 0;
}
