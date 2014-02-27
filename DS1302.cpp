#include <RTC.h>

void DS1302::begin()
{
	pinMode(_ce_pin, OUTPUT);
	pinMode(_sclk_pin, OUTPUT);
    digitalWrite(_ce_pin, LOW);
    halt(false);
    writeProtect(false);
}


DS1302::DS1302(uint8_t ce_pin, uint8_t data_pin, uint8_t sclk_pin)
{
	_ce_pin = ce_pin;
	_data_pin = data_pin;
	_sclk_pin = sclk_pin;
}

RTCValue DS1302::value() {
    RTCValue tv;

    uint8_t data[8];
    _burstRead(data);

    tv.time(
        _decodeH(data[2]),
        _decode(data[1]),
        _decode(data[0])
    );
    tv.date(
        _decodeY(data[6]),
        _decode(data[4]),
        _decode(data[3])
    );
    tv.dayOfWeek(_decode(data[5]));
    return tv;
}

void DS1302::set(RTCValue tv) {
    _writeRegister(REG_HOUR, _encode(tv.hours()));
    _writeRegister(REG_MIN , _encode(tv.minutes()));
    _writeRegister(REG_SEC,  _encode(tv.seconds()));
    _writeRegister(REG_YEAR, _encode(tv.year()));
    _writeRegister(REG_MON,  _encode(tv.month()));
    _writeRegister(REG_DATE, _encode(tv.day()));
    _writeRegister(REG_DOW,  tv.dayOfWeek());
}

void DS1302::halt(bool enable)
{
  uint8_t _reg = _readRegister(REG_SEC);
  _reg &= ~(1 << 7);
  _reg |= (enable << 7);
  _writeRegister(REG_SEC, _reg);
}

void DS1302::writeProtect(bool enable)
{
  uint8_t _reg = (enable << 7);
  _writeRegister(REG_WP, _reg);
}

/* Private */

uint8_t DS1302::_readByte()
{
	pinMode(_data_pin, INPUT);

	uint8_t value = 0;
	uint8_t currentBit = 0;

	for (int i = 0; i < 8; ++i)
	{
		currentBit = digitalRead(_data_pin);
		value |= (currentBit << i);
		digitalWrite(_sclk_pin, HIGH);
		delayMicroseconds(1);
		digitalWrite(_sclk_pin, LOW);
	}
	return value;
}

void DS1302::_writeByte(uint8_t value)
{
	pinMode(_data_pin, OUTPUT);
	shiftOut(_data_pin, _sclk_pin, LSBFIRST, value);
}

uint8_t DS1302::_readRegister(uint8_t reg)
{
	uint8_t cmdByte = 129;
	cmdByte |= (reg << 1);

	uint8_t readValue;

	digitalWrite(_sclk_pin, LOW);
	digitalWrite(_ce_pin, HIGH);

	_writeByte(cmdByte);
	readValue = _readByte();
	
	digitalWrite(_ce_pin, LOW);

	return readValue;
}

void DS1302::_writeRegister(uint8_t reg, uint8_t value)
{
	uint8_t cmdByte = (128 | (reg << 1));

	digitalWrite(_sclk_pin, LOW);
	digitalWrite(_ce_pin, HIGH);

	_writeByte(cmdByte);
	_writeByte(value);

	digitalWrite(_ce_pin, LOW);
}

void DS1302::_burstRead(uint8_t *regset)
{
	digitalWrite(_sclk_pin, LOW);
	digitalWrite(_ce_pin, HIGH);

	_writeByte(191);
	for (int i=0; i<8; i++)
	{
		regset[i] = _readByte();
	}
	digitalWrite(_ce_pin, LOW);
}

uint8_t	DS1302::_decode(uint8_t value)
{
	uint8_t decoded = value & 127;
	decoded = (decoded & 15) + 10 * ((decoded & (15 << 4)) >> 4);
	return decoded;
}

uint8_t DS1302::_decodeH(uint8_t value)
{
  if (value & 128)
    value = (value & 15) + (12 * ((value & 32) >> 5));
  else
    value = (value & 15) + (10 * ((value & 48) >> 4));
  return value;
}

uint8_t	DS1302::_decodeY(uint8_t value)
{
	uint8_t decoded = (value & 15) + 10 * ((value & (15 << 4)) >> 4);
	return decoded;
}

uint8_t DS1302::_encode(uint8_t value)
{
	uint8_t encoded = ((value / 10) << 4) + (value % 10);
	return encoded;
}
