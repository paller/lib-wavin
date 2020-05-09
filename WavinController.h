#ifndef _WAVIN_CONTROLLER_H
#define _WAVIN_CONTROLLER_H

#include <cstddef>
#include <cstdint>

typedef struct {
    void (*init)(void);
    size_t (*write)(uint8_t* data, uint8_t length);
    size_t (*read)(uint8_t* buffer, uint8_t length);
} io_descriptor_wavin;

class WavinController {
public:

    enum Category : uint8_t {
        CATEGORY_MAIN = 0x00,
        CATEGORY_ELEMENTS = 0x01,
        CATEGORY_PACKED_DATA = 0x02,
        CATEGORY_CHANNELS = 0x03,
        CATEGORY_RELAYS = 0x04,
        CATEGORY_CLOCK = 0x05,
        CATEGORY_SCHEDULES = 0x06,
        CATEGORY_INFO = 0x07,
    };

    enum Element : uint8_t {
        ELEMENTS_AIR_TEMPERATURE = 0x04,
        ELEMENTS_BATTERY_STATUS = 0x0A,
        ELEMENTS_SYNC_GROUP = 0x0B,
    };

    enum Data : uint8_t {
        PACKED_DATA_MANUAL_TEMPERATURE = 0x00,
        PACKED_DATA_STANDBY_TEMPERATURE = 0x04,
        PACKED_DATA_CONFIGURATION = 0x07,
        PACKED_DATA_CONFIGURATION_MODE_MASK = 0x07,
        PACKED_DATA_CONFIGURATION_MODE_MANUAL = 0x00,
        PACKED_DATA_CONFIGURATION_MODE_STANDBY = 0x01,
    };

    enum Channel : uint16_t {
        NUMBER_OF_CHANNELS = 16,
        CHANNELS_TIMER_EVENT = 0x0000,
        CHANNELS_TIMER_EVENT_OUTP_ON_MASK = 0x0010,
        CHANNELS_PRIMARY_ELEMENT = 0x0002,
        CHANNELS_PRIMARY_ELEMENT_ELEMENT_MASK = 0x003f,
        CHANNELS_PRIMARY_ELEMENT_ALL_TP_LOST_MASK = 0x0400,
    };

    WavinController(io_descriptor_wavin io_desciptor);
    bool readRegisters(Category category, uint8_t page, uint8_t index, uint8_t count, uint16_t* reply);
    bool writeRegister(Category category, uint8_t page, uint8_t index, uint16_t value);
    bool writeMaskedRegister(Category category, uint8_t page, uint8_t index, uint16_t value, uint16_t mask);

private:
    io_descriptor_wavin io;
    bool recieve(uint16_t* reply, uint8_t cmdtype);
    bool transmit(uint8_t* data, uint8_t lenght);

    unsigned int calculateCRC(unsigned char* frame, unsigned char bufferSize);

    const uint8_t MODBUS_DEVICE = 0x01;
    const uint8_t MODBUS_READ_REGISTER = 0x43;
    const uint8_t MODBUS_WRITE_REGISTER = 0x44;
    const uint8_t MODBUS_WRITE_MASKED_REGISTER = 0x45;

    // Largest page contains 22 registers of 2 bytes + 5 bytes header
    const uint8_t RECIEVE_BUFFER_SIZE = 22 * 2 + 5;
};

#endif /* _WAVIN_CONTROLLER_H */