#ifndef _RTCC_H
#define _RTCC_H

#include <RTC.h>

class RTCC : protected RTC {
    public:
        // Alarm and Chimes intervals
        const static uint8_t AL_HALF_SECOND = 0x00;
        const static uint8_t AL_SECOND      = 0x01;
        const static uint8_t AL_10_SECOND   = 0x02;
        const static uint8_t AL_MINUTE      = 0x03;
        const static uint8_t AL_10_MINUTE   = 0x04;
        const static uint8_t AL_HOUR        = 0x05;
        const static uint8_t AL_DAY         = 0x06;
        const static uint8_t AL_WEEK        = 0x07;
        const static uint8_t AL_MONTH       = 0x08;
        const static uint8_t AL_YEAR        = 0x09;

        const static uint8_t ALARM          = 0x00;
        const static uint8_t SECONDS        = 0x01;

	public:
		void begin();
		RTCValue value();
		void set(RTCValue setting);

		void setValidity(char val);
		char getValidity();

		void outputEnable();
		void outputEnable(unsigned char);
		void outputDisable();

		RTCValue alarmValue();
		void alarmEnable();
		void alarmDisable();
		void chimeEnable();
		void chimeDisable();
		void alarmMask(unsigned char);
		void alarmRepeat(unsigned char);

		void alarmSet(RTCValue val);
		void attachInterrupt(void (*)());

	private:
		char _validity;
		void timeSync();
		void alarmSync();
};

#endif
