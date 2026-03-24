#ifndef CANOPEN_TYPES_H
#define CANOPEN_TYPES_H

#include <stdint.h>

// Абстрактный CANopen-Frame (DATA-Frame)
typedef struct {
    uint32_t id;        // Идентификатор кадра (COB-ID)
    uint8_t dlc;        // Кол-во байт данных
    uint8_t data[8];    // Массив данных 
    uint8_t rtr;        // Какой кадр мы шлём (0 - стандартный кадр)
} CAN_Frame;

/* ИСПРАВИТЬ */
typedef struct {
    uint8_t id; // айди узла надо нет ? хз
    uint16_t index; // Каждая запись имеет индекс
    uint8_t sub_index; // Используеться для структур данных и массивов
    uint64_t data_type;
} Object_dictionary;

#endif // CANOPEN_TYPES_H