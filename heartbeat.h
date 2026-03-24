#ifndef HEARTBIT_H
#define HEARTDIT_H

#include "canopen_types.h"

// Состояния узла (NMT State) согласно CiA 301
#define NMT_STATE_BOOTUP          0x00
#define NMT_STATE_STOPPED         0x04
#define NMT_STATE_OPERATIONAL     0x05
#define NMT_STATE_PRE_OPERATIONAL 0x7F

/*
 * Функция для разбора CAN-кадра.
 * frame - указатель на принятый кадр
 * node_id - указатель, куда будет записан ID узла, от которого пришел Heartbeat
 * state - указатель, куда будет записано состояние этого узла
 * Возвращает: 1 если это Heartbeat-кадр, 0 - если нет.
 */
int Heartbeat_Parse(const CAN_Frame *frame, uint8_t *node_id, uint8_t *state);

// Вспомогательная функция для печати состояния
const char* NMT_StateToString(uint8_t state);

#endif