#ifndef _DS1302_H
#define _DS1302_H

#include <RTC.h>

class DS1302 : public RTC {
    public:
        static const uint8_t TCR_D1R2K  = 165;
        static const uint8_t TCR_D1R4K  = 166;
        static const uint8_t TCR_D1R8K  = 167;
        static const uint8_t TCR_D2R2K  = 169;
        static const uint8_t TCR_D2R4K  = 170;
        static const uint8_t TCR_D2R8K  = 171;
        static const uint8_t TCR_OFF    = 92;

        static const uint8_t MONDAY     = 1;
        static const uint8_t TUESDAY    = 2;
        static const uint8_t WEDNESDAY  = 3;
        static const uint8_t THURSDAY   = 4;
        static const uint8_t FRIDAY     = 5;
        static const uint8_t SATURDAY   = 6;
        static const uint8_t SUNDAY     = 7;

        static const uint8_t FORMAT_LITTLEENDIAN = 1;
        static const uint8_t FORMAT_BIGENDIAN    = 2;
        static const uint8_t FORMAT_MIDDLEENDIAN = 3;

        static const uint8_t FORMAT_SHORT = 1;
        static const uint8_t FORMAT_LONG  = 2;

        static const uint8_t REG_SEC  = 0;
        static const uint8_t REG_MIN  = 1;
        static const uint8_t REG_HOUR = 2;
        static const uint8_t REG_DATE = 3;
        static const uint8_t REG_MON  = 4;
        static const uint8_t REG_DOW  = 5;
        static const uint8_t REG_YEAR = 6;
        static const uint8_t REG_WP   = 7;
        static const uint8_t REG_TCR  = 8;

    public:
		DS1302(uint8_t ce_pin, uint8_t data_pin, uint8_t sclk_pin);
        void begin();

        // Standard interface functions

        RTCValue value();
        void set(RTCValue);

        // Chip specific functions and utilities

        void halt(bool);
        void writeProtect(bool);

    private:
        uint8_t _readByte();
        void _writeByte(uint8_t);

        uint8_t _readRegister(uint8_t reg);
        void _writeRegister(uint8_t reg, uint8_t value);
        void _burstRead(uint8_t *regset);

        uint8_t _decode(uint8_t value);
        uint8_t _decodeH(uint8_t value);
        uint8_t _decodeY(uint8_t value);

        uint8_t _encode(uint8_t value);

        uint8_t _ce_pin;
        uint8_t _sclk_pin;
        uint8_t _data_pin;
    
};
#endif
