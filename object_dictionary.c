#include <stdint.h>

typedef struct {
    uint16_t index;
    uint8_t  subindex;
    uint8_t  size;       // Размер данных в байтах (1, 2, 4...)
    uint8_t  *data_ptr;  // Указатель на реальную переменную в памяти
    uint8_t  access;     // Права: RO, WO, RW
} OD_Entry_t;
