#include <stdio.h>
#include <unistd.h>

#include "canopen_types.h"
#include "abstraction_layer.h"
#include "heartbeat.h" // Оставляем, чтобы видеть, что узел жив
#include "sdo.h"       // Наш новый модуль

void print_frame(const CAN_Frame* frame) {
    printf("  ID: 0x%03X, DLC: %d, Data: ", frame->id, frame->dlc);
    for (int i = 0; i < frame->dlc; ++i) {
        printf("%02X ", frame->data[i]);
    }
    printf("\n");
}

int main() 
{
    int sock = can_init("can0");
    if (sock < 0) {
        fprintf(stderr, "Не удалось запустить CAN.\n");
        return 1;
    }

    // --- Отправка SDO-запроса ---
    CAN_Frame sdo_request_frame;
    uint8_t target_node_id = 5; // ID нашего STM32

    // Запрашиваем стандартный объект 0x1008, 0x00 - "Device Name"
    SDO_Create_Read_Request(&sdo_request_frame, target_node_id, 0x1008, 0);
    
    printf("Отправка SDO-запроса на чтение [0x1008:0x00] узлу %d\n", target_node_id);
    print_frame(&sdo_request_frame);

    if (can_send(sock, &sdo_request_frame) != 0) {
        fprintf(stderr, "Ошибка отправки SDO-запроса.\n");
        can_close(sock);
        return 1;
    }

    // --- Прослушивание ответов ---
    printf("\nОжидание ответов...\n");

    CAN_Frame received_frame;
    while (1) 
    {
        if (can_receive(sock, &received_frame) > 0) 
        {
            uint16_t rx_index;
            uint8_t rx_subindex;
            uint8_t rx_data[4];
            uint8_t rx_data_len;

            // Парсим как SDO-ответ от узла 5
            int sdo_result = SDO_Parse_Response(&received_frame, target_node_id, 
                                                   &rx_index, &rx_subindex, 
                                                   rx_data, &rx_data_len);

            if (sdo_result == 1) {
                printf("\n[SDO УСПЕХ] Узел %d ответил на индекс 0x%04X:%02X\n", 
                       target_node_id, rx_index, rx_subindex);
                printf("Данные (%d байт): %c%c%c%c\n\n", 
                       rx_data_len, rx_data[0], rx_data[1], rx_data[2], rx_data[3]);
            } 
            else if (sdo_result == -1) 
                printf("\n[SDO ОШИБКА] Узел %d вернул SDO Abort!\n\n", target_node_id);
            else 
            {
                // Если это не SDO, возможно это Heartbeat
                uint8_t hb_node, hb_state;
                if (Heartbeat_Parse(&received_frame, &hb_node, &hb_state))
                    printf("Heartbeat: Узел %d, Состояние 0x%02X\n", hb_node, hb_state);
                else
                    printf("ERROR: Неопознанный кадр!");
    
            }
        }
        usleep(1000);
    }

    can_close(sock);
    return 0;
}
