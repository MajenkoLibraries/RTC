#ifndef _RTCC_H
#define _RTCC_H

#include <RTC.h>

// Alarm and Chimes intervals
#define RTCC_AL_HALF_SECOND  0x0
#define RTCC_AL_SECOND       0x1
#define RTCC_AL_10_SECOND    0x2
#define RTCC_AL_MINUTE       0x3
#define RTCC_AL_10_MINUTE    0x4
#define RTCC_AL_HOUR         0x5
#define RTCC_AL_DAY          0x6
#define RTCC_AL_WEEK         0x7
#define RTCC_AL_MONTH        0x8
#define RTCC_AL_YEAR         0x9

#define RTCC_ALARM           0x0
#define RTCC_SECONDS         0x1

class RTCC : protected RTC {
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
