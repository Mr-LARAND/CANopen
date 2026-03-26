#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "canopen_types.h"
#include "abstraction_layer.h"
#include "pdo.h"
#include "nmt.h"

int main() 
{
    int sock = can_init("can0");
    if (sock < 0) {
        fprintf(stderr, "Не удалось запустить CAN.\n");
        return 1;
    }
    uint16_t node_id = 5;
    CAN_Frame Tx_frame;
    // Перевод узла в Operational
    NMT_Create_Command(&Tx_frame, NMT_OPERATIONAL, node_id);

    can_send(sock, &Tx_frame);
    printf("Перевод узла %d в режим \"Operational\"\n", node_id);
    usleep(100000);

    // Формирование RxPDO1-Frame
    uint16_t control_word = 0x000F;
    uint8_t pdo_data[2];
    pdo_data[0] = (uint8_t)(control_word & 0xFF);
    pdo_data[1] = (uint8_t)((control_word >> 8) & 0xFF);

    PDO_Create_RxPDO(&Tx_frame, 1, node_id, pdo_data, 2);
    can_send(sock, &Tx_frame);

    // Слушаем шину на предмет Входящих TxPDO
    CAN_Frame Rx_frame;

    while (1)
    {
        if(can_receive(sock, &Rx_frame) > 0) {
            uint8_t pdo_num, node_id, data_len;
            uint8_t data[8];

            if(PDO_Parse_TxPDO(&Rx_frame, &pdo_num, &node_id, data, &data_len)) {
                if (data_len == 2){
                    uint16_t status_word = data[0] | (data[1] << 8);
                    printf("Получен TxPDO %d от узла %d. Statusword = 0x%04X\n", pdo_num, node_id, status_word);
                }
                else
                    printf("Получен TxPDO %d от узла %d. Раземр данных %d байт\n", pdo_num, node_id, data_len);
            }
        }
        usleep(1000);     
    }
    return 0;
}
